// Copyright (c) 2025 Joel Winarske
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

#ifndef SRC_UTILS_UTILS_H
#define SRC_UTILS_UTILS_H

#include <sstream>
#include <unordered_map>

#include <sdbus-c++/sdbus-c++.h>

#include <spdlog/spdlog.h>

class Utils {
 public:
  static void append_property(const sdbus::Variant& value,
                              std::stringstream& string_stream);

  static void append_properties(
      const std::map<sdbus::MemberName, sdbus::Variant>& props,
      std::stringstream& ss);

  static void append_properties(
      const std::map<std::string, sdbus::Variant>& props,
      std::stringstream& ss);

  static void print_changed_properties(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties);

  static std::vector<std::string> ListNames(sdbus::IConnection& connection);

  static bool isServicePresent(const std::vector<std::string>& dbus_interfaces,
                               const std::string_view& service);

 private:
  static const std::unordered_map<
      std::string,
      std::function<void(const sdbus::Variant&, std::stringstream&)>>
      type_map_;
};

#endif  // SRC_UTILS_UTILS_H