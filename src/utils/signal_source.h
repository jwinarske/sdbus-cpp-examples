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

#ifndef SRC_UTILS_SIGNAL_SOURCE_H
#define SRC_UTILS_SIGNAL_SOURCE_H

#include <cerrno>
#include <csignal>
#include <cstring>
#include <initializer_list>

#include <sys/signalfd.h>
#include <unistd.h>

#include "event_loop.h"
#include "logging.h"
#include "sys.h"
#include "unique_fd.h"

/// Turns POSIX signals into an EventLoop stop(). The signals are blocked
/// process-wide and delivered through a signalfd, so they are handled
/// synchronously on the loop thread (via dispatch()) rather than through an
/// async signal handler. Construct this before starting any worker threads so
/// they inherit the blocked mask and never take the default disposition.
class SignalSource final : public EventSource {
 public:
  explicit SignalSource(EventLoop& loop,
                        const std::initializer_list<int> signals = {SIGINT,
                                                                    SIGTERM})
      : loop_(loop) {
    sigemptyset(&mask_);
    for (const int signum : signals) {
      sigaddset(&mask_, signum);
    }
    if (sigprocmask(SIG_BLOCK, &mask_, nullptr) < 0) {
      LOG_ERROR("SignalSource: sigprocmask failed: {}", strerror(errno));
    }
    if (auto fd = sys::make_signalfd(-1, mask_, SFD_CLOEXEC | SFD_NONBLOCK)) {
      fd_ = std::move(*fd);
    } else {
      LOG_ERROR(
          "SignalSource: signalfd failed: {}; restoring default signal "
          "disposition",
          fd.error().message());
      // Undo the block, otherwise the signals stay blocked with no consumer and
      // the process can no longer be stopped via SIGINT/SIGTERM.
      sigprocmask(SIG_UNBLOCK, &mask_, nullptr);
    }
  }

  [[nodiscard]] int fd() const override { return fd_.get(); }

  void dispatch(short /*revents*/) override {
    signalfd_siginfo info{};
    while (::read(fd_.get(), &info, sizeof(info)) == sizeof(info)) {
      LOG_INFO("SignalSource: received signal {}, stopping", info.ssi_signo);
    }
    loop_.stop(0);
  }

 private:
  EventLoop& loop_;
  sigset_t mask_{};
  UniqueFd fd_;
};

#endif  // SRC_UTILS_SIGNAL_SOURCE_H
