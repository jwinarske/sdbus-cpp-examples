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

#ifndef SRC_BLUEZ_GATT_DESCRIPTOR1_H
#define SRC_BLUEZ_GATT_DESCRIPTOR1_H

#include "../proxy/org/bluez/GattDescriptor1/gatt_descriptor1_proxy.h"

class GattDescriptor1 final
    : public sdbus::ProxyInterfaces<org::bluez::GattDescriptor1_proxy> {
 public:
  struct Properties {
    sdbus::ObjectPath characteristic;
    std::string uuid;
    std::vector<std::uint8_t> value;
  };

  GattDescriptor1(sdbus::IConnection& connection,
                  const sdbus::ServiceName(&destination),
                  const sdbus::ObjectPath(&objectPath),
                  const std::map<sdbus::MemberName, sdbus::Variant>& properties)
      : ProxyInterfaces{connection, destination, objectPath} {
    // Iterate the provided properties once and dispatch on the key. This avoids
    // scanning every known property (and the per-property map lookup +
    // MemberName allocation).
    for (const auto& [key, value] : properties) {
      if (key == "Characteristic") {
        properties_.characteristic = value.get<sdbus::ObjectPath>();
      } else if (key == "UUID") {
        properties_.uuid = value.get<std::string>();
      } else if (key == "Value") {
        properties_.value = value.get<std::vector<std::uint8_t>>();
      }
    }
  }

  ~GattDescriptor1() = default;

  [[nodiscard]] const Properties& GetProperties() const { return properties_; }

 private:
  Properties properties_;
};

#endif  // SRC_BLUEZ_GATT_DESCRIPTOR1_H
