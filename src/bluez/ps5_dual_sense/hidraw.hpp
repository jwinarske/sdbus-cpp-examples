// File: hidraw.hpp

#pragma once

#include <iomanip>
#include <ostream>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

#include <fcntl.h>
#include <unistd.h>

#include <libudev.h>
#include <linux/hidraw.h>
#include <linux/input.h>
#include <linux/types.h>

#include <hid/rdf/descriptor_view.hpp>
#include <hid/rdf/parser.hpp>

#include <spdlog/spdlog.h>

class Hidraw {
 public:
  Hidraw();
  ~Hidraw();

  static void print_udev_properties(
      const std::unordered_map<std::string, std::map<std::string, std::string>>&
          property_map) {
    std::ostringstream os;
    os << "\n";
    for (const auto& [path, properties] : property_map) {
      os << "Path: " << path << "\n";
      os << "Properties:\n";
      for (const auto& [key, value] : properties) {
        os << "  " << key << ": " << value << "\n";
      }
      os << "\n";
    }
    spdlog::info(os.str());
  }

  static void print_udev_properties(
      const std::string& sub_system,
      const std::vector<std::pair<std::string, std::string>>& match_params =
          {}) {
    std::ostringstream os;
    os << "\n";
    for (const auto& [path, properties] :
         get_udev_properties(sub_system, match_params)) {
      os << "Path: " << path << "\n";
      os << "Properties:\n";
      for (const auto& [key, value] : properties) {
        os << "  " << key << ": " << value << "\n";
      }
      os << "\n";
    }
    spdlog::info(os.str());
  }

  /**
   * \brief Retrieves the udev framebuffer system attributes.
   *
   * \return An unordered map containing the udev framebuffer system attributes.
   */
  static std::unordered_map<std::string, std::map<std::string, std::string>>
  get_udev_properties(const std::string& sub_system,
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
        }
      }
      udev_device_unref(dev);

      bool match = true;
      for (const auto& [key, value] : match_params) {
        if (!properties.contains(key) || properties.at(key) != value) {
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
    print_udev_properties("hidraw");
    for (const auto& properties :
         get_udev_properties("hidraw") | std::views::values) {
      if (properties.contains("DEVNAME") && properties.contains("DEVPATH")) {
        const auto dev_name = properties.at("DEVNAME");
        if (compare_uhid_vid_pid(properties.at("DEVPATH"), bus, vid, pid) ==
            0) {
          devices.push_back(dev_name);
        }
      }
    }
    return devices;
  }

  static void dump_info(const std::string& device) {
    std::ostringstream ss;

    const int fd = open(device.c_str(), O_RDWR | O_NONBLOCK);

    if (fd < 0) {
      spdlog::error("Unable to open device");
      return;
    }

    // Report Descriptor Size
    int desc_size = 0;
    int res = ioctl(fd, HIDIOCGRDESCSIZE, &desc_size);
    if (res < 0)
      spdlog::error("HIDIOCGRDESCSIZE");
    else
      spdlog::info("Report Descriptor Size: {}", desc_size);

    // Report Descriptor
    hidraw_report_descriptor rpt_desc{};
    rpt_desc.size = desc_size;
    res = ioctl(fd, HIDIOCGRDESC, &rpt_desc);
    if (res < 0) {
      spdlog::error("HIDIOCGRDESC");
    } else {
      const auto view =
          hid::rdf::descriptor_view_base<hid::rdf::reinterpret_iterator>(
              rpt_desc.value, rpt_desc.size);
      const auto usage_id = get_application_usage_id(view);
      spdlog::info("Usage page id: {}", usage_id.page_id());
      spdlog::info("Usage id: {}", usage_id.id());

      ss << "Report Descriptor:\n";
      for (int i = 0; i < rpt_desc.size; i++)
        ss << "0x" << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(rpt_desc.value[i]) << ", ";
      ss << "\n";
    }

    // Raw Name
    char buf[256]{};
    res = ioctl(fd, HIDIOCGRAWNAME(sizeof(buf)), buf);
    if (res < 0)
      spdlog::error("HIDIOCGRAWNAME");
    else
      ss << "HID Name: " << buf << "\n";

    // Physical Location
    res = ioctl(fd, HIDIOCGRAWPHYS(sizeof(buf)), buf);
    if (res < 0)
      spdlog::error("HIDIOCGRAWPHYS");
    else
      ss << "HID Physical Location: " << buf << "\n";

    // Info
    hidraw_devinfo dev_info{};
    res = ioctl(fd, HIDIOCGRAWINFO, &dev_info);
    if (res < 0) {
      spdlog::error("HIDIOCGRAWINFO");
    } else {
      ss << "HID Info:\n";
      ss << "\tBus: " << bus_str(dev_info.bustype) << "\n";
      ss << "\tVID: 0x" << std::hex << std::setw(4) << std::setfill('0')
         << dev_info.vendor << "\n";
      ss << "\tPID: 0x" << std::hex << std::setw(4) << std::setfill('0')
         << dev_info.product << "\n";
    }
    spdlog::info(ss.str());
  }

 private:
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
};