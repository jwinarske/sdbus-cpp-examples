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

#ifndef SRC_BLUEZ_GATT_CHARACTERISTIC1_H
#define SRC_BLUEZ_GATT_CHARACTERISTIC1_H

#include "../proxy/org/bluez/GattCharacteristic1/gatt_characteristic1_proxy.h"

class GattCharacteristic1 final
    : public sdbus::ProxyInterfaces<org::bluez::GattCharacteristic1_proxy> {
 public:
  struct Properties {
    std::vector<std::string> flags;
    bool notify_acquired{};
    bool notifying{};
    sdbus::ObjectPath service;
    std::string uuid;
    std::vector<std::uint8_t> value;
  };

  GattCharacteristic1(
      sdbus::IConnection& connection,
      const sdbus::ServiceName(&destination),
      const sdbus::ObjectPath(&objectPath),
      const std::map<sdbus::MemberName, sdbus::Variant>& properties)
      : ProxyInterfaces{connection, destination, objectPath} {
    if (const auto key = sdbus::MemberName("Flags"); properties.contains(key)) {
      properties_.flags = properties.at(key).get<std::vector<std::string>>();
    }
    if (isNotifyFlagSet()) {
      if (const auto key = sdbus::MemberName("NotifyAcquired");
          properties.contains(key)) {
        properties_.notify_acquired = properties.at(key).get<bool>();
      }
      if (const auto key = sdbus::MemberName("Notifying");
          properties.contains(key)) {
        properties_.notifying = properties.at(key).get<bool>();
      }
    }
    if (const auto key = sdbus::MemberName("Service");
        properties.contains(key)) {
      properties_.service = properties.at(key).get<sdbus::ObjectPath>();
    }
    if (const auto key = sdbus::MemberName("UUID"); properties.contains(key)) {
      properties_.uuid = properties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("Value"); properties.contains(key)) {
      properties_.value = properties.at(key).get<std::vector<std::uint8_t>>();
    }
  }

  [[nodiscard]] bool isNotifyFlagSet() const {
    if (std::ranges::find(properties_.flags, "notify") !=
        properties_.flags.end()) {
      return true;
    }
    return false;
  }

  ~GattCharacteristic1() = default;

  [[nodiscard]] const Properties& GetProperties() const { return properties_; }

 private:
  Properties properties_;
};

#endif  // SRC_BLUEZ_GATT_CHARACTERISTIC1_H
