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

#ifndef SRC_WPA_SUPPLICANT_CLIENT_H
#define SRC_WPA_SUPPLICANT_CLIENT_H

#include "../proxy/fi/w1/wpa_supplicant1/wpa_supplicant1_proxy.h"
#include "../utils/utils.h"

class WpaSupplicant1Client final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    fi::w1::wpa_supplicant1_proxy> {
 public:
  static constexpr auto SERVICE_NAME = "fi.w1.wpa_supplicant1";
  static constexpr auto OBJECT_PATH = "/fi/w1/wpa_supplicant1";

  explicit WpaSupplicant1Client(sdbus::IConnection& connection);
  ~WpaSupplicant1Client();

 private:
  sdbus::IConnection& connection_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;
  void onPropertiesChanged(
      const std::map<std::string, sdbus::Variant>& properties) override;
  void onInterfaceAdded(
      const sdbus::ObjectPath& path,
      const std::map<std::string, sdbus::Variant>& properties) override;
  void onInterfaceRemoved(const sdbus::ObjectPath& path) override;
};

#endif  // SRC_WPA_SUPPLICANT_CLIENT_H