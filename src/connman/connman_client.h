// Copyright (c) 2026 Jian De
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

#ifndef SRC_CONNMAN_CONNMAN_CLIENT_H
#define SRC_CONNMAN_CONNMAN_CLIENT_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../proxy/net/connman/Manager/manager_proxy.h"
#include "../proxy/net/connman/Service/service_proxy.h"
#include "../proxy/net/connman/Technology/technology_proxy.h"

class ConnmanTechnologyClient;
class ConnmanServiceClient;

class ConnmanManagerClient final
    : public sdbus::ProxyInterfaces<net::connman::Manager_proxy> {
 public:
  static constexpr auto SERVICE_NAME = "net.connman";
  static constexpr auto OBJECT_PATH = "/";

  explicit ConnmanManagerClient(sdbus::IConnection& connection);
  ~ConnmanManagerClient();

  void onPropertyChanged(const std::string& name,
                         const sdbus::Variant& value) override;

  void onTechnologyAdded(
      const sdbus::ObjectPath& path,
      const std::map<std::string, sdbus::Variant>& properties) override;

  void onTechnologyRemoved(const sdbus::ObjectPath& path) override;

  void onServicesChanged(
      const std::vector<sdbus::Struct<sdbus::ObjectPath,
                                      std::map<std::string, sdbus::Variant>>>&
          changed,
      const std::vector<sdbus::ObjectPath>& removed) override;

  void onPeersChanged(
      const std::vector<sdbus::Struct<sdbus::ObjectPath,
                                      std::map<std::string, sdbus::Variant>>>&
          changed,
      const std::vector<sdbus::ObjectPath>& removed) override;

  void onTetheringClientsChanged(
      const std::vector<std::string>& registered,
      const std::vector<std::string>& removed) override;

 private:
  std::map<sdbus::ObjectPath, std::unique_ptr<ConnmanTechnologyClient>>
      technologies_;
  std::map<sdbus::ObjectPath, std::unique_ptr<ConnmanServiceClient>> services_;
};

class ConnmanTechnologyClient final
    : public sdbus::ProxyInterfaces<net::connman::Technology_proxy> {
 public:
  static constexpr auto SERVICE_NAME = "net.connman";

  ConnmanTechnologyClient(sdbus::IConnection& connection,
                          sdbus::ObjectPath path);
  ~ConnmanTechnologyClient();

  void onPropertyChanged(const std::string& name,
                         const sdbus::Variant& value) override;
};

class ConnmanServiceClient final
    : public sdbus::ProxyInterfaces<net::connman::Service_proxy> {
 public:
  static constexpr auto SERVICE_NAME = "net.connman";

  ConnmanServiceClient(sdbus::IConnection& connection, sdbus::ObjectPath path);
  ~ConnmanServiceClient();

  void onPropertyChanged(const std::string& name,
                         const sdbus::Variant& value) override;
};

#endif  // SRC_CONNMAN_CONNMAN_CLIENT_H