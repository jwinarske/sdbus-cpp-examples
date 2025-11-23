// Copyright (c) 2025 Joel Winarske
// Licensed under the Apache License, Version 2.0
// SPDX-License-Identifier: Apache-2.0

#include "wpa_supplicant1_client.h"

WpaSupplicant1Client::WpaSupplicant1Client(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(SERVICE_NAME),
                      sdbus::ObjectPath(OBJECT_PATH)},
      connection_(connection) {
  // Async initial property fetch -> then enumerate links
  this->GetAllAsync(
      wpa_supplicant1_proxy::INTERFACE_NAME,
      [](std::optional<sdbus::Error> error,
         const std::map<sdbus::PropertyName, sdbus::Variant>& values) {
        if (error) {
          spdlog::warn("wpa_supplicant1 GetAllAsync failed: {} - {}",
                       error->getName(), error->getMessage());
        } else {
          Utils::print_changed_properties(
              sdbus::InterfaceName(wpa_supplicant1_proxy::INTERFACE_NAME),
              values, {});
        }
      });
  registerProxy();
}

WpaSupplicant1Client::~WpaSupplicant1Client() {
  unregisterProxy();
}

void WpaSupplicant1Client::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  if (!changedProperties.empty()) {
    Utils::print_changed_properties(interfaceName, changedProperties,
                                    invalidatedProperties);
  }
}

void WpaSupplicant1Client::onPropertiesChanged(
    const std::map<std::string, sdbus::Variant>& properties) {}

void WpaSupplicant1Client::onInterfaceAdded(
    const sdbus::ObjectPath& path,
    const std::map<std::string, sdbus::Variant>& properties) {
  spdlog::info("onInterfaceAdded");
}

void WpaSupplicant1Client::onInterfaceRemoved(const sdbus::ObjectPath& path) {
  spdlog::info("onInterfaceRemoved");
}
