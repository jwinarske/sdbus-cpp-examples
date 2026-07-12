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

// Self-test for the sys:: std::expected syscall wrappers: a success returns an
// owned, valid fd; a failure returns the expected std::error_code.

#include <system_error>

#include <fcntl.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>

#include "logging.h"
#include "sys.h"

int main() {
  int failures = 0;
  const auto check = [&failures](const bool cond, const char* what) {
    if (cond) {
      LOG_INFO("sys test: {} - ok", what);
    } else {
      LOG_ERROR("sys test: {} - FAIL", what);
      ++failures;
    }
  };

  // open_fd success: /dev/null always exists and is readable.
  if (auto fd = sys::open_fd("/dev/null", O_RDONLY | O_CLOEXEC)) {
    check(fd->valid(), "open_fd(/dev/null) yields a valid fd");
  } else {
    check(false, "open_fd(/dev/null) unexpectedly failed");
  }

  // open_fd failure: a missing path yields the matching error_code, no fd.
  if (auto fd = sys::open_fd("/nonexistent/sdbus-cpp-examples/xyz",
                             O_RDONLY | O_CLOEXEC)) {
    check(false, "open_fd(missing) unexpectedly succeeded");
  } else {
    check(fd.error() == std::errc::no_such_file_or_directory,
          "open_fd(missing) reports ENOENT");
  }

  // make_eventfd / make_timerfd success.
  check(sys::make_eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK).has_value(),
        "make_eventfd yields a valid fd");
  check(sys::make_timerfd(CLOCK_MONOTONIC, TFD_CLOEXEC).has_value(),
        "make_timerfd yields a valid fd");

  if (failures == 0) {
    LOG_INFO("sys test: PASS");
    return 0;
  }
  LOG_ERROR("sys test: FAIL ({} check(s))", failures);
  return 1;
}
