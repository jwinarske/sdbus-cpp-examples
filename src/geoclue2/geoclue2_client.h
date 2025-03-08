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

#ifndef SRC_GEOCLUE2_GEOCLUE2_CLIENT_H
#define SRC_GEOCLUE2_GEOCLUE2_CLIENT_H

#include "../proxy/org/freedesktop/GeoClue2/Client/client_proxy.h"
#include "geoclue2_location.h"

#include "../utils/utils.h"

class GeoClue2Client final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::GeoClue2::Client_proxy> {
 public:
  explicit GeoClue2Client(sdbus::IConnection& connection,
                          const sdbus::ObjectPath& objectPath)
      : ProxyInterfaces{connection, sdbus::ServiceName(INTERFACE_NAME),
                        objectPath} {
    registerProxy();
    const auto properties = this->GetAll("org.freedesktop.GeoClue2.Client");
    GeoClue2Client::onPropertiesChanged(sdbus::InterfaceName(INTERFACE_NAME),
                                        properties, {});
  }

  virtual ~GeoClue2Client() { unregisterProxy(); }

  [[nodiscard]] auto getLocation() const { return location_; }

 private:
  static constexpr auto INTERFACE_NAME = "org.freedesktop.GeoClue2";

  std::shared_ptr<GeoClue2Location> location_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    std::ostringstream os;
    os << std::endl;
    os << "[" << interfaceName << "] GeoClue2Client Properties changed"
       << std::endl;
    Utils::append_properties(changedProperties, os);
    for (const auto& name : invalidatedProperties) {
      os << "[" << interfaceName << "] Invalidated property: " << name
         << std::endl;
    }
    spdlog::info(os.str());
  }

  void onLocationUpdated(const sdbus::ObjectPath& old,
                         const sdbus::ObjectPath& new_) override {
    spdlog::info("GeoClue2Client onLocationUpdated: {}, {}", old, new_);
    location_ =
        std::make_unique<GeoClue2Location>(getProxy().getConnection(), new_);
  }
};

#endif  // SRC_GEOCLUE2_GEOCLUE2_CLIENT_H
