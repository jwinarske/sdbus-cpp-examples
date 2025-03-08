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

#ifndef SRC_DUAL_SENSE_H
#define SRC_DUAL_SENSE_H

#include <vector>

#if 0
#include "hid/page/button.hpp"
#include "hid/page/consumer.hpp"
#include "hid/page/generic_desktop.hpp"
#include "hid/rdf/descriptor.hpp"
#endif

#include "../../upower/upower_display_device.h"
#include "../adapter1.h"
#include "../device1.h"
#include "../input1.h"
#include "udev_monitor.hpp"

#if 0
#include "vendor.hpp"
#include "vendor1.hpp"
#endif

class UPowerDisplayDevice;
class DualSense final
    : public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy>,
      public UdevMonitor {
 public:
  explicit DualSense(sdbus::IConnection& connection);

  ~DualSense() override;

#if 0  // pending upstream changes
  template <uint8_t REPORT_ID = 0>
  static constexpr auto report_descriptor() {
    using namespace hid::page;
    using namespace hid::rdf;
    using namespace hid::rdf::global;

    // clang-format off
      return descriptor(
          usage_page<generic_desktop>(),
          usage(generic_desktop::GAMEPAD),
          collection::application(

              report_id(1),
              usage(generic_desktop::X),
              usage(generic_desktop::Y),
              usage(generic_desktop::Z),
              usage(generic_desktop::RZ),
              logical_limits<1,2>(0,255),
              report_size(8),
              report_count(4),
              input::absolute_variable(),

              usage(generic_desktop::HAT_SWITCH),
              logical_limits<1,1>(0, 7),
              physical_limits<1,2>(0,315),
              unit::degree(),
              report_size(4),
              report_count(1),
              input::absolute_variable(main::field_flags::NULL_STATE),
              unit::none(),

              usage_page<button>(),
              usage_limits(button(1), button(14)),
              logical_limits<1,1>(0, 1),
              report_size(1),
              report_count(14),
              input::absolute_variable(),
              report_size(6),
              report_count(1),
              input::constant(),

              usage_page<generic_desktop>(),
              usage(generic_desktop::RX),
              usage(generic_desktop::RY),

              logical_limits<1,2>(0, 255),
              report_size(8),
              report_count(2),
              input::absolute_variable(),

              usage_page<vendor>(),

              logical_limits<1,2>(0, 255),
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
              feature::absolute_variable()
));
    //clang-format on
}
#endif

 private:
  static constexpr auto INTERFACE_NAME = "org.bluez";
  static constexpr auto PROPERTIES_INTERFACE_NAME =
      "org.freedesktop.DBus.Properties";
  static constexpr auto INTROSPECTABLE_INTERFACE_NAME =
      "org.freedesktop.DBus.Introspectable";

  std::mutex adapters_mutex_;
  std::map<sdbus::ObjectPath, std::unique_ptr<Adapter1>> adapters_;

  std::mutex devices_mutex_;
  std::map<sdbus::ObjectPath, std::unique_ptr<Device1>> devices_;

  std::mutex input1_mutex_;
  std::map<sdbus::ObjectPath, std::unique_ptr<Input1>> input1_;

  std::mutex hidraw_devices_mutex_;
  std::map<std::string, std::string> hidraw_devices_;

  std::mutex upower_display_devices_mutex_;
  std::map<std::string, std::unique_ptr<UPowerDisplayDevice>> upower_display_devices_;

  void onInterfacesAdded(
    const sdbus::ObjectPath& objectPath,
    const std::map<sdbus::InterfaceName,
                   std::map<sdbus::PropertyName, sdbus::Variant>>&
        interfacesAndProperties) override;

  void onInterfacesRemoved(
    const sdbus::ObjectPath& objectPath,
    const std::vector<sdbus::InterfaceName>& interfaces) override;

  void get_hidraw_devices();

  static std::string convert_mac_to_path(const std::string& mac_address);

  static bool compare_subsystem_device_paths(const std::string& input_path,
                          const std::string& hidraw_path);

  static std::string create_device_key_from_serial_number(const std::string& serial_number);
};

#endif  // SRC_DUAL_SENSE_H
