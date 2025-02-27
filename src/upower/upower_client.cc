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

#include "upower_client.h"

#include "upower_display_device.h"

#include "../utils.h"

void UPowerClient::onDeviceAdded(const sdbus::ObjectPath& device) {
  spdlog::info("onDeviceAdded: {}", device);
  std::lock_guard lock(devices_mutex_);
  if (!devices_.contains(device)) {
    devices_[device] = std::make_shared<UPowerDisplayDevice>(
        getProxy().getConnection(), device);
  }
}

void UPowerClient::onDeviceRemoved(const sdbus::ObjectPath& device) {
  spdlog::info("onDeviceRemoved: {}", device);
  std::lock_guard lock(devices_mutex_);
  if (devices_.contains(device)) {
    devices_[device].reset();
    devices_.erase(device);
  }
}

void UPowerClient::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  Utils::print_changed_properties(interfaceName, changedProperties,
                                  invalidatedProperties);
}
