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
#include "../utils/logging.h"

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
    LOG_DEBUG("Device1: {}", objectPath);
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
    LOG_ERROR("Failed to parse modalias");
    return {};
  }

 private:
  sdbus::ObjectPath adapter_;

  Properties properties_{};

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    // Iterate the changed properties once and dispatch on the key. A
    // PropertiesChanged signal usually carries only a handful of properties,
    // so this avoids scanning every known property (and the per-property map
    // lookup + MemberName allocation) on every signal.
    for (const auto& [key, value] : changedProperties) {
      if (key == "Adapter") {
        properties_.adapter = value.get<sdbus::ObjectPath>();
      } else if (key == "Address") {
        properties_.address = value.get<std::string>();
      } else if (key == "AddressType") {
        properties_.address_type = value.get<std::string>();
      } else if (key == "Bonded") {
        properties_.bonded = value.get<bool>();
      } else if (key == "Blocked") {
        properties_.blocked = value.get<bool>();
      } else if (key == "Connected") {
        properties_.connected = value.get<bool>();
      } else if (key == "LegacyPairing") {
        properties_.legacy_pairing = value.get<bool>();
      } else if (key == "Paired") {
        properties_.paired = value.get<bool>();
      } else if (key == "Modalias") {
        properties_.modalias = parse_modalias(value.get<std::string>());
      } else if (key == "Name") {
        properties_.name = value.get<std::string>();
      } else if (key == "ServiceData") {
        properties_.service_data =
            value.get<std::map<std::string, sdbus::Variant>>();
      } else if (key == "RSSI") {
        properties_.rssi = value.get<std::int16_t>();
        LOG_DEBUG("RSSI: {}", properties_.rssi);
      } else if (key == "ServicesResolved") {
        properties_.services_resolved = value.get<bool>();
      } else if (key == "Trusted") {
        properties_.trusted = value.get<bool>();
      } else if (key == "UUIDs") {
        properties_.uuids = value.get<std::vector<std::string>>();
      }
    }
#if 0
    Utils::print_changed_properties(interfaceName, changedProperties,
                                    invalidatedProperties);
#endif
  }
};

#endif  // SRC_BLUEZ_DEVICE1_H
