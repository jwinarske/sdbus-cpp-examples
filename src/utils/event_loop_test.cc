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

// Self-test for EventLoop / EventSource. Drives the system bus together with
// two fd sources on a single thread and asserts that:
//   1. a source is dispatched once per readable token (semaphore eventfd),
//   2. an async D-Bus call's reply is delivered by run() (proving the loop
//      drives processPendingEvent), and
//   3. stop() unblocks run() and returns the requested code.
// A timerfd acts as a hard deadline so the test can never hang.

#include <cstdint>
#include <cstring>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include <sdbus-c++/sdbus-c++.h>

#include "event_loop.h"
#include "logging.h"
#include "unique_fd.h"

namespace {

// Shared completion state. The loop stops cleanly only once BOTH the token
// source has drained and the async D-Bus reply has arrived — either may finish
// first, so whichever completes last triggers the stop.
struct Completion {
  EventLoop* loop = nullptr;
  bool tokens_done = false;
  bool reply_ok = false;

  void finish() const {
    if (tokens_done && reply_ok) {
      loop->stop(0);
    }
  }
};

// Reads one token per dispatch; marks completion once `target` are drained.
class TokenSource final : public EventSource {
 public:
  TokenSource(const int fd, Completion& done, const int target)
      : fd_(fd), done_(done), target_(target) {}

  [[nodiscard]] int fd() const override { return fd_; }

  void dispatch(short /*revents*/) override {
    std::uint64_t token = 0;
    if (::read(fd_, &token, sizeof(token)) == sizeof(token)) {
      ++count_;
    }
    if (count_ >= target_) {
      done_.tokens_done = true;
      done_.finish();
    }
  }

  [[nodiscard]] int count() const { return count_; }

 private:
  int fd_;
  Completion& done_;
  int target_;
  int count_ = 0;
};

// Hard deadline: stops the loop with a failure code if it ever fires.
class DeadlineSource final : public EventSource {
 public:
  DeadlineSource(const int fd, EventLoop& loop) : fd_(fd), loop_(loop) {}

  [[nodiscard]] int fd() const override { return fd_; }

  void dispatch(short /*revents*/) override {
    LOG_ERROR("EventLoop test: deadline expired before conditions were met");
    fired_ = true;
    loop_.stop(2);
  }

  [[nodiscard]] bool fired() const { return fired_; }

 private:
  int fd_;
  EventLoop& loop_;
  bool fired_ = false;
};

}  // namespace

int main() {
  std::unique_ptr<sdbus::IConnection> connection;
  try {
    connection = sdbus::createSystemBusConnection();
  } catch (const sdbus::Error& e) {
    // No system bus reachable (e.g. a minimal sandbox). Nothing to exercise —
    // report success so the test is portable.
    LOG_WARN("EventLoop test: no system bus ({}); skipping", e.getMessage());
    return 0;
  }

  EventLoop loop;

  // Semaphore eventfd pre-loaded with 3 tokens -> exactly 3 dispatches.
  constexpr int kTokens = 3;
  UniqueFd token_fd(::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK | EFD_SEMAPHORE));
  if (!token_fd.valid()) {
    LOG_ERROR("EventLoop test: eventfd failed: {}", strerror(errno));
    return 1;
  }
  const std::uint64_t seed = kTokens;
  if (::write(token_fd.get(), &seed, sizeof(seed)) != sizeof(seed)) {
    LOG_ERROR("EventLoop test: seeding eventfd failed: {}", strerror(errno));
    return 1;
  }

  // 5s deadline so the test cannot hang.
  UniqueFd timer_fd(::timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC));
  if (!timer_fd.valid()) {
    LOG_ERROR("EventLoop test: timerfd failed: {}", strerror(errno));
    return 1;
  }
  itimerspec spec{};
  spec.it_value.tv_sec = 5;
  if (::timerfd_settime(timer_fd.get(), 0, &spec, nullptr) < 0) {
    LOG_ERROR("EventLoop test: timerfd_settime failed: {}", strerror(errno));
    return 1;
  }

  Completion done;
  done.loop = &loop;

  TokenSource tokens(token_fd.get(), done, kTokens);
  DeadlineSource deadline(timer_fd.get(), loop);
  loop.add(&tokens);
  loop.add(&deadline);

  // Async D-Bus call whose reply must be delivered by run() (proving the loop
  // drives processPendingEvent). Its arrival is one of the two stop conditions.
  auto proxy = sdbus::createProxy(*connection,
                                  sdbus::ServiceName("org.freedesktop.DBus"),
                                  sdbus::ObjectPath("/org/freedesktop/DBus"));
  proxy->callMethodAsync("ListNames")
      .onInterface("org.freedesktop.DBus")
      // sdbus deduces the reply-handler argument types via function_traits and
      // requires the error to be taken by value, so the copy is API-mandated.
      // NOLINTNEXTLINE(performance-unnecessary-value-param)
      .uponReplyInvoke([&](std::optional<sdbus::Error> error,
                           const std::vector<std::string>& names) {
        done.reply_ok = !error && !names.empty();
        LOG_INFO("EventLoop test: ListNames reply ({} names, ok={})",
                 names.size(), done.reply_ok);
        done.finish();
      });

  const int rc = loop.run(*connection);

  const bool passed = rc == 0 && !deadline.fired() &&
                      tokens.count() == kTokens && done.reply_ok;
  if (passed) {
    LOG_INFO("EventLoop test: PASS (tokens={}, reply_ok={})", tokens.count(),
             done.reply_ok);
    return 0;
  }
  LOG_ERROR(
      "EventLoop test: FAIL (rc={}, deadline_fired={}, tokens={}, reply_ok={})",
      rc, deadline.fired(), tokens.count(), done.reply_ok);
  return 1;
}
