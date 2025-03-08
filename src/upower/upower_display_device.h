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
  struct Properties {
    std::uint32_t battery_level;
    double capacity;
    std::int32_t charge_cycles;
    std::uint32_t charge_end_threshold;
    std::uint32_t charge_start_threshold;
    bool charge_threshold_enabled;
    bool charge_threshold_supported;
    double energy;
    double energy_empty;
    double energy_full;
    double energy_full_design;
    double energy_rate;
    bool has_history;
    bool has_statistics;
    std::string icon_name;
    bool is_present;
    bool is_rechargeable;
    double luminosity;
    std::string model;
    std::string native_path;
    bool online;
    double percentage;
    bool power_supply;
    std::string serial;
    std::uint32_t state;
    std::uint32_t technology;
    double temperature;
    std::int64_t time_to_empty;
    std::int64_t time_to_full;
    std::uint32_t type;
    std::uint64_t update_time;
    std::string vendor;
    double voltage;
    std::uint32_t warning_level;
  };

  explicit UPowerDisplayDevice(sdbus::IConnection& connection,
                               const sdbus::ObjectPath& objectPath);

  virtual ~UPowerDisplayDevice();

  [[nodiscard]] const Properties& GetProperties() const { return properties_; }

 private:
  sdbus::ObjectPath object_path_;
  Properties properties_{};

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;
};

#endif  // SRC_UPOWER_UPOWER_DISPLAY_DEVICE_H
