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

#ifndef SRC_BLUEZ_ADAPTER1_H
#define SRC_BLUEZ_ADAPTER1_H

#include "../proxy/org/bluez/Adapter1/adapter1_proxy.h"

#include "../utils/utils.h"

class Adapter1 final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::bluez::Adapter1_proxy> {
 public:
  Adapter1(sdbus::IConnection& connection,
           const sdbus::ServiceName(&destination),
           const sdbus::ObjectPath(&objectPath),
           const std::map<sdbus::PropertyName, sdbus::Variant>& properties)
      : ProxyInterfaces{connection, destination, objectPath} {
    registerProxy();
    onPropertiesChanged(sdbus::InterfaceName(Adapter1_proxy::INTERFACE_NAME),
                        properties, {});
  }

  virtual ~Adapter1() { unregisterProxy(); }

 private:
  std::string address_;
  std::string address_type_;
  std::string alias_;
  std::uint32_t class_{};
  bool discoverable_{};
  std::uint32_t discoverable_timeout_{};
  bool discovering_{};
  std::string modalias_;
  std::string name_;
  bool pairable_{};
  std::uint32_t pairable_timeout_{};
  bool powered_{};
  std::vector<std::string> uuids_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    if (const auto key = sdbus::MemberName("Address");
        changedProperties.contains(key)) {
      address_ = changedProperties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("AddressType");
        changedProperties.contains(key)) {
      address_type_ = changedProperties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("Alias");
        changedProperties.contains(key)) {
      alias_ = changedProperties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("Class");
        changedProperties.contains(key)) {
      class_ = changedProperties.at(key).get<std::uint32_t>();
    }
    if (const auto key = sdbus::MemberName("Discoverable");
        changedProperties.contains(key)) {
      discoverable_ = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("DiscoverableTimeout");
        changedProperties.contains(key)) {
      discoverable_timeout_ = changedProperties.at(key).get<std::uint32_t>();
    }
    if (const auto key = sdbus::MemberName("Discovering");
        changedProperties.contains(key)) {
      discovering_ = changedProperties.at(key).get<bool>();

      if (!discovering_) {
        this->StartDiscovery();
      }
    }
    if (const auto key = sdbus::MemberName("Modalias");
        changedProperties.contains(key)) {
      modalias_ = changedProperties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("Name");
        changedProperties.contains(key)) {
      name_ = changedProperties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("Pairable");
        changedProperties.contains(key)) {
      pairable_ = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("PairableTimeout");
        changedProperties.contains(key)) {
      pairable_timeout_ = changedProperties.at(key).get<std::uint32_t>();
    }
    if (const auto key = sdbus::MemberName("Powered");
        changedProperties.contains(key)) {
      powered_ = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("UUIDs");
        changedProperties.contains(key)) {
      uuids_ = changedProperties.at(key).get<std::vector<std::string>>();
    }
  }
};

#endif  // SRC_BLUEZ_ADAPTER1_H
