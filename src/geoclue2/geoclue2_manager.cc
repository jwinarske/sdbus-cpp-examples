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

#include "geoclue2_manager.h"

#include "../utils/utils.h"

GeoClue2Manager::GeoClue2Manager(
    sdbus::IConnection& connection,
    const LocationPropertiesChangedCallback& callback)
    : ProxyInterfaces{connection, sdbus::ServiceName(kBusName),
                      sdbus::ObjectPath(kObjectPath)},
      object_path_(sdbus::ObjectPath(kObjectPath)) {
  registerProxy();
  const auto properties = this->GetAll("org.freedesktop.GeoClue2.Manager");
  GeoClue2Manager::onPropertiesChanged(sdbus::InterfaceName(kBusName),
                                       properties, {});
  if (auto client_path = this->GetClient(); !client_path.empty()) {
    client_ =
        std::make_shared<GeoClue2Client>(connection, client_path, callback);
  }
}

GeoClue2Manager::~GeoClue2Manager() {
  client_.reset();
  unregisterProxy();
}

void GeoClue2Manager::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  for (const auto& [key, value] : changedProperties) {
    if (key == "InUse") {
      properties_.in_use = value.get<bool>();
    } else if (key == "AvailableAccuracyLevel") {
      properties_.available_accuracy_level = value.get<std::uint32_t>();
    } else {
      Utils::print_changed_properties(interfaceName, changedProperties,
                                      invalidatedProperties);
    }
  }
}
