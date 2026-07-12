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

#include "../utils/logging.h"
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
    LOG_ERROR("UPowerDisplayDevice::UPowerDisplayDevice: {}", e.what());
  }
}

UPowerDisplayDevice::~UPowerDisplayDevice() {
  unregisterProxy();
}

void UPowerDisplayDevice::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  // Iterate the changed properties once and dispatch on the key. A
  // PropertiesChanged signal usually carries only a handful of properties,
  // so this avoids scanning every known property (and the per-property map
  // lookup + MemberName allocation) on every signal.
  for (const auto& [key, value] : changedProperties) {
    if (key == "BatteryLevel") {
      properties_.battery_level = value.get<std::uint32_t>();
    } else if (key == "Capacity") {
      properties_.capacity = value.get<double>();
    } else if (key == "ChargeCycles") {
      properties_.charge_cycles = value.get<int>();
    } else if (key == "ChargeEndThreshold") {
      properties_.charge_end_threshold = value.get<std::uint32_t>();
    } else if (key == "ChargeStartThreshold") {
      properties_.charge_start_threshold = value.get<std::uint32_t>();
    } else if (key == "ChargeThresholdEnabled") {
      properties_.charge_threshold_enabled = value.get<bool>();
    } else if (key == "ChargeThresholdSupported") {
      properties_.charge_threshold_supported = value.get<bool>();
    } else if (key == "Energy") {
      properties_.energy = value.get<double>();
    } else if (key == "EnergyEmpty") {
      properties_.energy_empty = value.get<double>();
    } else if (key == "EnergyFull") {
      properties_.energy_full = value.get<double>();
    } else if (key == "EnergyFullDesign") {
      properties_.energy_full_design = value.get<double>();
    } else if (key == "EnergyRate") {
      properties_.energy_rate = value.get<double>();
    } else if (key == "HasHistory") {
      properties_.has_history = value.get<bool>();
    } else if (key == "HasStatistics") {
      properties_.has_statistics = value.get<bool>();
    } else if (key == "IconName") {
      properties_.icon_name = value.get<std::string>();
    } else if (key == "IsPresent") {
      properties_.is_present = value.get<bool>();
    } else if (key == "IsRechargeable") {
      properties_.is_rechargeable = value.get<bool>();
    } else if (key == "Luminosity") {
      properties_.luminosity = value.get<double>();
    } else if (key == "Model") {
      properties_.model = value.get<std::string>();
    } else if (key == "NativePath") {
      properties_.native_path = value.get<std::string>();
    } else if (key == "Online") {
      properties_.online = value.get<bool>();
    } else if (key == "Percentage") {
      properties_.percentage = value.get<double>();
    } else if (key == "PowerSupply") {
      properties_.power_supply = value.get<bool>();
    } else if (key == "Serial") {
      properties_.serial = value.get<std::string>();
    } else if (key == "State") {
      properties_.state = value.get<std::uint32_t>();
    } else if (key == "Technology") {
      properties_.technology = value.get<std::uint32_t>();
    } else if (key == "Temperature") {
      properties_.temperature = value.get<double>();
    } else if (key == "TimeToEmpty") {
      properties_.time_to_empty = value.get<std::int64_t>();
    } else if (key == "TimeToFull") {
      properties_.time_to_full = value.get<std::int64_t>();
    } else if (key == "Type") {
      properties_.type = value.get<std::uint32_t>();
    } else if (key == "UpdateTime") {
      properties_.update_time = value.get<std::uint64_t>();
    } else if (key == "Vendor") {
      properties_.vendor = value.get<std::string>();
    } else if (key == "Voltage") {
      properties_.voltage = value.get<double>();
    } else if (key == "WarningLevel") {
      properties_.warning_level = value.get<std::uint32_t>();
    }
  }
  Utils::print_changed_properties(interfaceName, changedProperties,
                                  invalidatedProperties);
}
