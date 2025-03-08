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

#ifndef SRC_BLUEZ_DEVICE1_H
#define SRC_BLUEZ_DEVICE1_H

#include <optional>
#include <regex>

#include "../proxy/org/bluez/Device1/device1_proxy.h"

class Device1 final : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                                    org::bluez::Device1_proxy> {
 public:
  struct Modalias {
    std::string vid;
    std::string pid;
    std::string did;
  };

  struct Properties {
    std::string adapter;
    std::string address;
    std::string address_type;
    std::string alias;
    bool blocked;
    bool bonded;
    bool connected;
    bool legacy_pairing;
    std::optional<Modalias> modalias;
    std::string name;
    bool paired;
    std::map<std::uint16_t, sdbus::Variant> manufacturer_data;
    std::map<std::string, sdbus::Variant> service_data;
    std::int16_t rssi;
    bool services_resolved;
    bool trusted;
    std::vector<std::string> uuids;
  };

  Device1(sdbus::IConnection& connection,
          const sdbus::ServiceName(&destination),
          const sdbus::ObjectPath(&objectPath),
          const std::map<sdbus::PropertyName, sdbus::Variant>& properties)
      : ProxyInterfaces{connection, destination, objectPath} {
    registerProxy();
    spdlog::debug("Device1: {}", objectPath);
    onPropertiesChanged(sdbus::InterfaceName(Device1_proxy::INTERFACE_NAME),
                        properties, {});
  }

  virtual ~Device1() { unregisterProxy(); }

  [[nodiscard]] const Properties& GetProperties() const { return properties_; }

  static std::optional<Modalias> parse_modalias(const std::string& mod_alias) {
    const std::regex re(
        R"(usb:v([0-9A-Fa-f]{4})p([0-9A-Fa-f]{4})d([0-9A-Fa-f]{4}))");
    if (std::smatch match;
        std::regex_search(mod_alias, match, re) && match.size() == 4) {
      return Modalias{match[1].str(), match[2].str(), match[3].str()};
    }
    spdlog::error("Failed to parse modalias");
    return {};
  }

 private:
  sdbus::ObjectPath adapter_;

  Properties properties_{};

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    if (const auto key = sdbus::MemberName("Adapter");
        changedProperties.contains(key)) {
      properties_.adapter = changedProperties.at(key).get<sdbus::ObjectPath>();
    }
    if (const auto key = sdbus::MemberName("Address");
        changedProperties.contains(key)) {
      properties_.address = changedProperties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("AddressType");
        changedProperties.contains(key)) {
      properties_.address_type = changedProperties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("Bonded");
        changedProperties.contains(key)) {
      properties_.bonded = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("Blocked");
        changedProperties.contains(key)) {
      properties_.blocked = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("Connected");
        changedProperties.contains(key)) {
      properties_.connected = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("LegacyPairing");
        changedProperties.contains(key)) {
      properties_.legacy_pairing = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("Paired");
        changedProperties.contains(key)) {
      properties_.paired = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("Modalias");
        changedProperties.contains(key)) {
      properties_.modalias =
          parse_modalias(changedProperties.at(key).get<std::string>());
    }
    if (const auto key = sdbus::MemberName("Name");
        changedProperties.contains(key)) {
      properties_.name = changedProperties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("ServiceData");
        changedProperties.contains(key)) {
      properties_.service_data =
          changedProperties.at(key)
              .get<std::map<std::string, sdbus::Variant>>();
    }
    if (const auto key = sdbus::MemberName("RSSI");
        changedProperties.contains(key)) {
      properties_.rssi = changedProperties.at(key).get<std::int16_t>();
      spdlog::debug("RSSI: {}", properties_.rssi);
    }
    if (const auto key = sdbus::MemberName("ServicesResolved");
        changedProperties.contains(key)) {
      properties_.services_resolved = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("Trusted");
        changedProperties.contains(key)) {
      properties_.trusted = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("UUIDs");
        changedProperties.contains(key)) {
      properties_.uuids =
          changedProperties.at(key).get<std::vector<std::string>>();
    }
#if 0
    Utils::print_changed_properties(interfaceName, changedProperties,
                                    invalidatedProperties);
#endif
  }
};

#endif  // SRC_BLUEZ_DEVICE1_H
