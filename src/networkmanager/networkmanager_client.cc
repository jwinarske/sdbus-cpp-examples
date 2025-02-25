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

#include "networkmanager_client.h"

#include "../utils.h"

NetworkManagerClient::NetworkManagerClient(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(kBusName),
                      sdbus::ObjectPath(kObjectPath)},
      connection_(connection),
      object_path_(sdbus::ObjectPath(kObjectPath)) {
  registerProxy();
  for (const auto& [object, interfacesAndProperties] : GetManagedObjects()) {
    onInterfacesAdded(object, interfacesAndProperties);
  }
}

NetworkManagerClient::~NetworkManagerClient() {
  unregisterProxy();
}

void NetworkManagerClient::onInterfacesAdded(
    const sdbus::ObjectPath& objectPath,
    const std::map<sdbus::InterfaceName,
                   std::map<sdbus::PropertyName, sdbus::Variant>>&
        interfacesAndProperties) {
  std::stringstream ss;
  ss << std::endl;
  for (const auto& [interface, properties] : interfacesAndProperties) {
    ss << "[" << objectPath << "] Add - " << interface << std::endl;
    Utils::append_properties(properties, ss);
  }
  spdlog::info("{}", ss.str());
}

void NetworkManagerClient::onInterfacesRemoved(
    const sdbus::ObjectPath& objectPath,
    const std::vector<sdbus::InterfaceName>& interfaces) {
  std::stringstream ss;
  ss << std::endl;
  for (auto it = interfaces.begin(); it != interfaces.end(); ++it) {
    ss << "[" << objectPath << "] Remove - " << *it;
    if (std::next(it) != interfaces.end()) {
      ss << std::endl;
    }
  }
  spdlog::info("{}", ss.str());
}

void NetworkManagerClient::onPropertiesChanged(
    const std::map<std::string, sdbus::Variant>& properties) {
  std::stringstream ss;
  ss << std::endl;
  ss << "NetworkManagerClient Properties changed" << std::endl;
  Utils::append_properties(properties, ss);
  spdlog::info("{}", ss.str());
}

void NetworkManagerClient::onCheckPermissions() {
  spdlog::info("NetworkManagerClient::onCheckPermissions()");
}

void NetworkManagerClient::onStateChanged(const uint32_t& state) {
  spdlog::info("NetworkManagerClient::onStateChanged: {}", state);
}

void NetworkManagerClient::onDeviceAdded(const sdbus::ObjectPath& device_path) {
  spdlog::info("NetworkManagerClient::onDeviceAdded: {}", device_path);
}

void NetworkManagerClient::onDeviceRemoved(
    const sdbus::ObjectPath& device_path) {
  spdlog::info("NetworkManagerClient::onDeviceRemoved: {}", device_path);
}
