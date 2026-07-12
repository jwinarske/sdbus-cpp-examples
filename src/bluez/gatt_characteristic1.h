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
    // Iterate the provided properties once and dispatch on the key. This avoids
    // scanning every known property (and the per-property map lookup +
    // MemberName allocation). "Flags" sorts before "NotifyAcquired"/"Notifying"
    // in the map, so it is assigned before those guarded assignments are
    // reached, preserving the original isNotifyFlagSet() behavior.
    for (const auto& [key, value] : properties) {
      if (key == "Flags") {
        properties_.flags = value.get<std::vector<std::string>>();
      } else if (key == "NotifyAcquired") {
        if (isNotifyFlagSet()) {
          properties_.notify_acquired = value.get<bool>();
        }
      } else if (key == "Notifying") {
        if (isNotifyFlagSet()) {
          properties_.notifying = value.get<bool>();
        }
      } else if (key == "Service") {
        properties_.service = value.get<sdbus::ObjectPath>();
      } else if (key == "UUID") {
        properties_.uuid = value.get<std::string>();
      } else if (key == "Value") {
        properties_.value = value.get<std::vector<std::uint8_t>>();
      }
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
