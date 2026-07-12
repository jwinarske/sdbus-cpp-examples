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

#ifndef SRC_BLUEZ_HIDRAW_HPP_
#define SRC_BLUEZ_HIDRAW_HPP_

#include <algorithm>
#include <map>
#include <ranges>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include <unistd.h>

#include <libudev.h>
#include <linux/hidraw.h>
#include <linux/input.h>
#include <linux/types.h>

#include <cctype>

#include "../utils/logging.h"
#include "../utils/unique_fd.h"

#include "hexdump.hpp"

class Hidraw {
 public:
  Hidraw() = default;
  virtual ~Hidraw() = default;

  // Delete copy operations - this class manages a mutex which cannot be copied
  Hidraw(const Hidraw&) = delete;
  Hidraw& operator=(const Hidraw&) = delete;

  // Delete move operations - std::mutex is not movable
  // If move semantics are required in the future, would need to use
  // std::unique_ptr<std::mutex> instead of std::mutex directly
  Hidraw(Hidraw&&) = delete;
  Hidraw& operator=(Hidraw&&) = delete;

  /// Thread-safe lookup of a hidraw device by key.
  /// Returns the device path string if found, or an empty string if not.
  /// Internally uses std::scoped_lock — callers never touch the mutex directly.
  [[nodiscard]] std::string FindHidDevice(const std::string& key) const {
    std::scoped_lock lock(devices_mutex_);
    if (const auto it = devices_.find(key); it != devices_.end()) {
      return it->second;
    }
    return {};
  }

  /**
   * \brief RAII wrapper for udev context
   */
  struct UdevDeleter {
    void operator()(udev* u) const {
      if (u) {
        udev_unref(u);
      }
    }
  };
  using UdevPtr = std::unique_ptr<udev, UdevDeleter>;

  /**
   * \brief RAII wrapper for udev_enumerate
   */
  struct UdevEnumerateDeleter {
    void operator()(udev_enumerate* e) const {
      if (e) {
        udev_enumerate_unref(e);
      }
    }
  };
  using UdevEnumeratePtr =
      std::unique_ptr<udev_enumerate, UdevEnumerateDeleter>;

  /**
   * \brief RAII wrapper for udev_device
   */
  struct UdevDeviceDeleter {
    void operator()(udev_device* d) const {
      if (d) {
        udev_device_unref(d);
      }
    }
  };
  using UdevDevicePtr = std::unique_ptr<udev_device, UdevDeviceDeleter>;

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

    // Use RAII wrapper for automatic cleanup
    const UdevPtr udev(udev_new());
    if (!udev) {
      LOG_ERROR("Can't create udev");
      return results;  // Safe - RAII cleans up automatically
    }

    // Use RAII wrapper for enumerating
    const UdevEnumeratePtr enumerate(udev_enumerate_new(udev.get()));
    if (!enumerate) {
      LOG_ERROR("Can't create udev enumerate");
      return results;  // Safe - both RAII objects clean up
    }

    if (const int res = udev_enumerate_add_match_subsystem(enumerate.get(),
                                                           sub_system.c_str());
        res < 0) {
      LOG_ERROR("Failed to add subsystem match: {}", sub_system);
      return results;  // Safe - RAII cleanup
    }

    if (const int res = udev_enumerate_scan_devices(enumerate.get()); res < 0) {
      LOG_ERROR("Failed to scan devices");
      return results;  // Safe - RAII cleanup
    }

    const auto devices = udev_enumerate_get_list_entry(enumerate.get());
    udev_list_entry* dev_list_entry;
    udev_list_entry_foreach(dev_list_entry, devices) {
      std::map<std::string, std::string> properties;

      const auto path = udev_list_entry_get_name(dev_list_entry);
      if (!path) {
        continue;  // Skip invalid entries
      }

      // Use RAII wrapper for a device
      UdevDevicePtr dev(udev_device_new_from_syspath(udev.get(), path));
      if (!dev) {
        LOG_DEBUG("Failed to get device from syspath: {}", path);
        continue;  // Skip this device, continue with others
      }

      const auto properties_list =
          udev_device_get_properties_list_entry(dev.get());
      udev_list_entry* properties_list_entry;
      udev_list_entry_foreach(properties_list_entry, properties_list) {
        const auto properties_name =
            udev_list_entry_get_name(properties_list_entry);
        if (properties_name) {
          const auto value =
              udev_device_get_property_value(dev.get(), properties_name);
          properties[properties_name] = value ? value : "";
          if (debug) {
            LOG_DEBUG("  {} = {}", properties_name, value ? value : "");
          }
        }
      }
      // dev automatically cleaned up at the end of scope

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
    // enumerate and udev automatically cleaned up here
    return results;
  }

  static std::vector<std::string> get_hidraw_devices(const int bus,
                                                     const std::string& vid,
                                                     const std::string& pid) {
    std::vector<std::string> devices;
    for (const auto& properties :
         get_udev_properties("hidraw", false) | std::views::values) {
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
        get_udev_properties("input", false, match_params);

    if (input_devices.empty()) {
      return false;
    }

    // Get all hidraw devices
    const auto hidraw_devices = get_udev_properties("hidraw");

    for (const auto& [input_path, input_properties] : input_devices) {
      for (const auto& [hidraw_path, hidraw_properties] : hidraw_devices) {
        if (compare_subsystem_device_paths(input_path, hidraw_path)) {
          if (hidraw_properties.contains(DEV_NAME)) {
            const auto& hidraw_dev_name = hidraw_properties.at(DEV_NAME);
            if (!input_properties.contains("UNIQ")) {
              continue;
            }
            auto serial_number = input_properties.at("UNIQ");
            const auto dev_key =
                create_device_key_from_serial_number(serial_number);
            LOG_INFO("Associated input device path: {} with hidraw device: {}",
                     input_path, hidraw_dev_name);
            devices_[dev_key] = hidraw_dev_name;
          }
        }
      }
    }
    for (const auto& value : devices_ | std::views::values) {
      LOG_DEBUG("hidraw device: {}", value);
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

    // Find the positions of "/input" and "/hidraw"
    const auto input_pos = input_path.find("/input");
    const auto hidraw_pos = hidraw_path.find("/hidraw");

    // Validate that both substrings were found
    if (input_pos == std::string::npos) {
      LOG_DEBUG("Path does not contain '/input': {}", input_path);
      return false;
    }

    if (hidraw_pos == std::string::npos) {
      LOG_DEBUG("Path does not contain '/hidraw': {}", hidraw_path);
      return false;
    }

    // Ensure the positions are after the prefix
    if (input_pos < prefix.size() || hidraw_pos < prefix.size()) {
      LOG_DEBUG("Invalid path structure - subsystem before prefix");
      return false;
    }

    // Extract the common part of the paths (safe now that we've validated)
    const std::string input_common =
        input_path.substr(prefix.size(), input_pos - prefix.size());
    const std::string hidraw_common =
        hidraw_path.substr(prefix.size(), hidraw_pos - prefix.size());

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
                           [](unsigned char c) { return std::toupper(c); });

    // Replace ':' with '_'
    std::ranges::replace(converted_serial, ':', '_');

    const std::string key = "dev_" + converted_serial;
    return key;
  }

 private:
  static constexpr std::string DEV_NAME = "DEVNAME";
  static constexpr std::string DEV_PATH = "DEVPATH";

  mutable std::mutex devices_mutex_;
  std::map<std::string, std::string> devices_;
};

#endif  // SRC_BLUEZ_HIDRAW_HPP_
