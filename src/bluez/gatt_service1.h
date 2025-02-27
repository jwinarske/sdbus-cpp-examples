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

#ifndef SRC_BLUEZ_GATT_SERVICE1_H
#define SRC_BLUEZ_GATT_SERVICE1_H

#include "../proxy/org/bluez/GattService1/gatt_service1_proxy.h"

class GattService1
    : public sdbus::ProxyInterfaces<org::bluez::GattService1_proxy> {
 public:
  GattService1(sdbus::IConnection& connection,
               const sdbus::ServiceName(&destination),
               const sdbus::ObjectPath(&objectPath),
               const std::map<sdbus::MemberName, sdbus::Variant>& properties)
      : ProxyInterfaces{connection, destination, objectPath} {
    if (const auto key = sdbus::MemberName("Device");
        properties.contains(key)) {
      device_ = properties.at(key).get<sdbus::ObjectPath>();
    }
    if (const auto key = sdbus::MemberName("Includes");
        properties.contains(key)) {
      includes_ = properties.at(key).get<std::vector<sdbus::ObjectPath>>();
    }
    if (const auto key = sdbus::MemberName("Primary");
        properties.contains(key)) {
      primary_ = properties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("UUID"); properties.contains(key)) {
      uuid_ = properties.at(key).get<std::string>();
    }
  }

  ~GattService1() {
    for (auto& [object_path, characteristic] : gatt_characteristics_) {
      characteristic.reset();
    }
  }

  void AddCharacteristic(
      const sdbus::ObjectPath& objectPath,
      std::map<sdbus::MemberName, sdbus::Variant> const& properties) {
    gatt_characteristics_[objectPath] = std::make_unique<GattCharacteristic1>(
        getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
        objectPath, properties);
  }

  const std::unique_ptr<GattCharacteristic1>& getCharacteristic(
      const sdbus::ObjectPath& objectPath) {
    return gatt_characteristics_[objectPath];
  }

 private:
  sdbus::ObjectPath device_;
  std::vector<sdbus::ObjectPath> includes_;
  bool primary_;
  std::string uuid_;

  std::map<sdbus::ObjectPath, std::unique_ptr<GattCharacteristic1>>
      gatt_characteristics_;
};

#endif  // SRC_BLUEZ_GATT_SERVICE1_H
