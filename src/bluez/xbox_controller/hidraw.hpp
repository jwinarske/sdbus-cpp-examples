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
// #include "hid/page/consumer.hpp"
#include "hid/page/generic_desktop.hpp"
#include "hid/rdf/descriptor.hpp"
#include "hid/report_protocol.hpp"

#include "report_parser.hpp"

#include "vendor.hpp"
#include "vendor1.hpp"

#include "../hexdump.hpp"

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
                     unit::degree::unit_item(),
                     report_size(4),
                     report_count(1),
                     input::absolute_variable(main::field_flags::NULL_STATE),
                     unit::none::unit_item(),

                     usage_page<button>(),
                     usage_limits(button(1), button(14)),
                     logical_limits<1, 1>(0, 1),
                     report_size(1),
                     report_count(14),
                     input::absolute_variable(),
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
                     usage<1>(vendor::SeqNo),
                     report_count(1),
                     input::absolute_variable(),

                     usage_page<generic_desktop>(),
                     usage(generic_desktop::HAT_SWITCH),
                     logical_limits<1, 1>(0, 7),
                     physical_limits<1, 2>(0, 315),
                     unit::degree::unit_item(),
                     report_size(4),
                     report_count(1),
                     input::absolute_variable(main::field_flags::NULL_STATE),
                     unit::none::unit_item(),

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
                     usage<1>(vendor::SetStateData),
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
         get_udev_properties(sub_system, false, match_params)) {
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
    print_udev_properties("hidraw");
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

 private:
  static constexpr std::string DEV_NAME = "DEVNAME";
  static constexpr std::string DEV_PATH = "DEVPATH";

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