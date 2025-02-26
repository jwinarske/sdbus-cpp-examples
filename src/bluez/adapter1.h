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

#include "../utils.h"

class Adapter1 final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::bluez::Adapter1_proxy> {
 public:
  Adapter1(sdbus::IConnection& connection,
           const sdbus::ServiceName(&destination),
           const sdbus::ObjectPath(&objectPath))
      : ProxyInterfaces{connection, destination, objectPath},
        object_path_(objectPath) {
    registerProxy();
  }

  virtual ~Adapter1() { unregisterProxy(); }

 private:
  sdbus::ObjectPath object_path_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    std::stringstream ss;
    ss << std::endl;
    ss << "[" << object_path_ << "] Properties changed" << std::endl;
    Utils::append_properties(changedProperties, ss);
    for (const auto& name : invalidatedProperties) {
      ss << "[" << object_path_ << "] Invalidated property: " << name
         << std::endl;
    }
    spdlog::info("{}", ss.str());
  }
};

#endif  // SRC_BLUEZ_ADAPTER1_H
