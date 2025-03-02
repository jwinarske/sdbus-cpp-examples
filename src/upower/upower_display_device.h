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

#ifndef SRC_UPOWER_UPOWER_DISPLAY_DEVICE_H
#define SRC_UPOWER_UPOWER_DISPLAY_DEVICE_H

#include "../proxy/org/freedesktop/UPower/DisplayDevice/display_device_proxy.h"

class UPowerDisplayDevice final
    : public sdbus::ProxyInterfaces<org::freedesktop::UPower::Device_proxy,
                                    sdbus::Properties_proxy> {
 public:
  explicit UPowerDisplayDevice(sdbus::IConnection& connection,
                               const sdbus::ObjectPath& objectPath);

  virtual ~UPowerDisplayDevice();

 private:
  sdbus::ObjectPath object_path_;
  std::uint32_t battery_level_{};
  double capacity_{};
  std::int32_t charge_cycles_{};
  std::uint32_t charge_end_threshold_{};
  std::uint32_t charge_start_threshold_{};
  bool charge_threshold_enabled_{};
  bool charge_threshold_supported_{};
  double energy_{};
  double energy_empty_{};
  double energy_full_{};
  double energy_full_design_{};
  double energy_rate_{};
  bool has_history_{};
  bool has_statistics_{};
  std::string icon_name_;
  bool is_present_{};
  bool is_rechargeable_{};
  double luminosity_{};
  std::string model_;
  std::string native_path_;
  bool online_{};
  double percentage_{};
  bool power_supply_{};
  std::string serial_;
  std::uint32_t state_{};
  std::uint32_t technology_{};
  double temperature_{};
  std::int64_t time_to_empty_{};
  std::int64_t time_to_full_{};
  std::uint32_t type_{};
  std::uint64_t update_time_{};
  std::string vendor_;
  double voltage_{};
  std::uint32_t warning_level_{};

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;
};

#endif  // SRC_UPOWER_UPOWER_DISPLAY_DEVICE_H
