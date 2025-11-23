// Copyright (c) 2025 Joel Winarske
// Licensed under the Apache License, Version 2.0
// SPDX-License-Identifier: Apache-2.0

#include "network1_client.h"

Network1ManagerClient::Network1ManagerClient(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(SERVICE_NAME),
                      sdbus::ObjectPath(OBJECT_PATH)},
      connection_(connection) {
  // Async initial property fetch -> then enumerate links
  this->GetAllAsync(
      Manager_proxy::INTERFACE_NAME,
      [this](std::optional<sdbus::Error> error,
             const std::map<sdbus::PropertyName, sdbus::Variant>& values) {
        if (error) {
          spdlog::warn("network1.Manager GetAllAsync failed: {} - {}",
                       error->getName(), error->getMessage());
        } else {
          Utils::print_changed_properties(
              sdbus::InterfaceName(Manager_proxy::INTERFACE_NAME), values, {});
          enumerateLinks();
        }
      });
  registerProxy();
}

Network1ManagerClient::~Network1ManagerClient() {
  unregisterProxy();
}

void Network1ManagerClient::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  if (!changedProperties.empty()) {
    Utils::print_changed_properties(interfaceName, changedProperties,
                                    invalidatedProperties);
  }
}

void Network1ManagerClient::enumerateLinks() {
  links_.clear();
  try {
    for (auto entries = ListLinks(); auto& entry : entries) {
      LinkInfo info{entry.get<0>(), entry.get<1>(), entry.get<2>()};
      links_.push_back(info);
    }
    spdlog::info("[network1] Found {} link(s)", links_.size());
    for (const auto& [ifindex, name, path] : links_) {
      spdlog::info("  ifindex={} name={} path={}", ifindex, name,
                   static_cast<std::string>(path));
      const auto linkProxy = sdbus::createProxy(
          connection_, sdbus::ServiceName(SERVICE_NAME), path);
      std::string description;
      linkProxy->callMethod("Describe")
          .onInterface("org.freedesktop.network1.Link")
          .storeResultsTo(description);

      // Parse JSON description using simd-json and log a compact summary
      std::string parsedSummary = Utils::parseDescriptionJson(description);
      spdlog::info("\n{}", parsedSummary);

      auto props = linkProxy->getAllProperties().onInterface(
          "org.freedesktop.network1.Link");
      Utils::print_changed_properties(sdbus::InterfaceName(path), props, {});
      linkProxy->unregister();
    }
  } catch (const sdbus::Error& e) {
    spdlog::error("ListLinks failed: {} - {}", e.getName(), e.getMessage());
  }
}