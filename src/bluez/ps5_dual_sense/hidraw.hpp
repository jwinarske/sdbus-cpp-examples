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

#include "hid/page/button.hpp"
#include "hid/page/consumer.hpp"
#include "hid/page/generic_desktop.hpp"
#include "hid/rdf/descriptor.hpp"
#include "hid/report_protocol.hpp"

#include "vendor.hpp"
#include "vendor1.hpp"

using namespace hid::rdf;
using namespace hid::rdf;
using namespace hid::page;

class Hidraw {
 public:
  Hidraw();
  ~Hidraw();

  static constexpr auto bt_report_desc =
      descriptor(usage_page<generic_desktop>(),
                 usage(generic_desktop::GAMEPAD),
                 collection::application(

                     report_id(1),
                     usage(generic_desktop::X),
                     usage(generic_desktop::Y),
                     usage(generic_desktop::Z),
                     usage(generic_desktop::RZ),
                     logical_limits<1, 2>(0, 255),
                     report_size(8),
                     report_count(4),
                     input::absolute_variable(),

                     usage(generic_desktop::HAT_SWITCH),
                     logical_limits<1, 1>(0, 7),
                     physical_limits<1, 2>(0, 315),
                     unit::degree::unit_item(),  // requires two bytes, not four
                                                 // - upstream fix needed
                     report_size(4),
                     report_count(1),
                     input::absolute_variable(main::field_flags::NULL_STATE),
                     unit::none::unit_item(),  // requires two bytes, not four -
                                               // upstream fix needed

                     usage_page<button>(),
                     usage_limits(button(1), button(14)),
                     logical_limits<1, 1>(0, 1),
                     report_size(1),
                     report_count(14),
                     input::absolute_variable(),
                     // report_size(6),
                     // report_count(1),
                     // input::constant(),
                     input::padding(6),

                     usage_page<generic_desktop>(),
                     usage(generic_desktop::RX),
                     usage(generic_desktop::RY),

                     logical_limits<1, 2>(0, 255),
                     report_size(8),
                     report_count(2),
                     input::absolute_variable(),

                     usage_page<vendor>(),

                     logical_limits<1, 2>(0, 255),
                     report_size(8),
                     report_count(77),

                     report_id(0x31),
                     usage<1>(vendor::UNKNOWN_1),
                     output::absolute_variable(),
                     usage<1>(vendor::UNKNOWN_2),
                     input::absolute_variable(),

                     report_id(0x32),
                     usage<1>(vendor::UNKNOWN_3),
                     report_count(141),
                     output::absolute_variable(),

                     report_id(0x33),
                     usage<1>(vendor::UNKNOWN_4),
                     report_count(205),
                     output::absolute_variable(),

                     report_id(0x34),
                     usage<1>(vendor::UNKNOWN_5),
                     report_count<2>(269),
                     output::absolute_variable(),

                     report_id(53),
                     usage<1>(vendor::UNKNOWN_6),
                     report_count<2>(333),
                     output::absolute_variable(),

                     report_id(54),
                     usage<1>(vendor::UNKNOWN_7),
                     report_count<2>(397),
                     output::absolute_variable(),

                     report_id(55),
                     usage<1>(vendor::UNKNOWN_8),
                     report_count<2>(461),
                     output::absolute_variable(),

                     report_id(56),
                     usage<1>(vendor::UNKnOWN_9),
                     report_count<2>(525),
                     output::absolute_variable(),

                     report_id(57),
                     usage<1>(vendor::UNKNOWN_10),
                     report_count<2>(546),
                     output::absolute_variable(),

                     usage_page<vendor1>(),

                     report_id(5),
                     usage<1>(vendor1::UNKNOWN_1),
                     report_count(40),
                     feature::absolute_variable(),

                     report_id(8),
                     usage<1>(vendor1::UNKNOWN_2),
                     report_count(47),
                     feature::absolute_variable(),

                     report_id(9),
                     usage<1>(vendor1::UNKNOWN_3),
                     report_count(19),
                     feature::absolute_variable(),

                     report_id(32),
                     usage<1>(vendor1::UNKNOWN_4),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(34),
                     usage<1>(vendor1::UNKNOWN_5),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0x80),
                     usage<1>(vendor1::UNKNOWN_6),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0x81),
                     usage<1>(vendor1::UNKNOWN_7),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0x82),
                     usage<1>(vendor1::UNKNOWN_8),
                     report_count(9),
                     feature::absolute_variable(),

                     report_id(0x83),
                     usage<1>(vendor1::UNKNOWN_9),
                     report_count(9),
                     feature::absolute_variable(),

                     report_id(0xF1),
                     usage<1>(vendor1::UNKNOWN_10),
                     report_count(64),
                     feature::absolute_variable(),

                     report_id(0xF2),
                     usage<1>(vendor1::UNKNOWN_11),
                     report_count(15),
                     feature::absolute_variable(),

                     report_id(0xF0),
                     usage<1>(vendor1::UNKNOWN_12),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0xF4),
                     usage<1>(vendor1::UNKNOWN_13),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0xF5),
                     usage<1>(vendor1::UNKNOWN_14),
                     report_count(7),
                     feature::absolute_variable(),

                     report_id(0xF6),
                     usage<1>(vendor1::UNKNOWN_15),
                     report_count<2>(546),
                     feature::absolute_variable(),

                     report_id(0xF7),
                     usage<1>(vendor1::UNKNOWN_16),
                     report_count(7),
                     feature::absolute_variable()));

  static constexpr auto usb_report_desc =
      descriptor(usage_page<generic_desktop>(),
                 usage(generic_desktop::GAMEPAD),
                 collection::application(

                     report_id(1),
                     usage(generic_desktop::X),
                     usage(generic_desktop::Y),
                     usage(generic_desktop::Z),
                     usage(generic_desktop::RZ),
                     usage(generic_desktop::RX),
                     usage(generic_desktop::RY),
                     logical_limits<1, 2>(0, 255),
                     report_size(8),
                     report_count(6),
                     input::absolute_variable(),

                     usage_page<vendor>(),
                     usage<1>(vendor::USB_1),
                     report_count(1),
                     input::absolute_variable(),
                     usage_page<generic_desktop>(),

                     usage(generic_desktop::HAT_SWITCH),
                     logical_limits<1, 1>(0, 7),
                     physical_limits<1, 2>(0, 315),
                     unit::degree::unit_item(),  // requires two bytes, not four
                                                 // - upstream fix needed
                     report_size(4),
                     report_count(1),
                     input::absolute_variable(main::field_flags::NULL_STATE),
                     unit::none::unit_item(),  // requires two bytes, not four -
                                               // upstream fix needed

                     usage_page<button>(),
                     usage_limits(button(1), button(15)),
                     logical_limits<1, 1>(0, 1),
                     report_size(1),
                     report_count(15),
                     input::absolute_variable(),

                     usage_page<vendor>(),
                     usage<1>(vendor::USB_2),
                     report_count(13),
                     input::absolute_variable(),

                     usage_page<vendor>(),
                     usage<1>(vendor::USB_3),
                     logical_limits<1, 2>(0, 255),
                     report_size(8),
                     report_count(52),
                     input::absolute_variable(),

                     report_id(2),
                     usage<1>(vendor::USB_4),
                     report_count(47),
                     output::absolute_variable(),

                     report_id(5),
                     usage<1>(vendor::UNKNOWN_4),
                     report_count(40),
                     feature::absolute_variable(),

                     report_id(8),
                     usage<1>(vendor::UNKNOWN_5),
                     report_count(47),
                     feature::absolute_variable(),

                     report_id(9),
                     usage<1>(vendor::USB_5),
                     report_count(19),
                     feature::absolute_variable(),

                     report_id(10),
                     usage<1>(vendor::USB_6),
                     report_count(26),
                     feature::absolute_variable(),

                     report_id(32),
                     usage<1>(vendor::USB_7),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(33),
                     usage<1>(vendor::USB_8),
                     report_count(4),
                     feature::absolute_variable(),

                     report_id(34),
                     usage<1>(vendor1::UNKNOWN_5),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0x80),
                     usage<1>(vendor1::UNKNOWN_6),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0x81),
                     usage<1>(vendor1::UNKNOWN_7),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0x82),
                     usage<1>(vendor1::UNKNOWN_8),
                     report_count(9),
                     feature::absolute_variable(),

                     report_id(0x83),
                     usage<1>(vendor1::UNKNOWN_9),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0x84),
                     usage<1>(vendor1::UNKNOWN_13),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0x85),
                     usage<1>(vendor1::UNKNOWN_14),
                     report_count(2),
                     feature::absolute_variable(),

                     report_id(0xA0),
                     usage<1>(vendor1::UNKNOWN_15),
                     report_count(1),
                     feature::absolute_variable(),

                     report_id(0xE0),
                     usage<1>(vendor1::UNKNOWN_16),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0xF0),
                     usage<1>(vendor1::UNKNOWN_12),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0xF1),
                     usage<1>(vendor1::UNKNOWN_10),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0xF2),
                     usage<1>(vendor1::UNKNOWN_11),
                     report_count(15),
                     feature::absolute_variable(),

                     report_id(0xF4),
                     usage<1>(vendor::UNKNOWN_6),
                     report_count(63),
                     feature::absolute_variable(),

                     report_id(0xF5),
                     usage<1>(vendor::UNKNOWN_7),
                     report_count(3),
                     feature::absolute_variable()));

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
    print_udev_properties("hidraw");
    for (const auto& properties :
         get_udev_properties("hidraw") | std::views::values) {
      if (properties.contains(DEVNAME) && properties.contains(DEVPATH)) {
        const auto dev_name = properties.at(DEVNAME);
        if (compare_uhid_vid_pid(properties.at(DEVPATH), bus, vid, pid) == 0) {
          devices.push_back(dev_name);
        }
      }
    }
    return devices;
  }

  static void print_report_protocol(
      const hid::report_protocol& report_protocol) {
    spdlog::info("report_protocol");
    spdlog::info("max_report_size ........ {}",
                 report_protocol.max_report_size());
    spdlog::info("max_feature_id ......... {}", report_protocol.max_feature_id);
    spdlog::info("max_feature_size ....... {}",
                 report_protocol.max_feature_size);
    spdlog::info("max_input_id ........... {}", report_protocol.max_input_id);
    spdlog::info("max_input_size ......... {}", report_protocol.max_input_size);
    spdlog::info("max_output_id .......... {}", report_protocol.max_output_id);
    spdlog::info("max_output_size ........ {}",
                 report_protocol.max_output_size);
    spdlog::info("uses_report_ids: {}",
                 report_protocol.uses_report_ids() ? "True" : "False");
    spdlog::info("descriptor size: {}", report_protocol.descriptor.size());

    for (const auto& item : report_protocol.descriptor) {
      if (const auto type = item.type(); type == item_type::MAIN) {
        spdlog::info("** MAIN");
        const auto tag = item.main_tag();
        if (tag == main::tag::INPUT) {
          spdlog::info("\tINPUT: 0x{:02X}", item.value_unsigned());
        }
        if (tag == main::tag::OUTPUT) {
          spdlog::info("\tOUTPUT: 0x{:02X}", item.value_unsigned());
        }
        if (tag == main::tag::FEATURE) {
          spdlog::info("\tFEATURE: 0x{:02X}", item.value_unsigned());
        }
        if (tag == main::tag::COLLECTION) {
          spdlog::info("\tCOLLECTION");
        }
        if (tag == main::tag::END_COLLECTION) {
          spdlog::info("\tEND_COLLECTION");
        }

      } else if (type == item_type::GLOBAL) {
        spdlog::info("** GLOBAL");
        const auto tag = item.global_tag();
        if (tag == global::tag::USAGE_PAGE) {
          spdlog::info("\tUSAGE_PAGE: 0x{:02X}", item.value_unsigned());
        }
        if (tag == global::tag::LOGICAL_MINIMUM) {
          spdlog::info("\tLOGICAL_MINIMUM: 0x{:02X}", item.value_unsigned());
        }
        if (tag == global::tag::LOGICAL_MAXIMUM) {
          spdlog::info("\tLOGICAL_MAXIMUM: 0x{:02X}", item.value_unsigned());
        }
        if (tag == global::tag::PHYSICAL_MINIMUM) {
          spdlog::info("\tPHYSICAL_MINIMUM: 0x{:02X}", item.value_unsigned());
        }
        if (tag == global::tag::PHYSICAL_MAXIMUM) {
          spdlog::info("\tPHYSICAL_MAXIMUM: 0x{:02X}", item.value_unsigned());
        }
        if (tag == global::tag::UNIT_EXPONENT) {
          spdlog::info("\tUNIT_EXPONENT: 0x{:02X}", item.value_unsigned());
        }
        if (tag == global::tag::UNIT) {
          spdlog::info("\tUNIT: 0x{:02X}", item.value_unsigned());
        }
        if (tag == global::tag::REPORT_SIZE) {
          spdlog::info("\tREPORT_SIZE: 0x{:02X}", item.value_unsigned());
        }
        if (tag == global::tag::REPORT_ID) {
          spdlog::info("\tREPORT_ID: 0x{:02X}", item.value_unsigned());
        }
        if (tag == global::tag::REPORT_COUNT) {
          spdlog::info("\tREPORT_COUNT: 0x{:02X}", item.value_unsigned());
        }
        if (tag == global::tag::PUSH) {
          spdlog::info("\tPUSH");
        }
        if (tag == global::tag::POP) {
          spdlog::info("\tPOP");
        }
      } else if (type == item_type::LOCAL) {
        spdlog::info("** LOCAL");
        const auto tag = item.local_tag();
        if (tag == local::tag::USAGE) {
          spdlog::info("\tUSAGE: 0x{:02X}", item.value_unsigned());
        }
        if (tag == local::tag::USAGE_MINIMUM) {
          spdlog::info("\tUSAGE_MINIMUM: 0x{:02X}", item.value_unsigned());
        }
        if (tag == local::tag::USAGE_MAXIMUM) {
          spdlog::info("\tUSAGE_MAXIMUM: 0x{:02X}", item.value_unsigned());
        }
        if (tag == local::tag::DESIGNATOR_INDEX) {
          spdlog::info("\tDESIGNATOR_INDEX: 0x{:02X}", item.value_unsigned());
        }
        if (tag == local::tag::DESIGNATOR_MINIMUM) {
          spdlog::info("\tDESIGNATOR_MINIMUM: 0x{:02X}", item.value_unsigned());
        }
        if (tag == local::tag::DESIGNATOR_MAXIMUM) {
          spdlog::info("\tDESIGNATOR_MAXIMUM: 0x{:02X}", item.value_unsigned());
        }
        if (tag == local::tag::STRING_INDEX) {
          spdlog::info("\tSTRING_INDEX: 0x{:02X}", item.value_unsigned());
        }
        if (tag == local::tag::STRING_MINIMUM) {
          spdlog::info("\tSTRING_MINIMUM: 0x{:02X}", item.value_unsigned());
        }
        if (tag == local::tag::STRING_MAXIMUM) {
          spdlog::info("\tSTRING_MAXIMUM: 0x{:02X}", item.value_unsigned());
        }
        if (tag == local::tag::DELIMITER) {
          spdlog::info("\tDELIMITER");
        }
      } else if (type == item_type::RESERVED) {
        spdlog::info("** RESERVED");
      }
    }
  }

  // Generate CRC32 lookup table
  static std::array<std::uint32_t, 256> makeCRCTable() {
    std::array<std::uint32_t, 256> table{};
    for (std::size_t n = 0; n < 256; ++n) {
      std::uint32_t c = n;
      for (std::size_t k = 0; k < 8; ++k) {
        c = (c & 1) ? (0xEDB88320 ^ (c >> 1)) : (c >> 1);
      }
      table[n] = c;
    }
    return table;
  }

  // Compute CRC32 for prefixBytes concatenated with dataView
  static std::uint32_t crc32(const std::vector<std::uint8_t>& prefixBytes,
                             const std::vector<std::uint8_t>& dataView) {
    static const auto crcTable = makeCRCTable();
    std::uint32_t crc = 0xFFFFFFFF;
    for (const auto& byte : prefixBytes) {
      crc = (crc >> 8) ^ crcTable[(crc ^ byte) & 0xFF];
    }
    for (const auto& byte : dataView) {
      crc = (crc >> 8) ^ crcTable[(crc ^ byte) & 0xFF];
    }
    return crc ^ 0xFFFFFFFF;
  }

  // compute the CRC32 checksum and write it to the last four bytes
  static void AppendChecksum(const std::uint8_t reportId,
                             std::vector<std::uint8_t>& pkt) {
    const std::vector<std::uint8_t>& prefixBytes = {0xA2, reportId};
    const std::vector dataView(pkt.begin(), pkt.end() - 4);
    const std::uint32_t crc = crc32(prefixBytes, dataView);
    const auto idx = pkt.size();
    pkt[idx - 4] = crc & 0xFF;
    pkt[idx - 3] = (crc >> 8) & 0xFF;
    pkt[idx - 2] = (crc >> 16) & 0xFF;
    pkt[idx - 1] = (crc >> 24) & 0xFF;
  }

  static void GetFeature(const int fd, const std::uint8_t id) {
    std::uint8_t buf[256]{};
    spdlog::info("GetFeature: 0x{:02x}", id);
    buf[0] = id;  // Report Number
    if (const auto res = ioctl(fd, HIDIOCGFEATURE(256), buf); res < 0) {
      perror("HIDIOCGFEATURE");
    } else {
      printf("ioctl HIDIOCGFEATURE returned: %d\n", res);
      printf("Report data:\n\t");
      for (int i = 0; i < res; i++)
        printf("%hhx ", buf[i]);
      puts("\n");
    }
  }

  static void dump_info(const std::string& device) {
    if (device.empty())
      return;

    std::ostringstream ss;

    const int fd = open(device.c_str(), O_RDWR | O_NONBLOCK);

    if (fd < 0) {
      spdlog::error("unable to open device");
      return;
    }

    // Raw Info
    hidraw_devinfo dev_info{};
    auto res = ioctl(fd, HIDIOCGRAWINFO, &dev_info);
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

    // Report Descriptor Size
    int desc_size = 0;
    res = ioctl(fd, HIDIOCGRDESCSIZE, &desc_size);
    if (res < 0) {
      spdlog::error("HIDIOCGRDESCSIZE");
    } else {
      spdlog::info("Report Descriptor Size: {}", desc_size);
    }

    if (dev_info.bustype == BUS_USB) {
      const hid::report_protocol rp(usb_report_desc);
      print_report_protocol(rp);
      assert(rp.descriptor.size() == desc_size);
    } else if (dev_info.bustype == BUS_BLUETOOTH) {
      const hid::report_protocol rp(usb_report_desc);
      print_report_protocol(rp);
      assert(rp.descriptor.size() == desc_size);
    }

    // Raw Name
    std::uint8_t buf[256]{};
    res = ioctl(fd, HIDIOCGRAWNAME(sizeof(buf)), buf);
    if (res < 0)
      spdlog::error("HIDIOCGRAWNAME");
    else
      ss << "HID Name: " << buf << "\n";

    // Raw Physical Location
    res = ioctl(fd, HIDIOCGRAWPHYS(sizeof(buf)), buf);
    if (res < 0)
      spdlog::error("HIDIOCGRAWPHYS");
    else
      ss << "HID Physical Location: " << buf << "\n";

    // Report Descriptor
    hidraw_report_descriptor rpt_desc{};
    rpt_desc.size = desc_size;
    res = ioctl(fd, HIDIOCGRDESC, &rpt_desc);
    if (res < 0) {
      spdlog::error("HIDIOCGRDESC");
    } else {
      const auto view = hid::rdf::descriptor_view_base<reinterpret_iterator>(
          rpt_desc.value, rpt_desc.size);

      const auto usage_id = get_application_usage_id(view);
      spdlog::info("Usage page id: {}", usage_id.page_id());
      spdlog::info("Usage id: {}", usage_id.id());

      ss << "Raw Report Descriptor:\n";
      for (int i = 0; i < rpt_desc.size; i++)
        ss << "0x" << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(rpt_desc.value[i]) << ", ";
      ss << "\n";

      ss << "Reference Report Descriptor:\n";
      for (auto& b : usb_report_desc) {
        ss << "0x" << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(b) << ", ";
      }
    }
    spdlog::debug(ss.str());

    GetFeature(fd, 0x05);  // Get Calibration
    // GetFeature(fd, 0x08); // Get bluetooth control

    DualSense::ReportFeatureInMacAll report_feature_in_mac_all{};
    DualSense::GetControllerAndHostMAC(fd, report_feature_in_mac_all);
    DualSense::PrintControllerAndHostMac(report_feature_in_mac_all);

    // GetFeature(fd, 0x0A); // Set bluetooth pairing
    // GetFeature(fd, 0x20); // Get Controller Version/Date
    // GetFeature(fd, 0x21); // Set audio control
    // GetFeature(fd, 0x22); // Get Hardware Info
    // GetFeature(fd, 0x80); // Set test command
    // GetFeature(fd, 0x81); // Get test result
    // GetFeature(fd, 0x82); // Set calibration command
    // GetFeature(fd, 0x83); // Get calibration data
    // GetFeature(fd, 0x84); // Set individual data
    GetFeature(fd, 0x85);  // Get individual data result
    // GetFeature(fd, 0xA0); // Set DFU enable
    // GetFeature(fd, 0xE0); // Get system profile
    // GetFeature(fd, 0xF0); // Flash command
    // GetFeature(fd, 0xF1); // Get flash cmd status
    GetFeature(fd, 0xF2);  //
    // GetFeature(fd, 0xF4); // User update command
    GetFeature(fd, 0xF5);  // User get update status

    {
      // Get a report from the device

      if (const auto result = read(fd, buf, 64); result < 0) {
        perror("read");
      } else {
        std::ostringstream os;
        os << "read() read " << result << " bytes\n";
        for (int i = 0; i < result; i++)
          os << "0x" << std::hex << std::setw(2) << std::setfill('0') << buf[i]
             << ", ";
        os << "\n";
        spdlog::debug(os.str());
      }

      spdlog::debug("Report ID: 0x{:02x}", buf[0]);

      if (buf[0] == 0x01) {
        const auto state_data =
            reinterpret_cast<DualSense::USBGetStateData*>(&buf[1]);
        DualSense::print_state_data(state_data);
      }
    }

#if 0
    // Set Feature
    buf[0] = 0x1;  // Report Number
    buf[1] = 0xff;
    buf[2] = 0xff;
    buf[3] = 0xff;
    res = ioctl(fd, HIDIOCSFEATURE(4), buf);
    if (res < 0)
      perror("HIDIOCSFEATURE");
    else
      printf("ioctl HIDIOCSFEATURE returned: %d\n", res);

    // Set Feature
    buf[0] = 0x31;  // Report Number
    buf[1] = 0xff;
    buf[2] = 0xff;
    buf[3] = 0xff;
    res = ioctl(fd, HIDIOCSFEATURE(4), buf);
    if (res < 0)
      perror("HIDIOCSFEATURE");
    else
      printf("ioctl HIDIOCSFEATURE returned: %d\n", res);

    // Send a Report to the Device
    buf[0] = 0x1;  // Report Number
    buf[1] = 0x77;
    res = write(fd, buf, 2);
    if (res < 0) {
      printf("Error: %d\n", errno);
      perror("write");
    } else {
      printf("write() wrote %d bytes\n", res);
    }
#endif
    close(fd);
  }

 private:
  static constexpr std::string DEVNAME = "DEVNAME";
  static constexpr std::string DEVPATH = "DEVPATH";

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