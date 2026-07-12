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
  struct Properties {
    std::string address;
    std::string address_type;
    std::string alias;
    std::uint32_t class_type;
    bool discoverable;
    std::uint32_t discoverable_timeout;
    bool discovering;
    std::string modalias;
    std::string name;
    bool pairable;
    std::uint32_t pairable_timeout;
    bool powered;
    std::vector<std::string> uuids;
  };

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

  [[nodiscard]] const Properties& GetProperties() const { return properties_; }

 private:
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
      if (key == "Address") {
        properties_.address = value.get<std::string>();
      } else if (key == "AddressType") {
        properties_.address_type = value.get<std::string>();
      } else if (key == "Alias") {
        properties_.alias = value.get<std::string>();
      } else if (key == "Class") {
        properties_.class_type = value.get<std::uint32_t>();
      } else if (key == "Discoverable") {
        properties_.discoverable = value.get<bool>();
      } else if (key == "DiscoverableTimeout") {
        properties_.discoverable_timeout = value.get<std::uint32_t>();
      } else if (key == "Discovering") {
        properties_.discovering = value.get<bool>();

        if (!properties_.discovering) {
          this->StartDiscovery();
        }
      } else if (key == "Modalias") {
        properties_.modalias = value.get<std::string>();
      } else if (key == "Name") {
        properties_.name = value.get<std::string>();
      } else if (key == "Pairable") {
        properties_.pairable = value.get<bool>();
      } else if (key == "PairableTimeout") {
        properties_.pairable_timeout = value.get<std::uint32_t>();
      } else if (key == "Powered") {
        properties_.powered = value.get<bool>();
      } else if (key == "UUIDs") {
        properties_.uuids = value.get<std::vector<std::string>>();
      }
    }
  }
};

#endif  // SRC_BLUEZ_ADAPTER1_H
