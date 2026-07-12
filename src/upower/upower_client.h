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

#ifndef SRC_UPOWER_UPOWER_CLIENT_H
#define SRC_UPOWER_UPOWER_CLIENT_H

#include <utility>

#include "../proxy/org/freedesktop/UPower/upower_proxy.h"
#include "../utils/logging.h"
#include "../utils/utils.h"
#include "upower_display_device.h"

class UPowerDisplayDevice;

class UPowerClient final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::UPower_proxy> {
 public:
  explicit UPowerClient(sdbus::IConnection& connection,
                        std::string device_filter = {})
      : ProxyInterfaces{connection,
                        sdbus::ServiceName(UPower_proxy::INTERFACE_NAME),
                        sdbus::ObjectPath("/org/freedesktop/UPower")},
        device_filter_(std::move(device_filter)) {
    registerProxy();
    // Fetch properties and enumerate devices asynchronously so construction
    // never blocks the caller's event loop on a D-Bus round-trip. The replies
    // are delivered on whichever loop drives this connection; pending calls are
    // cancelled by unregisterProxy() if this object is destroyed first.
    GetAllAsync(
        sdbus::InterfaceName(UPower_proxy::INTERFACE_NAME),
        // sdbus requires the error argument by value (function_traits).
        // NOLINTNEXTLINE(performance-unnecessary-value-param)
        [this](
            std::optional<sdbus::Error> error,
            const std::map<sdbus::PropertyName, sdbus::Variant>& properties) {
          if (error) {
            LOG_ERROR("UPower GetAll failed: {} - {}", error->getName(),
                      error->getMessage());
            return;
          }
          onPropertiesChanged(
              sdbus::InterfaceName(UPower_proxy::INTERFACE_NAME), properties,
              {});
        });
    getProxy()
        .callMethodAsync("EnumerateDevices")
        .onInterface(UPower_proxy::INTERFACE_NAME)
        // sdbus requires the error argument by value (function_traits).
        // NOLINTNEXTLINE(performance-unnecessary-value-param)
        .uponReplyInvoke([this](std::optional<sdbus::Error> error,
                                const std::vector<sdbus::ObjectPath>& devices) {
          if (error) {
            LOG_ERROR("UPower EnumerateDevices failed: {} - {}",
                      error->getName(), error->getMessage());
            return;
          }
          for (const auto& device : devices) {
            onDeviceAdded(device);
          }
        });
  }

  virtual ~UPowerClient() {
    std::lock_guard lock(devices_mutex_);
    for (auto& [_, display_device] : devices_) {
      display_device.reset();
    }
    unregisterProxy();
  }

 private:
  std::string device_filter_;
  std::mutex devices_mutex_;
  std::map<sdbus::ObjectPath, std::shared_ptr<UPowerDisplayDevice>> devices_;

  void onDeviceAdded(const sdbus::ObjectPath& device) override {
    if (device_filter_.empty() || device_filter_ == device) {
      LOG_INFO("onDeviceAdded: {}", device);
      std::lock_guard lock(devices_mutex_);
      if (!devices_.contains(device)) {
        devices_[device] = std::make_shared<UPowerDisplayDevice>(
            getProxy().getConnection(), device);
      }
    }
  }

  void onDeviceRemoved(const sdbus::ObjectPath& device) override {
    if (device_filter_.empty() || device_filter_ == device) {
      LOG_INFO("onDeviceRemoved: {}", device);
      std::lock_guard lock(devices_mutex_);
      if (devices_.contains(device)) {
        devices_[device].reset();
        devices_.erase(device);
      }
    }
  }

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    LOG_INFO("onPropertiesChanged: {}", interfaceName);
    Utils::print_changed_properties(interfaceName, changedProperties,
                                    invalidatedProperties);
  }
};

#endif  // SRC_UPOWER_UPOWER_CLIENT_H
