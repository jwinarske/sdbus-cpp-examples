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
                        sdbus::ServiceName("org.freedesktop.UPower"),
                        sdbus::ObjectPath("/org/freedesktop/UPower")},
        device_filter_(std::move(device_filter)) {
    registerProxy();
    const auto properties = this->GetAll("org.freedesktop.UPower");
    UPowerClient::onPropertiesChanged(
        sdbus::InterfaceName("org.freedesktop.UPower"), properties, {});
    for (const auto devices = EnumerateDevices();
         const auto& device : devices) {
      UPowerClient::onDeviceAdded(device);
    }
  }

  virtual ~UPowerClient() {
    std::lock_guard lock(devices_mutex_);
    for (auto& [_, display_device] : devices_) {
      display_device.reset();
    }
    unregisterProxy();
  }

 private:
  std::unique_ptr<UPowerDisplayDevice> display_device_;
  std::string device_filter_;

  std::mutex devices_mutex_;
  std::map<sdbus::ObjectPath, std::shared_ptr<UPowerDisplayDevice>> devices_;

  void onDeviceAdded(const sdbus::ObjectPath& device) override {
    if (device_filter_.empty() ||
        !device_filter_.empty() && device_filter_ == device) {
      spdlog::info("onDeviceAdded: {}", device);
      std::lock_guard lock(devices_mutex_);
      if (!devices_.contains(device)) {
        devices_[device] = std::make_shared<UPowerDisplayDevice>(
            getProxy().getConnection(), device);
      }
    }
  }

  void onDeviceRemoved(const sdbus::ObjectPath& device) override {
    if (device_filter_.empty() ||
        !device_filter_.empty() && device_filter_ == device) {
      spdlog::info("onDeviceRemoved: {}", device);
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
    spdlog::info("onPropertiesChanged: {}", interfaceName);
    Utils::print_changed_properties(interfaceName, changedProperties,
                                    invalidatedProperties);
  }
};

#endif  // SRC_UPOWER_UPOWER_CLIENT_H
