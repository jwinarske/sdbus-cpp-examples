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

#include "network1_client.h"

Network1ManagerClient::Network1ManagerClient(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(SERVICE_NAME), sdbus::ObjectPath(OBJECT_PATH)},
      connection_(connection) {
  this->GetAllAsync(
      org::freedesktop::network1::Manager_proxy::INTERFACE_NAME,
      [this](std::optional<sdbus::Error> error,
             std::map<sdbus::PropertyName, sdbus::Variant> values) {
        if (error) {
          spdlog::warn("network1.Manager GetAllAsync failed: {} - {}",
                       error->getName(), error->getMessage());
        } else {
          Utils::print_changed_properties(
              sdbus::InterfaceName(org::freedesktop::network1::Manager_proxy::INTERFACE_NAME),
              values, {});
          enumerateLinks();
        }
      });
  registerProxy();
}

Network1ManagerClient::~Network1ManagerClient() { unregisterProxy(); }

void Network1ManagerClient::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  if (!changedProperties.empty()) {
    Utils::print_changed_properties(interfaceName, changedProperties, invalidatedProperties);
    // Re-enumerate if relevant properties changed (future extension)
  }
}

void Network1ManagerClient::enumerateLinks() {
  links_.clear();
  try {
    auto entries = ListLinks();
    for (auto& entry : entries) {
      LinkInfo info{entry.get<0>(), entry.get<1>(), entry.get<2>()};
      links_.push_back(info);
    }
    spdlog::info("[network1] Found {} link(s)", links_.size());
    for (const auto& l : links_) {
      spdlog::info("  ifindex={} name={} path={}", l.ifindex, l.name, static_cast<std::string>(l.path));
      auto linkProxy = sdbus::createProxy(connection_, sdbus::ServiceName(SERVICE_NAME), l.path);
      org::freedesktop::network1::Link_proxy linkIface(*linkProxy);
      spdlog::info("    Link proxy instantiated");
    }
  } catch (const sdbus::Error& e) {
    spdlog::error("ListLinks failed: {} - {}", e.getName(), e.getMessage());
  }
}
