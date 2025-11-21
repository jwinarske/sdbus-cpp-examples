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

#ifndef SRC_FWUPD_FWUPD_CLIENT_H
#define SRC_FWUPD_FWUPD_CLIENT_H

#include "../proxy/org/freedesktop/fwupd/fwupd_proxy.h"

class FwupdClient final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::fwupd_proxy> {
 public:
  static constexpr auto INTERFACE_NAME = "org.freedesktop.fwupd";
  static constexpr auto OBJECT_PATH = "/";

  explicit FwupdClient(sdbus::IConnection& connection);

  virtual ~FwupdClient();

 private:
  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;

  void onChanged() override;
  void onDeviceAdded(const std::map<std::string, sdbus::Variant>& device) override;
  void onDeviceRemoved(const std::map<std::string, sdbus::Variant>& device) override;
  void onDeviceChanged(const std::map<std::string, sdbus::Variant>& device) override;
  void onDeviceRequest(const std::map<std::string, sdbus::Variant>& request) override;
};

#endif  // SRC_FWUPD_FWUPD_CLIENT_H
