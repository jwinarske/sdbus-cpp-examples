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

#include "dual_sense.h"

#include <cctype>

#include "../../utils/property_utils.h"
#include "../../utils/resource_limits.h"

const std::vector<std::pair<std::string, std::string>> input_match_bt = {
    {"ID_BUS", "bluetooth"},
    {"NAME", "\"DualSense Wireless Controller\""},
    {"PRODUCT", "5/54c/ce6/8100"}};

const std::vector<std::pair<std::string, std::string>> input_match_usb = {
    {"ID_BUS", "usb"},
    {"ID_MODEL", "DualSense_Wireless_Controller"},
    {"ID_VENDOR_ID", "054c"},
    {"ID_MODEL_ID", "0ce6"},
    {"TAGS", ":seat:"}};

DualSense::DualSense(sdbus::IConnection& connection)
    : ProxyInterfaces(connection,
                      sdbus::ServiceName(INTERFACE_NAME),
                      sdbus::ObjectPath("/")),
      UdevMonitor({"hidraw"},
                  [&](const char* action,
                      const char* dev_node,
                      const char* sub_system) {
                    LOG_DEBUG("Action: {}, Device: {}, Subsystem: {}",
                              action ? action : "", dev_node ? dev_node : "",
                              sub_system ? sub_system : "");
                    if (std::strcmp(sub_system, "hidraw") == 0) {
                      if (std::strcmp(action, "remove") == 0) {
                        if (input_reader_) {
                          input_reader_->stop();
                          input_reader_.reset();
                        }
                      }
                      if (!get_hidraw_devices(input_match_bt)) {
                        get_hidraw_devices(input_match_usb);
                      }
                    }
                  }) {
  if (!get_hidraw_devices(input_match_bt)) {
    get_hidraw_devices(input_match_usb);
  }

  registerProxy();
  for (const auto& [object, interfacesAndProperties] : GetManagedObjects()) {
    onInterfacesAdded(object, interfacesAndProperties);
  }
}

DualSense::~DualSense() {
  unregisterProxy();
}

void DualSense::onInterfacesAdded(
    const sdbus::ObjectPath& objectPath,
    const std::map<sdbus::InterfaceName,
                   std::map<sdbus::PropertyName, sdbus::Variant>>&
        interfacesAndProperties) {
  for (const auto& [interface, properties] : interfacesAndProperties) {
    if (interface == PROPERTIES_INTERFACE_NAME ||
        interface == INTROSPECTABLE_INTERFACE_NAME) {
      continue;
    }
    if (interface == org::bluez::Adapter1_proxy::INTERFACE_NAME) {
      if (!adapters_.contains(objectPath)) {
        if (resource_limits::IsAtCapacity(adapters_.size(),
                                          resource_limits::kMaxAdapters)) {
          LOG_WARN("Skipping Adapter1 {}: resource limit reached ({}/{})",
                   objectPath, adapters_.size(), resource_limits::kMaxAdapters);
          continue;
        }
        auto adapter1 = std::make_unique<Adapter1>(
            getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
            objectPath, properties);
        adapters_[objectPath] = std::move(adapter1);
      }
    } else if (interface == org::bluez::Device1_proxy::INTERFACE_NAME) {
      auto mod_alias_key = sdbus::MemberName("Modalias");

      // Safely get the Modalias property
      auto mod_alias_str =
          property_utils::getProperty<std::string>(properties, mod_alias_key);
      if (!mod_alias_str) {
        continue;  // Skip devices without Modalias
      }

      if (auto mod_alias = Device1::parse_modalias(*mod_alias_str);
          mod_alias.has_value()) {
        LOG_DEBUG("VID: {}, PID: {}, DID: {}", mod_alias.value().vid,
                  mod_alias.value().pid, mod_alias.value().did);
        if (auto [vid, pid, did] = mod_alias.value();
            vid != VENDOR_ID || pid != PRODUCT_ID) {
          continue;
        }
      } else {
        LOG_DEBUG("modalias has no value assigned: {}", objectPath);
        continue;
      }

      std::string power_path_to_add;
      std::string hidraw_device_key;
      {
        if (devices_.contains(objectPath)) {
          continue;
        }

        if (resource_limits::IsAtCapacity(devices_.size(),
                                          resource_limits::kMaxDevices)) {
          LOG_WARN("Skipping Device1 {}: resource limit reached ({}/{})",
                   objectPath, devices_.size(), resource_limits::kMaxDevices);
          continue;
        }

        auto device = std::make_unique<Device1>(
            getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
            objectPath, properties);

        if (auto props = device->GetProperties(); props.modalias.has_value()) {
          auto [vid, pid, did] = props.modalias.value();
          LOG_INFO("Adding: {}, {}, {}", vid, pid, did);
          if (vid == VENDOR_ID && pid == PRODUCT_ID) {
            // if connected, paired, trusted, and bonded, a hidraw device should
            // be ready to use
            if (props.connected && props.paired && props.trusted) {
              hidraw_device_key =
                  create_device_key_from_serial_number(props.address);
            }

            power_path_to_add = convert_mac_to_path(props.address);
          }
        }

        devices_[objectPath] = std::move(device);
      }

      if (!hidraw_device_key.empty()) {
        if (const std::string hidraw_device = FindHidDevice(hidraw_device_key);
            !hidraw_device.empty()) {
          LOG_INFO("Adding hidraw device: {}", hidraw_device_key);
          if (!input_reader_) {
            input_reader_ = std::make_unique<InputReader>(hidraw_device);
            input_reader_->start();
          }
        }
      }

      if (!power_path_to_add.empty()) {
        if (!upower_clients_.contains(power_path_to_add)) {
          if (resource_limits::IsAtCapacity(
                  upower_clients_.size(), resource_limits::kMaxUPowerClients)) {
            LOG_WARN(
                "Skipping UPower client {}: resource limit reached ({}/{})",
                power_path_to_add, upower_clients_.size(),
                resource_limits::kMaxUPowerClients);
            continue;
          }
          LOG_INFO("[Add] UPower Display Device: {}", power_path_to_add);
          upower_clients_[power_path_to_add] = std::make_unique<UPowerClient>(
              getProxy().getConnection(), sdbus::ObjectPath(power_path_to_add));
        }
      }
    } else if (interface == org::bluez::Input1_proxy::INTERFACE_NAME) {
      if (!input1_.contains(objectPath)) {
        if (resource_limits::IsAtCapacity(input1_.size(),
                                          resource_limits::kMaxInputEntries)) {
          LOG_WARN("Skipping Input1 {}: resource limit reached ({}/{})",
                   objectPath, input1_.size(),
                   resource_limits::kMaxInputEntries);
          continue;
        }
        input1_[objectPath] = std::make_unique<Input1>(
            getProxy().getConnection(),
            sdbus::ServiceName(org::bluez::Input1_proxy::INTERFACE_NAME),
            objectPath);
      }
    }
  }
}

