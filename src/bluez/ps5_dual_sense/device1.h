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

#ifndef SRC_BLUEZ_DEVICE1_H
#define SRC_BLUEZ_DEVICE1_H

#include "../../proxy/org/bluez/Device1/device1_proxy.h"

#include <codecvt>

#include <hidapi/hidapi.h>

#include "../../utils/utils.h"

class Device1 : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                              org::bluez::Device1_proxy> {
 public:
  Device1(sdbus::IConnection& connection,
          const sdbus::ServiceName(&destination),
          const sdbus::ObjectPath(&objectPath),
          const std::map<sdbus::PropertyName, sdbus::Variant>& properties)
      : ProxyInterfaces{connection, destination, objectPath} {
    registerProxy();
    onPropertiesChanged(sdbus::InterfaceName(Device1_proxy::INTERFACE_NAME),
                        properties, {});
  }

  virtual ~Device1() {
    unregisterProxy();
    FinalizeHid();
  }

 private:
  sdbus::ObjectPath adapter_;
  std::string address_;
  std::string address_type_;
  std::string alias_;
  bool blocked_{};
  bool bonded_{};
  bool connected_{};
  bool legacy_pairing_{};
  std::string modalias_;
  std::string name_;
  bool paired_{};
  std::map<std::uint16_t, sdbus::Variant> manufacturer_data_;
  std::map<std::string, sdbus::Variant> service_data_;
  std::int16_t rssi_{};
  bool services_resolved_{};
  bool trusted_{};
  std::vector<std::string> uuids_;

  hid_device* handle_{};

  static int IntializeHid() {
    if (const int res = hid_init(); res < 0) {
      spdlog::error("hid_init failed");
      return 1;
    }
    return 0;
  }

  const char* hid_bus_name(hid_bus_type bus_type) {
    static const char* const HidBusTypeName[] = {
        "Unknown", "USB", "Bluetooth", "I2C", "SPI",
    };

    if ((int)bus_type < 0)
      bus_type = HID_API_BUS_UNKNOWN;
    if ((int)bus_type >=
        (int)(sizeof(HidBusTypeName) / sizeof(HidBusTypeName[0])))
      bus_type = HID_API_BUS_UNKNOWN;

    return HidBusTypeName[bus_type];
  }

  void print_device(struct hid_device_info* cur_dev) {
    printf(
        "Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls",
        cur_dev->vendor_id, cur_dev->product_id, cur_dev->path,
        cur_dev->serial_number);
    printf("\n");
    printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
    printf("  Product:      %ls\n", cur_dev->product_string);
    printf("  Release:      %hx\n", cur_dev->release_number);
    printf("  Interface:    %d\n", cur_dev->interface_number);
    printf("  Usage (page): 0x%hx (0x%hx)\n", cur_dev->usage,
           cur_dev->usage_page);
    printf("  Bus type: %u (%s)\n", (unsigned)cur_dev->bus_type,
           hid_bus_name(cur_dev->bus_type));
    printf("\n");
  }

  void print_hid_report_descriptor_from_device(hid_device* device) {
    unsigned char descriptor[HID_API_MAX_REPORT_DESCRIPTOR_SIZE];
    int res = 0;

    printf("  Report Descriptor: ");
#if HID_API_VERSION >= HID_API_MAKE_VERSION(0, 14, 0)
    res = hid_get_report_descriptor(device, descriptor, sizeof(descriptor));
#else
    (void)res;
#endif
    if (res < 0) {
      printf("error getting: %ls", hid_error(device));
    } else {
      printf("(%d bytes)", res);
    }
    for (int i = 0; i < res; i++) {
      if (i % 10 == 0) {
        printf("\n");
      }
      printf("0x%02x, ", descriptor[i]);
    }
    printf("\n");
  }

  void print_hid_report_descriptor_from_path(const char* path) {
    hid_device* device = hid_open_path(path);
    if (device) {
      print_hid_report_descriptor_from_device(device);
      hid_close(device);
    } else {
      spdlog::info("  Report Descriptor: Unable to open device by path");
    }
  }

  void print_devices(struct hid_device_info* cur_dev) {
    for (; cur_dev; cur_dev = cur_dev->next) {
      print_device(cur_dev);
    }
  }

  void print_devices_with_descriptor(struct hid_device_info* cur_dev) {
    for (; cur_dev; cur_dev = cur_dev->next) {
      print_device(cur_dev);
      print_hid_report_descriptor_from_path(cur_dev->path);
    }
  }

