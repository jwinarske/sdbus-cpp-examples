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

#ifndef SRC_PACKAGEKIT_PACKAGEKIT_CLIENT_H
#define SRC_PACKAGEKIT_PACKAGEKIT_CLIENT_H

#include "../proxy/org/freedesktop/PackageKit/packagekit_proxy.h"

class PackageKitClient final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::PackageKit_proxy> {
 public:
  static constexpr auto INTERFACE_NAME = "org.freedesktop.PackageKit";
  static constexpr auto OBJECT_PATH = "/org/freedesktop/PackageKit";

  explicit PackageKitClient(sdbus::IConnection& connection);

  virtual ~PackageKitClient();

 private:
  sdbus::ObjectPath object_path_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;

  void onTransactionListChanged(
      const std::vector<std::string>& transactions) override;
  void onRestartSchedule() override;
  void onRepoListChanged() override;
  void onUpdatesChanged() override;
};

#endif  // SRC_PACKAGEKIT_PACKAGEKIT_CLIENT_H
