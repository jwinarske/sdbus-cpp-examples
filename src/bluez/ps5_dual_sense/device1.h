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

#include <optional>
#include <regex>

#include "../../proxy/org/bluez/Device1/device1_proxy.h"
#include "hidraw.hpp"

#include "../../upower/upower_client.h"

class Device1 final : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
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

  virtual ~Device1() { unregisterProxy(); }

 private:
  struct Modalias {
    std::string vid;
    std::string pid;
    std::string did;
  };

  sdbus::ObjectPath adapter_;
  std::string address_;
  std::string address_type_;
  std::string alias_;
  bool blocked_{};
  bool bonded_{};
  bool connected_{};
  bool legacy_pairing_{};
  std::optional<Modalias> modalias_;
  std::string name_;
  bool paired_{};
  std::map<std::uint16_t, sdbus::Variant> manufacturer_data_;
  std::map<std::string, sdbus::Variant> service_data_;
  std::int16_t rssi_{};
  bool services_resolved_{};
  bool trusted_{};
  std::vector<std::string> uuids_;

  std::unique_ptr<UPowerDisplayDevice> upower_display_device_;

  static std::string convert_mac_to_path(const std::string& mac_address) {
    std::string result =
        "/org/freedesktop/UPower/devices/battery_ps_controller_battery_";
    std::string converted_mac = mac_address;
    std::ranges::replace(converted_mac, ':', 'o');
    std::ranges::transform(converted_mac, converted_mac.begin(), ::tolower);
    result += converted_mac;
    return result;
  }

  static std::optional<Modalias> parse_modalias(const std::string& modalias) {
    const std::regex re(
        R"(usb:v([0-9A-Fa-f]{4})p([0-9A-Fa-f]{4})d([0-9A-Fa-f]{4}))");
    if (std::smatch match;
        std::regex_search(modalias, match, re) && match.size() == 4) {
      return Modalias{match[1].str(), match[2].str(), match[3].str()};
    }
    spdlog::error("Failed to parse modalias");
    return {};
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
      modalias_ = parse_modalias(changedProperties.at(key).get<std::string>());
      if (modalias_.has_value()) {
        spdlog::info("VendorID: {}", modalias_.value().vid);
        spdlog::info("ProductID: {}", modalias_.value().pid);
        spdlog::info("DeviceID: {}", modalias_.value().did);
      }
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

    if (bonded_ && connected_ && paired_ && modalias_.has_value() &&
        modalias_.value().vid == "054C" && modalias_.value().pid == "0CE6" &&
        modalias_.value().did == "0100") {
      spdlog::info("Connected: {}", address_);

      std::string upower_path = convert_mac_to_path(address_);
      spdlog::info("UPower Display Device: {}", upower_path);
      upower_display_device_ = std::make_unique<UPowerDisplayDevice>(
          getProxy().getConnection(), sdbus::ObjectPath(upower_path));

      for (auto devices =
               Hidraw::get_hidraw_devices(BUS_BLUETOOTH, "054C", "0CE6");
           const auto& device : devices) {
        Hidraw::dump_info(device);
      }
    }
  }
};

#endif  // SRC_BLUEZ_DEVICE1_H
