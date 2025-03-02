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

#include "upower_display_device.h"

#include <chrono>
#include <iomanip>

#include "../utils/utils.h"

UPowerDisplayDevice::UPowerDisplayDevice(sdbus::IConnection& connection,
                                         const sdbus::ObjectPath& objectPath)
    : ProxyInterfaces(connection,
                      sdbus::ServiceName("org.freedesktop.UPower"),
                      objectPath),
      object_path_(objectPath) {
  registerProxy();
  const auto properties = this->GetAll("org.freedesktop.UPower.Device");
  UPowerDisplayDevice::onPropertiesChanged(
      sdbus::InterfaceName("org.freedesktop.UPower.Device"), properties, {});
}

UPowerDisplayDevice::~UPowerDisplayDevice() {
  unregisterProxy();
}

void UPowerDisplayDevice::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  if (const auto key = sdbus::MemberName("BatteryLevel");
      changedProperties.contains(key)) {
    battery_level_ = changedProperties.at(key).get<std::uint32_t>();
  }
  if (const auto key = sdbus::MemberName("Capacity");
      changedProperties.contains(key)) {
    capacity_ = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("ChargeCycles");
      changedProperties.contains(key)) {
    charge_cycles_ = changedProperties.at(key).get<int>();
  }
  if (const auto key = sdbus::MemberName("ChargeEndThreshold");
      changedProperties.contains(key)) {
    charge_end_threshold_ = changedProperties.at(key).get<std::uint32_t>();
  }
  if (const auto key = sdbus::MemberName("ChargeStartThreshold");
      changedProperties.contains(key)) {
    charge_start_threshold_ = changedProperties.at(key).get<std::uint32_t>();
  }
  if (const auto key = sdbus::MemberName("ChargeThresholdEnabled");
      changedProperties.contains(key)) {
    charge_threshold_enabled_ = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("ChargeThresholdSupported");
      changedProperties.contains(key)) {
    charge_threshold_supported_ = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("Energy");
      changedProperties.contains(key)) {
    energy_ = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("EnergyEmpty");
      changedProperties.contains(key)) {
    energy_empty_ = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("EnergyFull");
      changedProperties.contains(key)) {
    energy_full_ = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("EnergyFullDesign");
      changedProperties.contains(key)) {
    energy_full_design_ = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("EnergyRate");
      changedProperties.contains(key)) {
    energy_rate_ = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("HasHistory");
      changedProperties.contains(key)) {
    has_history_ = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("HasStatistics");
      changedProperties.contains(key)) {
    has_statistics_ = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("IconName");
      changedProperties.contains(key)) {
    icon_name_ = changedProperties.at(key).get<std::string>();
  }
  if (const auto key = sdbus::MemberName("IsPresent");
      changedProperties.contains(key)) {
    is_present_ = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("IsRechargeable");
      changedProperties.contains(key)) {
    is_rechargeable_ = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("Luminosity");
      changedProperties.contains(key)) {
    luminosity_ = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("Model");
      changedProperties.contains(key)) {
    model_ = changedProperties.at(key).get<std::string>();
  }
  if (const auto key = sdbus::MemberName("NativePath");
      changedProperties.contains(key)) {
    native_path_ = changedProperties.at(key).get<std::string>();
  }
  if (const auto key = sdbus::MemberName("Online");
      changedProperties.contains(key)) {
    online_ = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("Percentage");
      changedProperties.contains(key)) {
    percentage_ = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("PowerSupply");
      changedProperties.contains(key)) {
    power_supply_ = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("Serial");
      changedProperties.contains(key)) {
    serial_ = changedProperties.at(key).get<std::string>();
  }
  if (const auto key = sdbus::MemberName("State");
      changedProperties.contains(key)) {
    state_ = changedProperties.at(key).get<std::uint32_t>();
  }
  if (const auto key = sdbus::MemberName("Technology");
      changedProperties.contains(key)) {
    technology_ = changedProperties.at(key).get<std::uint32_t>();
  }
  if (const auto key = sdbus::MemberName("Temperature");
      changedProperties.contains(key)) {
    temperature_ = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("TimeToEmpty");
      changedProperties.contains(key)) {
    time_to_empty_ = changedProperties.at(key).get<std::int64_t>();
  }
  if (const auto key = sdbus::MemberName("TimeToFull");
      changedProperties.contains(key)) {
    time_to_full_ = changedProperties.at(key).get<std::int64_t>();
  }
  if (const auto key = sdbus::MemberName("Type");
      changedProperties.contains(key)) {
    type_ = changedProperties.at(key).get<std::uint32_t>();
  }
  if (const auto key = sdbus::MemberName("UpdateTime");
      changedProperties.contains(key)) {
    update_time_ = changedProperties.at(key).get<std::uint64_t>();
  }
  if (const auto key = sdbus::MemberName("Vendor");
      changedProperties.contains(key)) {
    vendor_ = changedProperties.at(key).get<std::string>();
  }
  if (const auto key = sdbus::MemberName("Voltage");
      changedProperties.contains(key)) {
    voltage_ = changedProperties.at(key).get<double>();
  }

  if (const auto key = sdbus::MemberName("WarningLevel");
      changedProperties.contains(key)) {
    warning_level_ = changedProperties.at(key).get<std::uint32_t>();
  }
  spdlog::info("UPowerDisplayDevice: {}", object_path_);
  Utils::print_changed_properties(interfaceName, changedProperties,
                                  invalidatedProperties);
}
