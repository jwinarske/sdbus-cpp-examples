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
    if (const auto key = sdbus::MemberName("Characteristic");
        properties.contains(key)) {
      properties_.characteristic = properties.at(key).get<sdbus::ObjectPath>();
    }
    if (const auto key = sdbus::MemberName("UUID"); properties.contains(key)) {
      properties_.uuid = properties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("Value"); properties.contains(key)) {
      properties_.value = properties.at(key).get<std::vector<std::uint8_t>>();
    }
  }

  ~GattDescriptor1() = default;

  [[nodiscard]] const Properties& GetProperties() const { return properties_; }

 private:
  Properties properties_;
};

#endif  // SRC_BLUEZ_GATT_DESCRIPTOR1_H
