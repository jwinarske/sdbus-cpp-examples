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

#ifndef SRC_BLUEZ_HID_ERROR_H
#define SRC_BLUEZ_HID_ERROR_H

#include <string>
#include <system_error>

/// Domain error category for HID feature-report handling. Failures that are not
/// errno (a well-formed ioctl that returns a response failing a protocol check)
/// carry a std::error_code in this category, so they compose with the errno
/// codes from sys:: through the same std::expected<T, std::error_code> channel.
namespace hid {

enum class Error {
  kInvalidResponse = 1,  // ioctl succeeded but the report failed validation
};

inline const std::error_category& error_category() noexcept {
  static const struct Category final : std::error_category {
    [[nodiscard]] const char* name() const noexcept override { return "hid"; }
    [[nodiscard]] std::string message(const int condition) const override {
      switch (static_cast<Error>(condition)) {
        case Error::kInvalidResponse:
          return "invalid device response";
        default:
          return "unknown hid error";
      }
    }
  } category;
  return category;
}

inline std::error_code make_error_code(const Error e) noexcept {
  return {static_cast<int>(e), error_category()};
}

}  // namespace hid

template <>
struct std::is_error_code_enum<hid::Error> : std::true_type {};

#endif  // SRC_BLUEZ_HID_ERROR_H
