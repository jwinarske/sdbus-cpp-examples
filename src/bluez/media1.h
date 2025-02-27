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

#ifndef SRC_BLUEZ_MEDIA1_H
#define SRC_BLUEZ_MEDIA1_H

#include "../proxy/org/bluez/Media1/media1_proxy.h"

#include "../utils/utils.h"

class Media1 final : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                                   org::bluez::Media1_proxy> {
 public:
  Media1(sdbus::IConnection& connection,
         const sdbus::ServiceName(&destination),
         const sdbus::ObjectPath(&objectPath))
      : ProxyInterfaces{connection, destination, objectPath},
        object_path_(objectPath) {
    registerProxy();
    const auto props = this->GetAllAsync(
        Media1_proxy::INTERFACE_NAME,
        [&](std::optional<sdbus::Error> error,
            std::map<sdbus::PropertyName, sdbus::Variant> values) {
          if (!error)
            onPropertiesChanged(
                sdbus::InterfaceName(Media1_proxy::INTERFACE_NAME), values, {});
          else
            spdlog::error("Media1: {} - {}", error->getName(),
                          error->getMessage());
        });
  }

  virtual ~Media1() { unregisterProxy(); }

 private:
  sdbus::ObjectPath object_path_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    Utils::print_changed_properties(interfaceName, changedProperties,
                                    invalidatedProperties);
  }
};

#endif  // SRC_BLUEZ_MEDIA1_H