void DualSense::onInterfacesRemoved(
    const sdbus::ObjectPath& objectPath,
    const std::vector<sdbus::InterfaceName>& interfaces) {
  for (const auto& interface : interfaces) {
    if (interface == org::bluez::Adapter1_proxy::INTERFACE_NAME) {
      if (adapters_.contains(objectPath)) {
        adapters_[objectPath].reset();
        adapters_.erase(objectPath);
      }
    } else if (interface == org::bluez::Device1_proxy::INTERFACE_NAME) {
      std::string power_path_to_remove;
      {
        if (devices_.contains(objectPath)) {
          auto& device = devices_[objectPath];
          if (auto props = device->GetProperties();
              props.modalias.has_value()) {
            auto [vid, pid, did] = props.modalias.value();
            LOG_INFO("Removing: {}, {}, {}", vid, pid, did);
            if (vid == VENDOR_ID && pid == PRODUCT_ID) {
              power_path_to_remove = convert_mac_to_path(props.address);
            }
          }
          device.reset();
          devices_.erase(objectPath);
        }
      }

      if (!power_path_to_remove.empty()) {
        if (upower_clients_.contains(power_path_to_remove)) {
          LOG_INFO("[Remove] UPower Display Device: {}", power_path_to_remove);
          auto& power_device = upower_clients_[power_path_to_remove];
          power_device.reset();
          upower_clients_.erase(power_path_to_remove);
        }
      }
    } else if (interface == org::bluez::Input1_proxy::INTERFACE_NAME) {
      if (input1_.contains(objectPath)) {
        input1_[objectPath].reset();
        input1_.erase(objectPath);
      }
    }
  }
}

std::string DualSense::convert_mac_to_path(const std::string& mac_address) {
  std::string result =
      "/org/freedesktop/UPower/devices/battery_ps_controller_battery_";
  std::string converted_mac = mac_address;
  std::ranges::replace(converted_mac, ':', 'o');
  std::ranges::transform(converted_mac, converted_mac.begin(),
                         [](unsigned char c) { return std::tolower(c); });
  result += converted_mac;
  return result;
}
