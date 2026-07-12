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

#ifndef SRC_UTILS_UNIQUE_FD_H
#define SRC_UTILS_UNIQUE_FD_H

#include <unistd.h>

/// RAII owner for a POSIX file descriptor.
///
/// Closes the fd on destruction, preventing leaks on every error-path
/// break/return/exception. Non-copyable, movable. A UniqueFd may either own
/// its fd (the default — it is closed on destruction/reset) or borrow one whose
/// lifetime is managed elsewhere (see Borrow()); a borrowed fd is never closed.
/// This matters for descriptors received over D-Bus (sdbus::UnixFd) or handed
/// in by a caller, where closing here would be a double-close.
class UniqueFd {
 public:
  UniqueFd() noexcept = default;

  explicit UniqueFd(const int fd) noexcept : fd_(fd) {}

  /// Wrap a borrowed fd this object must NOT close.
  [[nodiscard]] static UniqueFd Borrow(const int fd) noexcept {
    UniqueFd f(fd);
    f.owns_ = false;
    return f;
  }

  ~UniqueFd() { reset(); }

  // Non-copyable.
  UniqueFd(const UniqueFd&) = delete;
  UniqueFd& operator=(const UniqueFd&) = delete;

  // Movable.
  UniqueFd(UniqueFd&& other) noexcept : fd_(other.fd_), owns_(other.owns_) {
    other.fd_ = -1;
    other.owns_ = true;
  }
  UniqueFd& operator=(UniqueFd&& other) noexcept {
    if (this != &other) {
      reset();
      fd_ = other.fd_;
      owns_ = other.owns_;
      other.fd_ = -1;
      other.owns_ = true;
    }
    return *this;
  }

  /// Returns true if the fd is valid (>= 0).
  [[nodiscard]] bool valid() const noexcept { return fd_ >= 0; }
  explicit operator bool() const noexcept { return fd_ >= 0; }

  /// Returns the raw file descriptor without transferring ownership.
  [[nodiscard]] int get() const noexcept { return fd_; }

  /// Relinquish ownership and return the fd; the caller becomes responsible
  /// for closing it. Leaves this wrapper empty.
  [[nodiscard]] int release() noexcept {
    const int fd = fd_;
    fd_ = -1;
    owns_ = true;
    return fd;
  }

  /// Close the currently-owned fd (if any) and optionally adopt a new one,
  /// which is then owned.
  void reset(const int fd = -1) noexcept {
    if (owns_ && fd_ >= 0 && fd_ != fd) {
      ::close(fd_);
    }
    fd_ = fd;
    owns_ = true;
  }

 private:
  int fd_ = -1;
  bool owns_ = true;
};

#endif  // SRC_UTILS_UNIQUE_FD_H
