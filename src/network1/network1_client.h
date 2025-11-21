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

#ifndef SRC_NETWORK1_NETWORK1_CLIENT_H
#define SRC_NETWORK1_NETWORK1_CLIENT_H

#include "../proxy/org/freedesktop/network1/Link/link_proxy.h"
#include "../proxy/org/freedesktop/network1/Manager/manager_proxy.h"
#include "../utils/utils.h"

class Network1ManagerClient final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::network1::Manager_proxy> {
 public:
  static constexpr auto SERVICE_NAME = "org.freedesktop.network1";
  static constexpr auto OBJECT_PATH = "/org/freedesktop/network1";

  explicit Network1ManagerClient(sdbus::IConnection& connection);
  ~Network1ManagerClient() override;

  struct LinkInfo {
    int32_t ifindex{};
    std::string name;
    sdbus::ObjectPath path;
  };
  [[nodiscard]] const std::vector<LinkInfo>& links() const { return links_; }

 private:
  sdbus::IConnection& connection_;
  std::vector<LinkInfo> links_;

  void enumerateLinks();

  // Implement Properties_proxy pure virtual to avoid abstract type
  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;
};

#endif  // SRC_NETWORK1_NETWORK1_CLIENT_H