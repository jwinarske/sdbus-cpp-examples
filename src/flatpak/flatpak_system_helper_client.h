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

#ifndef SRC_FLATPAK_FLATPAK_SYSTEM_HELPER_CLIENT_H
#define SRC_FLATPAK_FLATPAK_SYSTEM_HELPER_CLIENT_H

#include "../proxy/org/freedesktop/Flatpak/SystemHelper/system_helper_proxy.h"

class FlatpakSystemHelperClient final
    : public sdbus::ProxyInterfaces<
          sdbus::Properties_proxy,
          org::freedesktop::Flatpak::SystemHelper_proxy> {
 public:
  static constexpr auto SERVICE_NAME = "org.freedesktop.Flatpak.SystemHelper";
  static constexpr auto OBJECT_PATH = "/org/freedesktop/Flatpak/SystemHelper";

  explicit FlatpakSystemHelperClient(sdbus::IConnection& connection);

  ~FlatpakSystemHelperClient();

  void printInfo() const;

 private:
  uint32_t version_{};

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;
};

#endif  // SRC_FLATPAK_FLATPAK_SYSTEM_HELPER_CLIENT_H
