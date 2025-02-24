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

#ifndef SRC_NETWORKMANAGER_NETWORKMANAGER_CLIENT_H
#define SRC_NETWORKMANAGER_NETWORKMANAGER_CLIENT_H

#include "../proxy/org/freedesktop/NetworkManager/network_manager_proxy.h"

class NetworkManagerClient
    : public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy,
                                    org::freedesktop::NetworkManager_proxy> {
 public:
  explicit NetworkManagerClient(sdbus::IConnection& connection);

  ~NetworkManagerClient();

 private:
  static constexpr auto kBusName = "org.freedesktop.NetworkManager";
  static constexpr auto kObjectPath = "/org/freedesktop";

  sdbus::IConnection& connection_;
  sdbus::ObjectPath object_path_;

  void onInterfacesAdded(
      const sdbus::ObjectPath& objectPath,
      const std::map<sdbus::InterfaceName,
                     std::map<sdbus::PropertyName, sdbus::Variant>>&
          interfacesAndProperties) override;

  void onInterfacesRemoved(
      const sdbus::ObjectPath& objectPath,
      const std::vector<sdbus::InterfaceName>& interfaces) override;

  void onPropertiesChanged(
      const std::map<std::string, sdbus::Variant>& properties) override;
  void onCheckPermissions() override;
  void onStateChanged(const uint32_t& state) override;
  void onDeviceAdded(const sdbus::ObjectPath& device_path) override;
  void onDeviceRemoved(const sdbus::ObjectPath& device_path) override;
};

#endif  // SRC_NETWORKMANAGER_NETWORKMANAGER_CLIENT_H
