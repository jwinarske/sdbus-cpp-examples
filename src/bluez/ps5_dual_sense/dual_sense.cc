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

#include "hidraw.hpp"

DualSense::DualSense(sdbus::IConnection& connection)
    : ProxyInterfaces(connection,
                      sdbus::ServiceName(INTERFACE_NAME),
                      sdbus::ObjectPath("/")),
      UdevMonitor({"hidraw"},
                  [&](const char* /* action */,
                      const char* /* dev_node */,
                      const char* sub_system) { get_hidraw_devices(); }) {
  get_hidraw_devices();
  registerProxy();
  for (const auto& [object, interfacesAndProperties] : GetManagedObjects()) {
    onInterfacesAdded(object, interfacesAndProperties);
  }
}

DualSense::~DualSense() {
  stop();
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
      std::scoped_lock lock(adapters_mutex_);
      if (!adapters_.contains(objectPath)) {
        auto adapter1 = std::make_unique<Adapter1>(
            getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
            objectPath, properties);
        adapters_[objectPath] = std::move(adapter1);
      }
    } else if (interface == org::bluez::Device1_proxy::INTERFACE_NAME) {
      auto mod_alias_key = sdbus::MemberName("Modalias");
      if (!properties.contains(mod_alias_key))
        continue;

      auto mod_alias = Device1::parse_modalias(
          properties.at(mod_alias_key).get<std::string>());
      if (mod_alias.has_value()) {
        spdlog::debug("VID: {}, PID: {}, DID: {}", mod_alias.value().vid,
                      mod_alias.value().pid, mod_alias.value().did);
        if (auto [vid, pid, did] = mod_alias.value();
            vid != kVendorId || pid != kProductId || did != kDeviceId) {
          continue;
        }
      } else {
        spdlog::debug("modalias has no value assigned: {}", objectPath);
        continue;
      }

      std::scoped_lock lock(devices_mutex_);
      if (!devices_.contains(objectPath)) {
        auto device = std::make_unique<Device1>(
            getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
            objectPath, properties);

        if (auto props = device->GetProperties(); props.modalias.has_value()) {
          auto [vid, pid, did] = props.modalias.value();
          spdlog::info("Adding: {}, {}, {}", vid, pid, did);
          if (vid == kVendorId && pid == kProductId && did == kDeviceId) {
            // if connected, paired, trusted, and bonded a hidraw device should
            // be ready to use
            if (props.bonded && props.connected && props.paired &&
                props.trusted) {
              const auto dev_key =
                  create_device_key_from_serial_number(props.address);
              hidraw_devices_mutex_.lock();
              Hidraw::dump_info(hidraw_devices_[dev_key]);
              hidraw_devices_mutex_.unlock();
            }

            // Add UPower Display Device
            if (std::string power_path = convert_mac_to_path(props.address);
                !upower_clients_.contains(power_path)) {
              upower_display_devices_mutex_.lock();
              spdlog::info("[Add] UPower Display Device: {}", power_path);
              upower_clients_[power_path] = std::make_unique<UPowerClient>(
                  getProxy().getConnection(), sdbus::ObjectPath(power_path));
              upower_display_devices_mutex_.unlock();
            }
          }
        }

        devices_[objectPath] = std::move(device);
      }
    } else if (interface == org::bluez::Input1_proxy::INTERFACE_NAME) {
      std::lock_guard lock(input1_mutex_);
      if (!input1_.contains(objectPath)) {
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
      std::scoped_lock lock(adapters_mutex_);
      if (!adapters_.contains(objectPath)) {
        if (adapters_.contains(objectPath)) {
          adapters_[objectPath].reset();
          adapters_.erase(objectPath);
        }
      }
    } else if (interface == org::bluez::Device1_proxy::INTERFACE_NAME) {
      std::scoped_lock devices_lock(devices_mutex_);
      if (!devices_.contains(objectPath)) {
        if (devices_.contains(objectPath)) {
          devices_[objectPath].reset();
          devices_.erase(objectPath);
        }
      }
    } else if (interface == org::bluez::Input1_proxy::INTERFACE_NAME) {
      std::lock_guard lock(input1_mutex_);
      if (!input1_.contains(objectPath)) {
        input1_[objectPath].reset();
        input1_.erase(objectPath);
      }
    }
  }
  for (auto it = interfaces.begin(); it != interfaces.end(); ++it) {
    std::scoped_lock lock(devices_mutex_);
    if (devices_.contains(objectPath)) {
      auto& device = devices_[objectPath];

      if (auto props = device->GetProperties(); props.modalias.has_value()) {
        auto [vid, pid, did] = props.modalias.value();
        spdlog::info("Removing: {}, {}, {}", vid, pid, did);
        if (vid == kVendorId && pid == kProductId && did == kDeviceId) {
          if (std::string power_path = convert_mac_to_path(props.address);
              upower_clients_.contains(power_path)) {
            std::scoped_lock power_lock(upower_display_devices_mutex_);
            spdlog::info("[Remove] UPower Display Device: {}", power_path);
            auto& power_device = upower_clients_[power_path];
            power_device.reset();
            upower_clients_.erase(power_path);
          }
        }
      }

      device.reset();
      devices_.erase(objectPath);
    }
  }
}

