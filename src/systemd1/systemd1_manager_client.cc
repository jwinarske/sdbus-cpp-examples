// Copyright (c) 2025 Joel Winarske
// Licensed under the Apache License, Version 2.0
// SPDX-License-Identifier: Apache-2.0

#include "systemd1_manager_client.h"

#include <algorithm>

#include "../utils/logging.h"
#include "../utils/resource_limits.h"

Systemd1ManagerClient::Systemd1ManagerClient(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(SERVICE_NAME),
                      sdbus::ObjectPath(OBJECT_PATH)} {
  // Async initial property fetch for consistency
  this->GetAllAsync(
      org::freedesktop::systemd1::Manager_proxy::INTERFACE_NAME,
      [](std::optional<sdbus::Error> error,
         const std::map<sdbus::PropertyName, sdbus::Variant>& values) {
        if (error) {
          LOG_WARN("systemd1.Manager GetAllAsync failed: {} - {}",
                   error->getName(), error->getMessage());
        } else {
          Utils::print_changed_properties(
              sdbus::InterfaceName(Manager_proxy::INTERFACE_NAME), values, {});
        }
      });
  registerProxy();
}

Systemd1ManagerClient::~Systemd1ManagerClient() {
  unregisterProxy();
}

void Systemd1ManagerClient::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  if (!changedProperties.empty()) {
    Utils::print_changed_properties(interfaceName, changedProperties,
                                    invalidatedProperties);
  }
}

void Systemd1ManagerClient::onUnitNew(const std::string& id,
                                      const sdbus::ObjectPath& unit) {
  // systemd emits UnitNew for thousands of units and re-emits them over a long
  // run; de-duplicate and cap growth so activeUnits_ cannot grow unbounded.
  if (std::ranges::find(activeUnits_, id) == activeUnits_.end()) {
    if (resource_limits::IsAtCapacity(activeUnits_.size(),
                                      resource_limits::kMaxUnits)) {
      LOG_WARN("[systemd1] Skipping UnitNew id={}: resource limit reached ({})",
               id, resource_limits::kMaxUnits);
    } else {
      activeUnits_.push_back(id);
    }
  }
  LOG_INFO("[systemd1] UnitNew id={} path={}", id,
           static_cast<std::string>(unit));
}

void Systemd1ManagerClient::onUnitRemoved(const std::string& id,
                                          const sdbus::ObjectPath& unit) {
  activeUnits_.erase(std::ranges::remove(activeUnits_, id).begin(),
                     activeUnits_.end());
  LOG_INFO("[systemd1] UnitRemoved id={} path={}", id,
           static_cast<std::string>(unit));
}

void Systemd1ManagerClient::onJobNew(const uint32_t& id,
                                     const sdbus::ObjectPath& job,
                                     const std::string& unit) {
  LOG_INFO("[systemd1] JobNew job_id={} job_path={} unit={}", id,
           static_cast<std::string>(job), unit);
}

void Systemd1ManagerClient::onJobRemoved(const uint32_t& id,
                                         const sdbus::ObjectPath& job,
                                         const std::string& unit,
                                         const std::string& result) {
  LOG_INFO("[systemd1] JobRemoved job_id={} job_path={} unit={} result={}", id,
           static_cast<std::string>(job), unit, result);
}

void Systemd1ManagerClient::onStartupFinished(const uint64_t& firmware,
                                              const uint64_t& loader,
                                              const uint64_t& kernel,
                                              const uint64_t& initrd,
                                              const uint64_t& userspace,
                                              const uint64_t& total) {
  LOG_INFO(
      "[systemd1] StartupFinished firmware={} loader={} kernel={} initrd={} "
      "userspace={} total={}",
      firmware, loader, kernel, initrd, userspace, total);
}

void Systemd1ManagerClient::onUnitFilesChanged() {
  LOG_INFO("[systemd1] UnitFilesChanged");
}

void Systemd1ManagerClient::onReloading(const bool& active) {
  LOG_INFO("[systemd1] Reloading active={}", active);
}