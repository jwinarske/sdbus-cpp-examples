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

#ifndef SRC_GEOCLUE2_GEOCLUE2_MANAGER_H
#define SRC_GEOCLUE2_GEOCLUE2_MANAGER_H

#include "../proxy/GeoClue2Manager_proxy.h"
#include "geoclue2_client.h"
#include "geoclue2_location.h"

class GeoClue2Manager final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::GeoClue2::Manager_proxy> {
 public:
  explicit GeoClue2Manager(sdbus::IConnection& connection);

  ~GeoClue2Manager();

  [[nodiscard]] auto getClient() const { return client_; }

 private:
  static constexpr auto kBusName = "org.freedesktop.GeoClue2";
  static constexpr auto kObjectPath = "/org/freedesktop/GeoClue2/Manager";

  sdbus::ObjectPath object_path_;

  std::shared_ptr<GeoClue2Client> client_;
  std::map<sdbus::ObjectPath, std::unique_ptr<GeoClue2Location>> locations_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;
};

#endif  // SRC_GEOCLUE2_GEOCLUE2_MANAGER_H