std::string DualSense::convert_mac_to_path(const std::string& mac_address) {
  std::string result =
      "/org/freedesktop/UPower/devices/battery_ps_controller_battery_";
  std::string converted_mac = mac_address;
  std::ranges::replace(converted_mac, ':', 'o');
  std::ranges::transform(converted_mac, converted_mac.begin(), ::tolower);
  result += converted_mac;
  return result;
}

bool DualSense::compare_subsystem_device_paths(const std::string& input_path,
                                               const std::string& hidraw_path) {
  const std::string prefix = "/sys/devices/virtual/misc/uhid/";

  // Check if both paths start with their respective prefixes
  if (input_path.compare(0, prefix.size(), prefix) != 0 ||
      hidraw_path.compare(0, prefix.size(), prefix) != 0) {
    return false;
  }

  // Extract the common part of the paths
  const std::string input_common = input_path.substr(
      prefix.size(), input_path.find("/input") - prefix.size());
  const std::string hidraw_common = hidraw_path.substr(
      prefix.size(), hidraw_path.find("/hidraw") - prefix.size());

  // Compare the common parts
  return input_common == hidraw_common;
}

std::string DualSense::create_device_key_from_serial_number(
    const std::string& serial_number) {
  std::string converted_serial = serial_number;

  // Remove leading and trailing `"`
  if (!converted_serial.empty() && converted_serial.front() == '"') {
    converted_serial.erase(0, 1);
  }
  if (!converted_serial.empty() && converted_serial.back() == '"') {
    converted_serial.pop_back();
  }

  // Convert to uppercase
  std::ranges::transform(converted_serial, converted_serial.begin(), ::toupper);

  // Replace ':' with '_'
  std::ranges::replace(converted_serial, ':', '_');

  const std::string key = "dev_" + converted_serial;
  return key;
}

void DualSense::get_hidraw_devices() {
  // Clear all contents of hidraw_devices_
  std::scoped_lock lock(hidraw_devices_mutex_);
  hidraw_devices_.clear();

  // Get input devices matching the specified properties
  const auto input_devices = Hidraw::get_udev_properties(
      "input", {{"ID_BUS", "bluetooth"},
                {"NAME", "\"DualSense Wireless Controller\""},
                {"PRODUCT", "5/54c/ce6/8100"}});

  if (input_devices.empty()) {
    SPDLOG_DEBUG("No matching input devices found.");
    return;
  }

  // Get all hidraw devices
  const auto hidraw_devices = Hidraw::get_udev_properties("hidraw");

  for (const auto& [input_path, input_properties] : input_devices) {
    for (const auto& [hidraw_path, hidraw_properties] : hidraw_devices) {
      if (compare_subsystem_device_paths(input_path, hidraw_path)) {
        if (hidraw_properties.contains("DEVNAME")) {
          const auto& hidraw_dev_name = hidraw_properties.at("DEVNAME");
          auto serial_number = input_properties.at("UNIQ");
          const auto dev_key =
              create_device_key_from_serial_number(serial_number);
          spdlog::info(
              "Associated input device path: {} with hidraw device: {}",
              input_path, hidraw_dev_name);
          hidraw_devices_[dev_key] = hidraw_dev_name;
        }
      }
    }
  }
  for (const auto& [key, value] : hidraw_devices_) {
    spdlog::debug("hidraw device: {} = {}", key, value);
    Hidraw::dump_info(value);
  }
}
