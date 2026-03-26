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

#include "flatpak_system_helper_client.h"

#include "../utils/logging.h"

FlatpakSystemHelperClient::FlatpakSystemHelperClient(
    sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(SERVICE_NAME),
                      sdbus::ObjectPath(OBJECT_PATH)} {
  registerProxy();

  try {
    version_ = version();
  } catch (const sdbus::Error& e) {
    LOG_WARN("Failed to read version property: {} - {}", e.getName(),
             e.getMessage());
  }
}

FlatpakSystemHelperClient::~FlatpakSystemHelperClient() {
  unregisterProxy();
}

void FlatpakSystemHelperClient::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  for (const auto& [key, value] : changedProperties) {
    if (key == "version") {
      version_ = value.get<uint32_t>();
      LOG_INFO("version changed: {}", version_);
    }
  }
}

void FlatpakSystemHelperClient::printInfo() const {
  LOG_INFO("Flatpak SystemHelper:");
  LOG_INFO("\tversion: {}", version_);
}
