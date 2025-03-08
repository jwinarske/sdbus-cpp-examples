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

#include "../utils/utils.h"

UPowerDisplayDevice::UPowerDisplayDevice(sdbus::IConnection& connection,
                                         const sdbus::ObjectPath& objectPath)
    : ProxyInterfaces(connection,
                      sdbus::ServiceName("org.freedesktop.UPower"),
                      objectPath),
      object_path_(objectPath) {
  registerProxy();
  try {
    const auto properties = this->GetAll("org.freedesktop.UPower.Device");
    UPowerDisplayDevice::onPropertiesChanged(
        sdbus::InterfaceName("org.freedesktop.UPower.Device"), properties, {});
  } catch (const sdbus::Error& e) {
    spdlog::error("UPowerDisplayDevice::UPowerDisplayDevice: {}", e.what());
  }
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
    properties_.battery_level = changedProperties.at(key).get<std::uint32_t>();
  }
  if (const auto key = sdbus::MemberName("Capacity");
      changedProperties.contains(key)) {
    properties_.capacity = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("ChargeCycles");
      changedProperties.contains(key)) {
    properties_.charge_cycles = changedProperties.at(key).get<int>();
  }
  if (const auto key = sdbus::MemberName("ChargeEndThreshold");
      changedProperties.contains(key)) {
    properties_.charge_end_threshold =
        changedProperties.at(key).get<std::uint32_t>();
  }
  if (const auto key = sdbus::MemberName("ChargeStartThreshold");
      changedProperties.contains(key)) {
    properties_.charge_start_threshold =
        changedProperties.at(key).get<std::uint32_t>();
  }
  if (const auto key = sdbus::MemberName("ChargeThresholdEnabled");
      changedProperties.contains(key)) {
    properties_.charge_threshold_enabled =
        changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("ChargeThresholdSupported");
      changedProperties.contains(key)) {
    properties_.charge_threshold_supported =
        changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("Energy");
      changedProperties.contains(key)) {
    properties_.energy = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("EnergyEmpty");
      changedProperties.contains(key)) {
    properties_.energy_empty = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("EnergyFull");
      changedProperties.contains(key)) {
    properties_.energy_full = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("EnergyFullDesign");
      changedProperties.contains(key)) {
    properties_.energy_full_design = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("EnergyRate");
      changedProperties.contains(key)) {
    properties_.energy_rate = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("HasHistory");
      changedProperties.contains(key)) {
    properties_.has_history = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("HasStatistics");
      changedProperties.contains(key)) {
    properties_.has_statistics = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("IconName");
      changedProperties.contains(key)) {
    properties_.icon_name = changedProperties.at(key).get<std::string>();
  }
  if (const auto key = sdbus::MemberName("IsPresent");
      changedProperties.contains(key)) {
    properties_.is_present = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("IsRechargeable");
      changedProperties.contains(key)) {
    properties_.is_rechargeable = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("Luminosity");
      changedProperties.contains(key)) {
    properties_.luminosity = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("Model");
      changedProperties.contains(key)) {
    properties_.model = changedProperties.at(key).get<std::string>();
  }
  if (const auto key = sdbus::MemberName("NativePath");
      changedProperties.contains(key)) {
    properties_.native_path = changedProperties.at(key).get<std::string>();
  }
  if (const auto key = sdbus::MemberName("Online");
      changedProperties.contains(key)) {
    properties_.online = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("Percentage");
      changedProperties.contains(key)) {
    properties_.percentage = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("PowerSupply");
      changedProperties.contains(key)) {
    properties_.power_supply = changedProperties.at(key).get<bool>();
  }
  if (const auto key = sdbus::MemberName("Serial");
      changedProperties.contains(key)) {
    properties_.serial = changedProperties.at(key).get<std::string>();
  }
  if (const auto key = sdbus::MemberName("State");
      changedProperties.contains(key)) {
    properties_.state = changedProperties.at(key).get<std::uint32_t>();
  }
  if (const auto key = sdbus::MemberName("Technology");
      changedProperties.contains(key)) {
    properties_.technology = changedProperties.at(key).get<std::uint32_t>();
  }
  if (const auto key = sdbus::MemberName("Temperature");
      changedProperties.contains(key)) {
    properties_.temperature = changedProperties.at(key).get<double>();
  }
  if (const auto key = sdbus::MemberName("TimeToEmpty");
      changedProperties.contains(key)) {
    properties_.time_to_empty = changedProperties.at(key).get<std::int64_t>();
  }
  if (const auto key = sdbus::MemberName("TimeToFull");
      changedProperties.contains(key)) {
    properties_.time_to_full = changedProperties.at(key).get<std::int64_t>();
  }
  if (const auto key = sdbus::MemberName("Type");
      changedProperties.contains(key)) {
    properties_.type = changedProperties.at(key).get<std::uint32_t>();
  }
  if (const auto key = sdbus::MemberName("UpdateTime");
      changedProperties.contains(key)) {
    properties_.update_time = changedProperties.at(key).get<std::uint64_t>();
  }
  if (const auto key = sdbus::MemberName("Vendor");
      changedProperties.contains(key)) {
    properties_.vendor = changedProperties.at(key).get<std::string>();
  }
  if (const auto key = sdbus::MemberName("Voltage");
      changedProperties.contains(key)) {
    properties_.voltage = changedProperties.at(key).get<double>();
  }

  if (const auto key = sdbus::MemberName("WarningLevel");
      changedProperties.contains(key)) {
    properties_.warning_level = changedProperties.at(key).get<std::uint32_t>();
  }
  Utils::print_changed_properties(interfaceName, changedProperties,
                                  invalidatedProperties);
}
