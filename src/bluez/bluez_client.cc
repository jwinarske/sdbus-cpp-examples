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
#include "gatt_service1.h"
#include "le_advertising_manager1.h"
#include "media1.h"
#include "network_server1.h"

BluezClient::BluezClient(sdbus::IConnection& connection)
    : ProxyInterfaces(connection,
                      sdbus::ServiceName(INTERFACE_NAME),
                      sdbus::ObjectPath("/")) {
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
    if (interface == PROPERTIES_INTERFACE_NAME ||
        interface == INTROSPECTABLE_INTERFACE_NAME) {
      continue;
    }
    ss << "[" << objectPath << "] Add - " << interface << std::endl;
    if (!properties.empty()) {
      Utils::append_properties(properties, ss);
    }
    if (interface == org::bluez::Adapter1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(adapters_mutex_);
      if (!adapters_.contains(objectPath)) {
        auto adapter1 = std::make_unique<Adapter1>(
            getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
            objectPath, properties);
        adapters_[objectPath] = std::move(adapter1);
      }
    } else if (interface == org::bluez::Device1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(devices_mutex_);
      if (!devices_.contains(objectPath)) {
        auto device = std::make_unique<Device1>(
            getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
            objectPath, properties);
        devices_[objectPath] = std::move(device);
      }
    } else if (interface == org::bluez::GattService1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(gatt_services_mutex_);
      if (!gatt_services_.contains(objectPath)) {
        auto device = std::make_unique<GattService1>(
            getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
            objectPath, properties);
        gatt_services_[objectPath] = std::move(device);
      }
    } else if (interface ==
               org::bluez::GattCharacteristic1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(gatt_services_mutex_);
      auto key = sdbus::MemberName("Service");
      auto object_path = properties.at(key).get<sdbus::ObjectPath>();
      gatt_characteristics_[objectPath] = std::make_unique<GattCharacteristic1>(
          getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
          objectPath, properties);
    } else if (interface == org::bluez::GattDescriptor1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(gatt_services_mutex_);
      auto key = sdbus::MemberName("Characteristic");
      auto object_path = properties.at(key).get<sdbus::ObjectPath>();
      gatt_descriptors_[objectPath] = std::make_unique<GattDescriptor1>(
          getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
          objectPath, properties);
    } else if (interface ==
               org::bluez::BatteryProviderManager1_proxy::INTERFACE_NAME) {
      battery_provider_manager1_ = std::make_unique<BatteryProviderManager1>(
          getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
          objectPath);
    } else if (interface == org::bluez::Battery1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(battery1_mutex_);
      if (!battery1_.contains(objectPath)) {
        auto device = std::make_unique<Battery1>(
            getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
            objectPath);
        battery1_[objectPath] = std::move(device);
      }
    } else if (interface == org::bluez::GattManager1_proxy::INTERFACE_NAME) {
      gatt_manager1_ = std::make_unique<GattManager1>(
          getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
          objectPath);
    } else if (interface ==
               org::bluez::LEAdvertisingManager1_proxy::INTERFACE_NAME) {
      le_advertising_manager1_ = std::make_unique<LEAdvertisingManager1>(
          getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
          objectPath, properties);
    } else if (interface == org::bluez::Media1_proxy::INTERFACE_NAME) {
      media1_ = std::make_unique<Media1>(getProxy().getConnection(),
                                         sdbus::ServiceName(INTERFACE_NAME),
                                         objectPath);
    } else if (interface == org::bluez::NetworkServer1_proxy::INTERFACE_NAME) {
      network_server1_ = std::make_unique<NetworkServer1>(
          getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
          objectPath);
    }
  }
  spdlog::info("{}", ss.str());
}

void BluezClient::onInterfacesRemoved(
    const sdbus::ObjectPath& objectPath,
    const std::vector<sdbus::InterfaceName>& interfaces) {
  std::stringstream ss;
  ss << std::endl;
  for (const auto& interface : interfaces) {
    if (interface == org::bluez::Adapter1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(adapters_mutex_);
      if (!adapters_.contains(objectPath)) {
        if (adapters_.contains(objectPath)) {
          adapters_[objectPath].reset();
          adapters_.erase(objectPath);
        }
      }
    } else if (interface == org::bluez::Device1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(devices_mutex_);
      if (!devices_.contains(objectPath)) {
        if (devices_.contains(objectPath)) {
          devices_[objectPath].reset();
          devices_.erase(objectPath);
        }
      }
    } else if (interface == org::bluez::GattService1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(gatt_services_mutex_);
      if (!gatt_services_.contains(objectPath)) {
        if (gatt_services_.contains(objectPath)) {
          gatt_services_[objectPath].reset();
          gatt_services_.erase(objectPath);
        }
      }
    } else if (interface == org::bluez::Battery1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(battery1_mutex_);
      if (!battery1_.contains(objectPath)) {
        if (battery1_.contains(objectPath)) {
          battery1_[objectPath].reset();
          battery1_.erase(objectPath);
        }
      }
    } else if (interface ==
               org::bluez::BatteryProviderManager1_proxy::INTERFACE_NAME) {
      battery_provider_manager1_.reset();
    } else if (interface == org::bluez::GattManager1_proxy::INTERFACE_NAME) {
      gatt_manager1_.reset();
    } else if (interface ==
               org::bluez::LEAdvertisingManager1_proxy::INTERFACE_NAME) {
      le_advertising_manager1_.reset();
    } else if (interface == org::bluez::Media1_proxy::INTERFACE_NAME) {
      media1_.reset();
    } else if (interface == org::bluez::NetworkServer1_proxy::INTERFACE_NAME) {
      network_server1_.reset();
    }
  }
  for (auto it = interfaces.begin(); it != interfaces.end(); ++it) {
    ss << "[" << objectPath << "] Remove - " << *it;
    if (std::next(it) != interfaces.end()) {
      ss << std::endl;
    }
    std::scoped_lock lock(devices_mutex_);
    if (devices_.contains(objectPath)) {
      devices_[objectPath].reset();
      devices_.erase(objectPath);
    }
  }
  spdlog::info("{}", ss.str());
}
