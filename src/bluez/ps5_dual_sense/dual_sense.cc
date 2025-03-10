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
                      const char* sub_system) {
                    if (!get_bt_hidraw_devices()) {
                      if (!get_usb_hidraw_devices()) {
                        spdlog::warn("Dual Sense not present.");
                      }
                    }
                  }) {
  if (!get_bt_hidraw_devices()) {
    get_usb_hidraw_devices();
  }

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
  const std::string prefix = "/sys/devices/";

  // Check if both paths start with the prefix
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

bool DualSense::get_usb_hidraw_devices() {
  // Clear all contents of hidraw_devices_
  std::scoped_lock lock(hidraw_devices_mutex_);
  hidraw_devices_.clear();

  // Get input devices matching the specified properties
  const auto usb_input_devices =
      Hidraw::get_udev_properties("input", {{"ID_BUS", "usb"},
                                            {"ID_USB_VENDOR_ID", "054c"},
                                            {"ID_USB_MODEL_ID", "0ce6"},
                                            {"ID_USB_REVISION", "0100"},
                                            {"TAGS", ":seat:"}});

  if (usb_input_devices.empty()) {
    spdlog::warn("No matching USB input devices found.");
    return false;
  }

  // Get all hidraw devices
  const auto hidraw_devices = Hidraw::get_udev_properties("hidraw");

  for (const auto& [input_path, input_properties] : usb_input_devices) {
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
  return true;
}

bool DualSense::get_bt_hidraw_devices() {
  // Clear all contents of hidraw_devices_
  std::scoped_lock lock(hidraw_devices_mutex_);
  hidraw_devices_.clear();

  // Get input devices matching the specified properties
  const auto bt_input_devices = Hidraw::get_udev_properties(
      "input", {{"ID_BUS", "bluetooth"},
                {"NAME", "\"DualSense Wireless Controller\""},
                {"PRODUCT", "5/54c/ce6/8100"}});

  if (bt_input_devices.empty()) {
    spdlog::warn("No matching BT input devices found.");
    return false;
  }

  // Get all hidraw devices
  const auto hidraw_devices = Hidraw::get_udev_properties("hidraw");

  for (const auto& [input_path, input_properties] : bt_input_devices) {
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
  return !hidraw_devices_.empty();
}

void DualSense::print_state_data(USBGetStateData* state_data) {
  spdlog::info("DPad: {}", static_cast<uint8_t>(state_data->DPad));
  spdlog::info("ButtonSquare: {}",
               static_cast<uint8_t>(state_data->ButtonSquare));
  spdlog::info("ButtonCross: {}",
               static_cast<uint8_t>(state_data->ButtonCross));
  spdlog::info("ButtonCircle: {}",
               static_cast<uint8_t>(state_data->ButtonCircle));
  spdlog::info("ButtonTriangle: {}",
               static_cast<uint8_t>(state_data->ButtonTriangle));
  spdlog::info("ButtonL1: {}", static_cast<uint8_t>(state_data->ButtonL1));
  spdlog::info("ButtonR1: {}", static_cast<uint8_t>(state_data->ButtonR1));
  spdlog::info("ButtonL2: {}", static_cast<uint8_t>(state_data->ButtonL2));
  spdlog::info("ButtonR2: {}", static_cast<uint8_t>(state_data->ButtonR2));
  spdlog::info("ButtonCreate: {}",
               static_cast<uint8_t>(state_data->ButtonCreate));
  spdlog::info("ButtonOptions: {}",
               static_cast<uint8_t>(state_data->ButtonOptions));
  spdlog::info("ButtonL3: {}", static_cast<uint8_t>(state_data->ButtonL3));
  spdlog::info("ButtonR3: {}", static_cast<uint8_t>(state_data->ButtonR3));
  spdlog::info("ButtonHome: {}", static_cast<uint8_t>(state_data->ButtonHome));
  spdlog::info("ButtonPad: {}", static_cast<uint8_t>(state_data->ButtonPad));
  spdlog::info("ButtonMute: {}", static_cast<uint8_t>(state_data->ButtonMute));
  spdlog::info("ButtonLeftFunction: {}",
               static_cast<uint8_t>(state_data->ButtonLeftFunction));
  spdlog::info("ButtonRightFunction: {}",
               static_cast<uint8_t>(state_data->ButtonRightFunction));
  spdlog::info("ButtonLeftPaddle: {}",
               static_cast<uint8_t>(state_data->ButtonLeftPaddle));
  spdlog::info("ButtonRightPaddle: {}",
               static_cast<uint8_t>(state_data->ButtonRightPaddle));
  spdlog::info("TriggerRightStopLocation: {}",
               static_cast<uint8_t>(state_data->TriggerRightStopLocation));
  spdlog::info("TriggerRightStatus: {}",
               static_cast<uint8_t>(state_data->TriggerRightStatus));
  spdlog::info("TriggerLeftStopLocation: {}",
               static_cast<uint8_t>(state_data->TriggerLeftStopLocation));
  spdlog::info("TriggerLeftStatus: {}",
               static_cast<uint8_t>(state_data->TriggerLeftStatus));
  spdlog::info("TriggerRightEffect: {}",
               static_cast<uint8_t>(state_data->TriggerRightEffect));
  spdlog::info("TriggerLeftEffect: {}",
               static_cast<uint8_t>(state_data->TriggerLeftEffect));
  spdlog::info("PowerPercent: {}",
               static_cast<uint8_t>(state_data->PowerPercent));
  spdlog::info("PluggedHeadphones: {}",
               static_cast<uint8_t>(state_data->PluggedHeadphones));
  spdlog::info("PluggedMic: {}", static_cast<uint8_t>(state_data->PluggedMic));
  spdlog::info("MicMuted: {}", static_cast<uint8_t>(state_data->MicMuted));
  spdlog::info("PluggedUsbData: {}",
               static_cast<uint8_t>(state_data->PluggedUsbData));
  spdlog::info("PluggedUsbPower: {}",
               static_cast<uint8_t>(state_data->PluggedUsbPower));
  spdlog::info("PluggedExternalMic: {}",
               static_cast<uint8_t>(state_data->PluggedExternalMic));
  spdlog::info("HapticLowPassFilter: {}",
               static_cast<uint8_t>(state_data->HapticLowPassFilter));
}

int DualSense::GetControllerAndHostMAC(
    const int fd,
    ReportFeatureInMacAll& controller_and_host_mac) {
  controller_and_host_mac.ReportID = 0x09;
  if (const auto res = ioctl(fd, HIDIOCGFEATURE(20), &controller_and_host_mac);
      res < 0) {
    spdlog::error("GetControllerAndHostMAC failed: {}", strerror(errno));
    return 1;
  }
  if (controller_and_host_mac.ReportID != 0x09 ||
      controller_and_host_mac.Hard08 != 0x08 ||
      controller_and_host_mac.Hard25 != 0x25 ||
      controller_and_host_mac.Hard00 != 0x00) {
    spdlog::error("GetControllerAndHostMAC invalid response");
    return 1;
  }
  return 0;
}

void DualSense::PrintControllerAndHostMac(
    ReportFeatureInMacAll const& controller_and_host_mac) {
  std::ostringstream os;
  os << "ReportID: " << std::hex << std::setw(2) << std::setfill('0')
     << static_cast<int>(controller_and_host_mac.ReportID) << "\n";

  for (int i = 0; i < 6; i++) {
    os << "ClientMac[" << i << "]: " << std::hex << std::setw(2)
       << std::setfill('0')
       << static_cast<int>(controller_and_host_mac.ClientMac[i]) << "\n";
  }
  os << "Hard08: " << std::hex << std::setw(2) << std::setfill('0')
     << static_cast<int>(controller_and_host_mac.Hard08) << "\n";
  os << "Hard25: " << std::hex << std::setw(2) << std::setfill('0')
     << static_cast<int>(controller_and_host_mac.Hard25) << "\n";
  os << "Hard00: " << std::hex << std::setw(2) << std::setfill('0')
     << static_cast<int>(controller_and_host_mac.Hard00) << "\n";

  for (int i = 0; i < 6; i++) {
    os << "HostMac[" << i << "]: " << std::hex << std::setw(2)
       << std::setfill('0')
       << static_cast<int>(controller_and_host_mac.HostMac[i]) << "\n";
  }
  for (int i = 0; i < 3; i++) {
    os << "HostMac[" << i << "]: " << std::hex << std::setw(2)
       << std::setfill('0') << static_cast<int>(controller_and_host_mac.Pad[i])
       << "\n";
  }
  spdlog::info(os.str());
}