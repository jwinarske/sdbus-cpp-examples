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

#ifndef SRC_BLUEZ_XBOX_CONTROLLER_HIDRAW_HPP_
#define SRC_BLUEZ_XBOX_CONTROLLER_HIDRAW_HPP_

#include <algorithm>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

#include <libudev.h>
#include <linux/hidraw.h>
#include <linux/input.h>
#include <linux/types.h>

#include <spdlog/spdlog.h>

#include "hexdump.hpp"

class Hidraw {
 public:
  Hidraw() = default;
  virtual ~Hidraw() = default;

  void HidDevicesLock() { devices_mutex_.lock(); }

  void HidDevicesUnlock() { devices_mutex_.unlock(); }

  [[nodiscard]] bool HidDevicesContains(const std::string& key) const {
    return devices_.contains(key);
  }

  [[nodiscard]] const std::string& GetHidDevice(
      const std::string& dev_key) const {
    return devices_.at(dev_key);
  }

  /**
   * \brief Retrieves the udev framebuffer system attributes.
   *
   * \return An unordered map containing the udev framebuffer system attributes.
   */
  static std::unordered_map<std::string, std::map<std::string, std::string>>
  get_udev_properties(const std::string& sub_system,
                      const bool debug = false,
                      const std::vector<std::pair<std::string, std::string>>&
                          match_params = {}) {
    std::unordered_map<std::string, std::map<std::string, std::string>> results;
    const auto udev = udev_new();
    if (!udev) {
      spdlog::error("Can't create udev");
      return results;
    }

    const auto enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, sub_system.c_str());
    udev_enumerate_scan_devices(enumerate);

    const auto devices = udev_enumerate_get_list_entry(enumerate);
    udev_list_entry* dev_list_entry;
    udev_list_entry_foreach(dev_list_entry, devices) {
      std::map<std::string, std::string> properties;

      const auto path = udev_list_entry_get_name(dev_list_entry);
      const auto dev = udev_device_new_from_syspath(udev, path);

      const auto properties_list = udev_device_get_properties_list_entry(dev);
      udev_list_entry* properties_list_entry;
      udev_list_entry_foreach(properties_list_entry, properties_list) {
        const auto properties_name =
            udev_list_entry_get_name(properties_list_entry);
        if (properties_name) {
          const auto value =
              udev_device_get_property_value(dev, properties_name);
          properties[properties_name] = value ? value : "";
          if (debug) {
            spdlog::debug("  {} = {}", properties_name, value ? value : "");
          }
        }
      }
      udev_device_unref(dev);

      bool match = true;
      for (const auto& [key, value] : match_params) {
        if (auto it = properties.find(key);
            it == properties.end() || it->second != value) {
          match = false;
          break;
        }
      }
      if (match) {
        results[path] = std::move(properties);
      }
    }
    udev_unref(udev);
    return results;
  }

  static std::vector<std::string> get_hidraw_devices(const int bus,
                                                     const std::string& vid,
                                                     const std::string& pid) {
    std::vector<std::string> devices;
    for (const auto& properties :
         get_udev_properties("hidraw", true) | std::views::values) {
      if (properties.contains(DEV_NAME) && properties.contains(DEV_PATH)) {
        const auto dev_name = properties.at(DEV_NAME);
        if (compare_uhid_vid_pid(properties.at(DEV_PATH), bus, vid, pid) == 0) {
          devices.push_back(dev_name);
        }
      }
    }
    return devices;
  }

  bool get_hidraw_devices(
      const std::vector<std::pair<std::string, std::string>>& match_params =
          {}) {
    // Clear all contents of hidraw_devices_
    std::scoped_lock lock(devices_mutex_);
    devices_.clear();

    // Get input devices matching the specified properties
    const auto input_devices =
        Hidraw::get_udev_properties("input", true, match_params);

    if (input_devices.empty()) {
      spdlog::warn("No matching input devices found.");
      return false;
    }

    // Get all hidraw devices
    const auto hidraw_devices = Hidraw::get_udev_properties("hidraw");

    for (const auto& [input_path, input_properties] : input_devices) {
      for (const auto& [hidraw_path, hidraw_properties] : hidraw_devices) {
        if (compare_subsystem_device_paths(input_path, hidraw_path)) {
          if (hidraw_properties.contains(DEV_NAME)) {
            const auto& hidraw_dev_name = hidraw_properties.at(DEV_NAME);
            auto serial_number = input_properties.at("UNIQ");
            const auto dev_key =
                create_device_key_from_serial_number(serial_number);
            spdlog::info(
                "Associated input device path: {} with hidraw device: {}",
                input_path, hidraw_dev_name);
            devices_[dev_key] = hidraw_dev_name;
          }
        }
      }
    }
    for (const auto& value : devices_ | std::views::values) {
      spdlog::debug("hidraw device: {}", value);
    }
    return true;
  }

  static const char* bus_str(const std::uint32_t bus) {
    switch (bus) {
      case BUS_USB:
        return "USB";
      case BUS_HIL:
        return "HIL";
      case BUS_BLUETOOTH:
        return "Bluetooth";
      case BUS_VIRTUAL:
        return "Virtual";
      default:
        return "Other";
    }
  }

  static int compare_uhid_vid_pid(const std::string& path,
                                  const int bus,
                                  const std::string& vid,
                                  const std::string& pid) {
    const std::regex re(
        R"(/devices/virtual/misc/uhid/([0-9A-Fa-f]{4}):([0-9A-Fa-f]{4}):([0-9A-Fa-f]{4})\..*)");
    if (std::smatch match;
        std::regex_search(path, match, re) && match.size() == 4) {
      const int b = std::stoi(match[1].str(), nullptr, 16);
      const std::string v = match[2].str();
      if (const std::string p = match[3].str();
          bus == b && vid == v && pid == p)
        return 0;
    }
    return 1;
  }

  static bool compare_subsystem_device_paths(const std::string& input_path,
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

  static std::string create_device_key_from_serial_number(
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
    std::ranges::transform(converted_serial, converted_serial.begin(),
                           ::toupper);

    // Replace ':' with '_'
    std::ranges::replace(converted_serial, ':', '_');

    const std::string key = "dev_" + converted_serial;
    return key;
  }

 private:
  static constexpr std::string DEV_NAME = "DEVNAME";
  static constexpr std::string DEV_PATH = "DEVPATH";

  std::mutex devices_mutex_;
  std::map<std::string, std::string> devices_;
};

#endif  // SRC_BLUEZ_XBOX_CONTROLLER_HIDRAW_HPP_
