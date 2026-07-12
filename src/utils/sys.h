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

#ifndef SRC_UTILS_SYS_H
#define SRC_UTILS_SYS_H

#include <cerrno>
#include <expected>
#include <system_error>

#include <fcntl.h>
#include <sys/eventfd.h>
#include <sys/signalfd.h>
#include <sys/stat.h>
#include <sys/timerfd.h>

#include "unique_fd.h"

/// Thin std::expected wrappers over the fd-creating syscalls.
///
/// Fallible system calls return std::expected<UniqueFd, std::error_code> so a
/// caller gets a [[nodiscard]], allocation-free error channel — errno mapped to
/// std::generic_category() — instead of a bare -1 it must remember to check,
/// and the fd is owned (closed on scope exit) the moment it is produced.
///
/// This is the errno layer only. D-Bus failures still surface as sdbus::Error
/// exceptions (sdbus-c++'s model), and absent-but-not-erroneous values still
/// use std::optional; both are the right tool for their layer.
namespace sys {

/// The current errno as a std::error_code.
[[nodiscard]] inline std::error_code last_error() noexcept {
  return {errno, std::generic_category()};
}

[[nodiscard]] inline std::expected<UniqueFd, std::error_code> open_fd(
    const char* path,
    const int flags) noexcept {
  UniqueFd fd(::open(path, flags));
  if (!fd.valid()) {
    return std::unexpected(last_error());
  }
  return fd;
}

[[nodiscard]] inline std::expected<UniqueFd, std::error_code>
open_fd(const char* path, const int flags, const mode_t mode) noexcept {
  UniqueFd fd(::open(path, flags, mode));
  if (!fd.valid()) {
    return std::unexpected(last_error());
  }
  return fd;
}

[[nodiscard]] inline std::expected<UniqueFd, std::error_code> make_eventfd(
    const unsigned initval,
    const int flags) noexcept {
  UniqueFd fd(::eventfd(initval, flags));
  if (!fd.valid()) {
    return std::unexpected(last_error());
  }
  return fd;
}

[[nodiscard]] inline std::expected<UniqueFd, std::error_code>
make_signalfd(const int fd, const sigset_t& mask, const int flags) noexcept {
  UniqueFd out(::signalfd(fd, &mask, flags));
  if (!out.valid()) {
    return std::unexpected(last_error());
  }
  return out;
}

[[nodiscard]] inline std::expected<UniqueFd, std::error_code> make_timerfd(
    const int clockid,
    const int flags) noexcept {
  UniqueFd fd(::timerfd_create(clockid, flags));
  if (!fd.valid()) {
    return std::unexpected(last_error());
  }
  return fd;
}

}  // namespace sys

#endif  // SRC_UTILS_SYS_H
