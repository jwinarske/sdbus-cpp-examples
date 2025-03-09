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

#ifndef SRC_BLUEZ_LE_ADVERTISING_MANAGER1_H
#define SRC_BLUEZ_LE_ADVERTISING_MANAGER1_H

#include "../proxy/org/bluez/LEAdvertisingManager1/le_advertising_manager1_proxy.h"

class LEAdvertisingManager1 final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::bluez::LEAdvertisingManager1_proxy> {
 public:
  struct Properties {
    std::uint8_t active_instances{};
    std::vector<std::string> supported_includes;
    std::uint8_t supported_instances{};
    std::vector<std::string> supported_secondary_channels;
  };

  LEAdvertisingManager1(
      sdbus::IConnection& connection,
      const sdbus::ServiceName(&destination),
      const sdbus::ObjectPath(&objectPath),
      const std::map<sdbus::MemberName, sdbus::Variant>& properties)
      : ProxyInterfaces{connection, destination, objectPath} {
    registerProxy();
    onPropertiesChanged(
        sdbus::InterfaceName(LEAdvertisingManager1_proxy::INTERFACE_NAME),
        properties, {});
  }

  virtual ~LEAdvertisingManager1() { unregisterProxy(); }

 private:
  Properties properties_{};

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    if (const auto key = sdbus::MemberName("ActiveInstances");
        changedProperties.contains(key)) {
      properties_.active_instances =
          changedProperties.at(key).get<std::uint8_t>();
    }
    if (const auto key = sdbus::MemberName("SupportedIncludes");
        changedProperties.contains(key)) {
      properties_.supported_includes =
          changedProperties.at(key).get<std::vector<std::string>>();
    }
    if (const auto key = sdbus::MemberName("SupportedInstances");
        changedProperties.contains(key)) {
      properties_.supported_instances =
          changedProperties.at(key).get<std::uint8_t>();
    }
    if (const auto key = sdbus::MemberName("SupportedSecondaryChannels");
        changedProperties.contains(key)) {
      properties_.supported_secondary_channels =
          changedProperties.at(key).get<std::vector<std::string>>();
    }
  }
};

#endif  // SRC_BLUEZ_LE_ADVERTISING_MANAGER1_H
