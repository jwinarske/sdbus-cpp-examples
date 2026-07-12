// Copyright (c) 2026 Joel Winarske
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "event_loop.h"

#include <algorithm>
#include <cerrno>
#include <cstdint>
#include <cstring>

#include <sys/eventfd.h>
#include <unistd.h>

#include <sdbus-c++/sdbus-c++.h>

#include "logging.h"
#include "sys.h"

namespace {
constexpr std::size_t kNotPresent = static_cast<std::size_t>(-1);
}  // namespace

EventLoop::EventLoop() {
  if (auto fd = sys::make_eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK)) {
    wake_fd_ = std::move(*fd);
  } else {
    LOG_ERROR("EventLoop: failed to create eventfd: {}", fd.error().message());
  }
}

void EventLoop::wake() noexcept {
  if (!wake_fd_.valid()) {
    return;
  }
  constexpr std::uint64_t one = 1;
  if (::write(wake_fd_.get(), &one, sizeof(one)) < 0 && errno != EAGAIN) {
    LOG_ERROR("EventLoop: failed to signal wake eventfd: {}", strerror(errno));
  }
}

void EventLoop::add(EventSource* source) {
  to_add_.push_back(source);
  wake();
}

void EventLoop::remove(EventSource* source) {
  to_remove_.push_back(source);
  wake();
}

void EventLoop::retire(std::unique_ptr<EventSource> source) {
  to_remove_.push_back(source.get());
  // Hold ownership until the next apply_pending() destroys it, so the object
  // outlives the current dispatch pass (its fd may still be in this
  // iteration's poll set).
  to_retire_.push_back(std::move(source));
  wake();
}

void EventLoop::stop(const int exit_code) noexcept {
  // release/acquire so run() observes exit_code_ once it sees running_ == false
  // (independently of the eventfd/poll barriers).
  exit_code_.store(exit_code, std::memory_order_relaxed);
  running_.store(false, std::memory_order_release);
  wake();  // write() is async-signal-safe
}

void EventLoop::apply_pending() {
  for (auto* source : to_remove_) {
    std::erase(sources_, source);
  }

  for (auto* source : to_add_) {
    // Skip a source that is also being removed/retired this round: added and
    // retired within the same iteration, its object is about to be destroyed
    // by the to_retire_ clear below, so re-inserting its pointer would dangle.
    if (std::ranges::find(to_remove_, source) != to_remove_.end()) {
      continue;
    }
    if (std::ranges::find(sources_, source) == sources_.end()) {
      sources_.push_back(source);
    }
  }
  to_add_.clear();
  to_remove_.clear();
  // Now that any retired sources are out of sources_ and cannot be re-added, it
  // is safe to destroy them.
  to_retire_.clear();
}

int EventLoop::run(sdbus::IConnection& bus) {
  running_.store(true, std::memory_order_relaxed);

  while (running_.load(std::memory_order_acquire)) {
    apply_pending();
    if (!running_.load(std::memory_order_acquire)) {
      break;
    }

    const auto pd = bus.getEventLoopPollData();

    std::vector<pollfd> pfds;
    pfds.reserve(sources_.size() + 3);

    const std::size_t dbus_idx = pfds.size();
    pfds.push_back({pd.fd, pd.events, 0});

    std::size_t dbus_event_idx = kNotPresent;
    if (pd.eventFd >= 0) {
      dbus_event_idx = pfds.size();
      pfds.push_back({pd.eventFd, POLLIN, 0});
    }

    const std::size_t wake_idx = pfds.size();
    pfds.push_back({wake_fd_.get(), POLLIN, 0});

    const std::size_t first_source_idx = pfds.size();
    for (const auto* source : sources_) {
      pfds.push_back({source->fd(), source->events(), 0});
    }

    const int n = ::poll(pfds.data(), pfds.size(), pd.getPollTimeout());
    if (n < 0) {
      if (errno == EINTR) {
        continue;
      }
      LOG_ERROR("EventLoop: poll failed: {}", strerror(errno));
      exit_code_.store(1, std::memory_order_relaxed);
      break;
    }

    // Drain the wake eventfd so it doesn't stay readable.
    if ((pfds[wake_idx].revents & POLLIN) != 0) {
      std::uint64_t drained = 0;
      while (::read(wake_fd_.get(), &drained, sizeof(drained)) ==
             sizeof(drained)) {
      }
    }

    // Drive D-Bus whenever its fd/eventFd is ready, or on timeout so sd-bus can
    // recalculate and fire its own timers.
    const bool dbus_ready =
        n == 0 || pfds[dbus_idx].revents != 0 ||
        (dbus_event_idx != kNotPresent && pfds[dbus_event_idx].revents != 0);
    if (dbus_ready) {
      try {
        while (bus.processPendingEvent()) {
        }
      } catch (const sdbus::Error& e) {
        LOG_ERROR("EventLoop: D-Bus processing error: {} - {}", e.getName(),
                  e.getMessage());
        exit_code_.store(1, std::memory_order_relaxed);
        break;
      }
    }

    // Dispatch ready sources. sources_ is only mutated in apply_pending() (top
    // of the loop), so indexing the snapshot built above stays valid even if a
    // dispatch() calls add()/remove().
    for (std::size_t i = 0; i < sources_.size(); ++i) {
      const short revents = pfds[first_source_idx + i].revents;
      if (revents == 0) {
        continue;
      }
      sources_[i]->dispatch(revents);
      // poll() reports POLLHUP/POLLERR level-triggered and unmaskable, so a
      // hung-up or errored fd stays "ready" every iteration. Stop polling it
      // (deferred to the next apply_pending()) so it can't spin the loop while
      // its owner tears the source down.
      if ((revents & (POLLHUP | POLLERR)) != 0) {
        remove(sources_[i]);
      }
    }
  }

  return exit_code_.load(std::memory_order_acquire);
}