  int OpenFirstDualSenseController() {
    hid_device_info* devs = hid_enumerate(0x054c, 0x0ce6);
    print_devices_with_descriptor(devs);
    hid_free_enumeration(devs);

    // Open the device using the VID, PID, and optionally the Serial number.
    handle_ = hid_open(0x054c, 0x0ce6, nullptr);
    if (!handle_) {
      spdlog::info("unable to open device");
      return 1;
    }

    return 0;
  }

  void FinalizeHid() const {
    hid_close(handle_);
    hid_exit();
  }

  void GetInputReport() const {
    while (true) {
      unsigned char buf[64];
      if (const int res = hid_read(handle_, buf, sizeof(buf)); res < 0) {
        spdlog::error("hid_read failed");
        break;
      }

      std::stringstream ss;
      ss << "HID Input Report: ";
      for (int i = 0; i < sizeof(buf); ++i) {
        ss << std::hex << static_cast<int>(buf[i]) << " ";
      }
      ss << std::dec;
      spdlog::info(ss.str());
    }
  }

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    if (const auto key = sdbus::MemberName("Adapter");
        changedProperties.contains(key)) {
      adapter_ = changedProperties.at(key).get<sdbus::ObjectPath>();
    }
    if (const auto key = sdbus::MemberName("Address");
        changedProperties.contains(key)) {
      address_ = changedProperties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("AddressType");
        changedProperties.contains(key)) {
      address_type_ = changedProperties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("Bonded");
        changedProperties.contains(key)) {
      bonded_ = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("Blocked");
        changedProperties.contains(key)) {
      blocked_ = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("Connected");
        changedProperties.contains(key)) {
      connected_ = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("LegacyPairing");
        changedProperties.contains(key)) {
      legacy_pairing_ = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("Paired");
        changedProperties.contains(key)) {
      paired_ = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("Modalias");
        changedProperties.contains(key)) {
      modalias_ = changedProperties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("Name");
        changedProperties.contains(key)) {
      name_ = changedProperties.at(key).get<std::string>();
    }
    if (const auto key = sdbus::MemberName("ServiceData");
        changedProperties.contains(key)) {
      service_data_ = changedProperties.at(key)
                          .get<std::map<std::string, sdbus::Variant>>();
    }
    if (const auto key = sdbus::MemberName("RSSI");
        changedProperties.contains(key)) {
      rssi_ = changedProperties.at(key).get<std::int16_t>();
      spdlog::debug("RSSI: {}", rssi_);
    }
    if (const auto key = sdbus::MemberName("ServicesResolved");
        changedProperties.contains(key)) {
      services_resolved_ = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("Trusted");
        changedProperties.contains(key)) {
      trusted_ = changedProperties.at(key).get<bool>();
    }
    if (const auto key = sdbus::MemberName("UUIDs");
        changedProperties.contains(key)) {
      uuids_ = changedProperties.at(key).get<std::vector<std::string>>();
    }

    if (bonded_ && connected_ && paired_ && !modalias_.empty() &&
        modalias_ == "usb:v054Cp0CE6d0100") {
      spdlog::info("Connected: {}", address_);
      IntializeHid();
      if (OpenFirstDualSenseController()) {
        return;
      }

      // Set up the command buffer.
      int res;
      unsigned char buf[256]{};
#define MAX_STR 255
      wchar_t wstr[MAX_STR]{};

      buf[0] = 0x01;
      buf[1] = 0x81;

      // Read the Manufacturer String
      wstr[0] = 0x0000;
      res = hid_get_manufacturer_string(handle_, wstr, MAX_STR);
      if (res < 0)
        printf("Unable to read manufacturer string\n");
      printf("Manufacturer String: %ls\n", wstr);

      // Read the Product String
      wstr[0] = 0x0000;
      res = hid_get_product_string(handle_, wstr, MAX_STR);
      if (res < 0)
        printf("Unable to read product string\n");
      printf("Product String: %ls\n", wstr);

      // Read the Serial Number String
      wstr[0] = 0x0000;
      res = hid_get_serial_number_string(handle_, wstr, MAX_STR);
      if (res < 0)
        printf("Unable to read serial number string\n");
      printf("Serial Number String: (%d) %ls", wstr[0], wstr);
      printf("\n");

      // Read Indexed String 1
      wstr[0] = 0x0000;
      res = hid_get_indexed_string(handle_, 1, wstr, MAX_STR);
      if (res < 0)
        printf("Unable to read indexed string 1\n");
      printf("Indexed String 1: %ls\n", wstr);

      // Set the hid_read() function to be non-blocking.
      hid_set_nonblocking(handle_, 1);

      // Try to read from the device. There shoud be no
      // data here, but execution should not block.
      res = hid_read(handle_, buf, 17);
    }
  }
};

#endif  // SRC_BLUEZ_DEVICE1_H
