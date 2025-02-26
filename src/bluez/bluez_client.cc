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

#include "bluez_client.h"

#include "battery_provider_manager1.h"
#include "gatt_manager1.h"
#include "le_advertising_manager1.h"
#include "media1.h"
#include "network_server1.h"

BluezClient::BluezClient(sdbus::IConnection& connection)
    : ProxyInterfaces(connection,
                      sdbus::ServiceName(INTERFACE_NAME),
                      sdbus::ObjectPath("/")),
      connection_(connection) {
  registerProxy();
  for (const auto& [object, interfacesAndProperties] : GetManagedObjects()) {
    onInterfacesAdded(object, interfacesAndProperties);
  }
}

BluezClient::~BluezClient() {
  unregisterProxy();
}

void BluezClient::onInterfacesAdded(
    const sdbus::ObjectPath& objectPath,
    const std::map<sdbus::InterfaceName,
                   std::map<sdbus::PropertyName, sdbus::Variant>>&
        interfacesAndProperties) {
  std::stringstream ss;
  ss << std::endl;
  for (const auto& [interface, properties] : interfacesAndProperties) {
    ss << "[" << objectPath << "] Add - " << interface << std::endl;
    Utils::append_properties(properties, ss);
    if (interface == org::bluez::Adapter1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(adapter_mutex_);
      if (!adapters_.contains(objectPath)) {
        std::unique_ptr<Adapter1, Adapter1::Adapter1Deleter> adapter1(
            new Adapter1(connection_, sdbus::ServiceName(INTERFACE_NAME),
                         objectPath));
        adapter1->Alias("bluez_ble_client");
        if (!adapter1->Discovering()) {
          adapter1->StartDiscovery();
        }
        for (auto filters = adapter1->GetDiscoveryFilters();
             const auto& filter : filters) {
          spdlog::info("Discovery Filter: {}", filter);
        }
        adapters_[objectPath] = std::move(adapter1);
      }
    } else if (interface == org::bluez::Device1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(device_mutex_);
      if (!devices_.contains(objectPath)) {
        auto device = std::make_unique<Device1>(
            connection_, sdbus::ServiceName(INTERFACE_NAME), objectPath);
        devices_[objectPath] = std::move(device);
      }
    } else if (interface ==
               org::bluez::BatteryProviderManager1_proxy::INTERFACE_NAME) {
      battery_provider_manager1_ = std::make_unique<BatteryProviderManager1>(
          connection_, sdbus::ServiceName(INTERFACE_NAME), objectPath);
    } else if (interface == org::bluez::GattManager1_proxy::INTERFACE_NAME) {
      gatt_manager1_ = std::make_unique<GattManager1>(
          connection_, sdbus::ServiceName(INTERFACE_NAME), objectPath);
    } else if (interface ==
               org::bluez::LEAdvertisingManager1_proxy::INTERFACE_NAME) {
      le_advertising_manager1_ = std::make_unique<LEAdvertisingManager1>(
          connection_, sdbus::ServiceName(INTERFACE_NAME), objectPath);
    } else if (interface == org::bluez::Media1_proxy::INTERFACE_NAME) {
      media1_ = std::make_unique<Media1>(
          connection_, sdbus::ServiceName(INTERFACE_NAME), objectPath);
    } else if (interface == org::bluez::NetworkServer1_proxy::INTERFACE_NAME) {
      network_server1_ = std::make_unique<NetworkServer1>(
          connection_, sdbus::ServiceName(INTERFACE_NAME), objectPath);
    }
  }
  spdlog::info("{}", ss.str());
}

void BluezClient::onInterfacesRemoved(
    const sdbus::ObjectPath& objectPath,
    const std::vector<sdbus::InterfaceName>& interfaces) {
  std::stringstream ss;
  ss << std::endl;
  for (auto it = interfaces.begin(); it != interfaces.end(); ++it) {
    ss << "[" << objectPath << "] Remove - " << *it;
    if (std::next(it) != interfaces.end()) {
      ss << std::endl;
    }
    std::scoped_lock lock(device_mutex_);
    if (devices_.contains(objectPath)) {
      devices_[objectPath].reset();
      devices_.erase(objectPath);
    }
  }
  spdlog::info("{}", ss.str());
}
