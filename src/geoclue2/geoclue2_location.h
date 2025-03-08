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

#ifndef SRC_GEOCLUE2_GEOCLUE2_LOCATION_H
#define SRC_GEOCLUE2_GEOCLUE2_LOCATION_H

#include "../proxy/org/freedesktop/GeoClue2/Client/Location/location_proxy.h"

#include "../utils/utils.h"

class GeoClue2Location final : public sdbus::ProxyInterfaces<
                                   sdbus::Properties_proxy,
                                   org::freedesktop::GeoClue2::Location_proxy> {
 public:
  explicit GeoClue2Location(sdbus::IConnection& connection,
                            const sdbus::ObjectPath& objectPath)
      : ProxyInterfaces{connection, sdbus::ServiceName(kBusName), objectPath} {
    registerProxy();
    const auto properties = this->GetAll("org.freedesktop.GeoClue2.Location");
    GeoClue2Location::onPropertiesChanged(sdbus::InterfaceName(kBusName),
                                          properties, {});
  }

  virtual ~GeoClue2Location() { unregisterProxy(); }

 private:
  static constexpr auto kBusName = "org.freedesktop.GeoClue2";

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    std::ostringstream os;
    os << std::endl;
    os << "[" << interfaceName << "] GeoClue2Location Properties changed"
       << std::endl;
    Utils::append_properties(changedProperties, os);
    for (const auto& name : invalidatedProperties) {
      os << "[" << interfaceName << "] Invalidated property: " << name
         << std::endl;
    }
    spdlog::info(os.str());
  }
};

#endif  // SRC_GEOCLUE2_GEOCLUE2_LOCATION_H
