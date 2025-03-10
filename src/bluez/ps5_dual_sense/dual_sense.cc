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

#include "dual_sense.h"

#include <poll.h>

#include "hidraw.hpp"

DualSense::DualSense(sdbus::IConnection& connection)
    : ProxyInterfaces(connection,
                      sdbus::ServiceName(INTERFACE_NAME),
                      sdbus::ObjectPath("/")),
      UdevMonitor({"hidraw"},
                  [&](const char* /* action */,
                      const char* /* dev_node */,
                      const char* sub_system) {
                    if (!get_bt_hidraw_devices()) {
                      if (!get_usb_hidraw_devices()) {
                        spdlog::warn("Dual Sense not present.");
                      }
                    }
                  }) {
  if (!get_bt_hidraw_devices()) {
    get_usb_hidraw_devices();
  }

  registerProxy();
  for (const auto& [object, interfacesAndProperties] : GetManagedObjects()) {
    onInterfacesAdded(object, interfacesAndProperties);
  }
}

DualSense::~DualSense() {
  stop();
  unregisterProxy();
}

void DualSense::onInterfacesAdded(
    const sdbus::ObjectPath& objectPath,
    const std::map<sdbus::InterfaceName,
                   std::map<sdbus::PropertyName, sdbus::Variant>>&
        interfacesAndProperties) {
  for (const auto& [interface, properties] : interfacesAndProperties) {
    if (interface == PROPERTIES_INTERFACE_NAME ||
        interface == INTROSPECTABLE_INTERFACE_NAME) {
      continue;
    }
    if (interface == org::bluez::Adapter1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(adapters_mutex_);
      if (!adapters_.contains(objectPath)) {
        auto adapter1 = std::make_unique<Adapter1>(
            getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
            objectPath, properties);
        adapters_[objectPath] = std::move(adapter1);
      }
    } else if (interface == org::bluez::Device1_proxy::INTERFACE_NAME) {
      auto mod_alias_key = sdbus::MemberName("Modalias");
      if (!properties.contains(mod_alias_key))
        continue;

      auto mod_alias = Device1::parse_modalias(
          properties.at(mod_alias_key).get<std::string>());
      if (mod_alias.has_value()) {
        spdlog::debug("VID: {}, PID: {}, DID: {}", mod_alias.value().vid,
                      mod_alias.value().pid, mod_alias.value().did);
        if (auto [vid, pid, did] = mod_alias.value();
            vid != VENDOR_ID || pid != PRODUCT_ID || did != DEVICE_ID) {
          continue;
        }
      } else {
        spdlog::debug("modalias has no value assigned: {}", objectPath);
        continue;
      }

      std::scoped_lock lock(devices_mutex_);
      if (!devices_.contains(objectPath)) {
        auto device = std::make_unique<Device1>(
            getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
            objectPath, properties);

        if (auto props = device->GetProperties(); props.modalias.has_value()) {
          auto [vid, pid, did] = props.modalias.value();
          spdlog::info("Adding: {}, {}, {}", vid, pid, did);
          if (vid == VENDOR_ID && pid == PRODUCT_ID && did == DEVICE_ID) {
            // if connected, paired, trusted, and bonded a hidraw device should
            // be ready to use
            if (props.bonded && props.connected && props.paired &&
                props.trusted) {
              const auto dev_key =
                  create_device_key_from_serial_number(props.address);
              hidraw_devices_mutex_.lock();
              Hidraw::dump_info(hidraw_devices_[dev_key]);
              hidraw_devices_mutex_.unlock();
            }

            // Add UPower Display Device
            if (std::string power_path = convert_mac_to_path(props.address);
                !upower_clients_.contains(power_path)) {
              upower_display_devices_mutex_.lock();
              spdlog::info("[Add] UPower Display Device: {}", power_path);
              upower_clients_[power_path] = std::make_unique<UPowerClient>(
                  getProxy().getConnection(), sdbus::ObjectPath(power_path));
              upower_display_devices_mutex_.unlock();
            }
          }
        }

        devices_[objectPath] = std::move(device);
      }
    } else if (interface == org::bluez::Input1_proxy::INTERFACE_NAME) {
      std::lock_guard lock(input1_mutex_);
      if (!input1_.contains(objectPath)) {
        input1_[objectPath] = std::make_unique<Input1>(
            getProxy().getConnection(),
            sdbus::ServiceName(org::bluez::Input1_proxy::INTERFACE_NAME),
            objectPath);
      }
    }
  }
}

void DualSense::onInterfacesRemoved(
    const sdbus::ObjectPath& objectPath,
    const std::vector<sdbus::InterfaceName>& interfaces) {
  for (const auto& interface : interfaces) {
    if (interface == org::bluez::Adapter1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(adapters_mutex_);
      if (!adapters_.contains(objectPath)) {
        if (adapters_.contains(objectPath)) {
          adapters_[objectPath].reset();
          adapters_.erase(objectPath);
        }
      }
    } else if (interface == org::bluez::Device1_proxy::INTERFACE_NAME) {
      std::scoped_lock devices_lock(devices_mutex_);
      if (!devices_.contains(objectPath)) {
        if (devices_.contains(objectPath)) {
          devices_[objectPath].reset();
          devices_.erase(objectPath);
        }
      }
    } else if (interface == org::bluez::Input1_proxy::INTERFACE_NAME) {
      std::lock_guard lock(input1_mutex_);
      if (!input1_.contains(objectPath)) {
        input1_[objectPath].reset();
        input1_.erase(objectPath);
      }
    }
  }
  for (auto it = interfaces.begin(); it != interfaces.end(); ++it) {
    std::scoped_lock lock(devices_mutex_);
    if (devices_.contains(objectPath)) {
      auto& device = devices_[objectPath];

      if (auto props = device->GetProperties(); props.modalias.has_value()) {
        auto [vid, pid, did] = props.modalias.value();
        spdlog::info("Removing: {}, {}, {}", vid, pid, did);
        if (vid == VENDOR_ID && pid == PRODUCT_ID && did == DEVICE_ID) {
          if (std::string power_path = convert_mac_to_path(props.address);
              upower_clients_.contains(power_path)) {
            std::scoped_lock power_lock(upower_display_devices_mutex_);
            spdlog::info("[Remove] UPower Display Device: {}", power_path);
            auto& power_device = upower_clients_[power_path];
            power_device.reset();
            upower_clients_.erase(power_path);
          }
        }
      }

      device.reset();
      devices_.erase(objectPath);
    }
  }
}

std::string DualSense::convert_mac_to_path(const std::string& mac_address) {
  std::string result =
      "/org/freedesktop/UPower/devices/battery_ps_controller_battery_";
  std::string converted_mac = mac_address;
  std::ranges::replace(converted_mac, ':', 'o');
  std::ranges::transform(converted_mac, converted_mac.begin(), ::tolower);
  result += converted_mac;
  return result;
}

bool DualSense::compare_subsystem_device_paths(const std::string& input_path,
                                               const std::string& hidraw_path) {
  const std::string prefix = "/sys/devices/";

  // Check if both paths start with the prefix
  if (input_path.compare(0, prefix.size(), prefix) != 0 ||
      hidraw_path.compare(0, prefix.size(), prefix) != 0) {
    return false;
  }

  // Extract the common part of the paths
  const std::string input_common = input_path.substr(
      prefix.size(), input_path.find("/input") - prefix.size());
  const std::string hidraw_common = hidraw_path.substr(
      prefix.size(), hidraw_path.find("/hidraw") - prefix.size());

  // Compare the common parts
  return input_common == hidraw_common;
}

std::string DualSense::create_device_key_from_serial_number(
    const std::string& serial_number) {
  std::string converted_serial = serial_number;

  // Remove leading and trailing `"`
  if (!converted_serial.empty() && converted_serial.front() == '"') {
    converted_serial.erase(0, 1);
  }
  if (!converted_serial.empty() && converted_serial.back() == '"') {
    converted_serial.pop_back();
  }

  // Convert to uppercase
  std::ranges::transform(converted_serial, converted_serial.begin(), ::toupper);

  // Replace ':' with '_'
  std::ranges::replace(converted_serial, ':', '_');

  const std::string key = "dev_" + converted_serial;
  return key;
}

bool DualSense::get_usb_hidraw_devices() {
  // Clear all contents of hidraw_devices_
  std::scoped_lock lock(hidraw_devices_mutex_);
  hidraw_devices_.clear();

  // Get input devices matching the specified properties
  const auto usb_input_devices =
      Hidraw::get_udev_properties("input", {{"ID_BUS", "usb"},
                                            {"ID_USB_VENDOR_ID", "054c"},
                                            {"ID_USB_MODEL_ID", "0ce6"},
                                            {"ID_USB_REVISION", "0100"},
                                            {"TAGS", ":seat:"}});

  if (usb_input_devices.empty()) {
    spdlog::warn("No matching USB input devices found.");
    return false;
  }

  // Get all hidraw devices
  const auto hidraw_devices = Hidraw::get_udev_properties("hidraw");

  for (const auto& [input_path, input_properties] : usb_input_devices) {
    for (const auto& [hidraw_path, hidraw_properties] : hidraw_devices) {
      if (compare_subsystem_device_paths(input_path, hidraw_path)) {
        if (hidraw_properties.contains("DEVNAME")) {
          const auto& hidraw_dev_name = hidraw_properties.at("DEVNAME");
          auto serial_number = input_properties.at("UNIQ");
          const auto dev_key =
              create_device_key_from_serial_number(serial_number);
          spdlog::info(
              "Associated input device path: {} with hidraw device: {}",
              input_path, hidraw_dev_name);
          hidraw_devices_[dev_key] = hidraw_dev_name;
        }
      }
    }
  }
  for (const auto& [key, value] : hidraw_devices_) {
    spdlog::debug("hidraw device: {} = {}", key, value);
    Hidraw::dump_info(value);
  }
  return true;
}

bool DualSense::get_bt_hidraw_devices() {
  // Clear all contents of hidraw_devices_
  std::scoped_lock lock(hidraw_devices_mutex_);
  hidraw_devices_.clear();

  // Get input devices matching the specified properties
  const auto bt_input_devices = Hidraw::get_udev_properties(
      "input", {{"ID_BUS", "bluetooth"},
                {"NAME", "\"DualSense Wireless Controller\""},
                {"PRODUCT", "5/54c/ce6/8100"}});

  if (bt_input_devices.empty()) {
    spdlog::warn("No matching BT input devices found.");
    return false;
  }

  // Get all hidraw devices
  const auto hidraw_devices = Hidraw::get_udev_properties("hidraw");

  for (const auto& [input_path, input_properties] : bt_input_devices) {
    for (const auto& [hidraw_path, hidraw_properties] : hidraw_devices) {
      if (compare_subsystem_device_paths(input_path, hidraw_path)) {
        if (hidraw_properties.contains("DEVNAME")) {
          const auto& hidraw_dev_name = hidraw_properties.at("DEVNAME");
          auto serial_number = input_properties.at("UNIQ");
          const auto dev_key =
              create_device_key_from_serial_number(serial_number);
          spdlog::info(
              "Associated input device path: {} with hidraw device: {}",
              input_path, hidraw_dev_name);
          hidraw_devices_[dev_key] = hidraw_dev_name;
        }
      }
    }
  }
  for (const auto& [key, value] : hidraw_devices_) {
    spdlog::debug("hidraw device: {} = {}", key, value);
    Hidraw::dump_info(value);
  }
  return !hidraw_devices_.empty();
}

int DualSense::GetControllerStateUsb(const int fd, ReportIn01USB& state_data) {
  state_data.ReportID = 0x01;

  // Check if data is available using poll

  pollfd pfd{};
  pfd.fd = fd;
  pfd.events = POLLIN;

  const int poll_res = poll(&pfd, 1, 0);
  if (poll_res == -1) {
    spdlog::error("poll failed: {}", strerror(errno));
    return 1;
  }

  // data not available, use epoll to block until data is available

  if (poll_res == 0) {
    spdlog::info("No data available to read");
    // Create an epoll instance
    const int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
      spdlog::error("Failed to create epoll instance: {}", strerror(errno));
      return 1;
    }

    // Block using epoll to wait for data to be available

    // Add the file descriptor to the epoll instance
    epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
      spdlog::error("Failed to add file descriptor to epoll: {}",
                    strerror(errno));
      close(epoll_fd);
      return 1;
    }

    // Wait for events
    epoll_event events[1];
    if (const int n = epoll_wait(epoll_fd, events, 1, -1); n == -1) {
      spdlog::error("epoll_wait failed: {}", strerror(errno));
      close(epoll_fd);
      return 1;
    }
    // Read data if available
    if (!(events[0].events & EPOLLIN)) {
      spdlog::error("epoll event EPOLLIN not set");
    }
    close(epoll_fd);
  }

  if (const auto result = read(fd, &state_data, sizeof(USBGetStateData));
      result < 0) {
    spdlog::error("GetControllerStateUsb failed: {}", strerror(errno));
    return 1;
  }

  if (state_data.ReportID != 0x01) {
    spdlog::error("GetControllerStateUsb invalid response");
    return 1;
  }

  return 0;
}

int DualSense::GetControllerStateBt(const int fd, ReportIn31& state_data) {
  // Check if data is available using poll

  pollfd pfd{};
  pfd.fd = fd;
  pfd.events = POLLIN;

  const int poll_res = poll(&pfd, 1, 0);
  if (poll_res == -1) {
    spdlog::error("poll failed: {}", strerror(errno));
    return 1;
  }

  // data not available, use epoll to block until data is available

  if (poll_res == 0) {
    spdlog::info("No data available to read");
    // Create an epoll instance
    const int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
      spdlog::error("Failed to create epoll instance: {}", strerror(errno));
      return 1;
    }

    // Add the file descriptor to the epoll instance
    epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
      spdlog::error("Failed to add file descriptor to epoll: {}",
                    strerror(errno));
      close(epoll_fd);
      return 1;
    }

    // Wait for events
    epoll_event events[1];
    if (const int n = epoll_wait(epoll_fd, events, 1, -1); n == -1) {
      spdlog::error("epoll_wait failed: {}", strerror(errno));
      close(epoll_fd);
      return 1;
    }

    // event happened
    if (!(events[0].events & EPOLLIN)) {
      spdlog::error("epoll event EPOLLIN not set");
    }
    close(epoll_fd);
  }

  if (const auto result = read(fd, &state_data, sizeof(ReportIn31));
      result < 0) {
    spdlog::error("GetControllerStateBt failed: {}", strerror(errno));
    return 1;
  }

  if (state_data.Data.ReportID != 0x31) {
    spdlog::error("GetControllerStateBt unexpected response");
    return 1;
  }

  return 0;
}

int DualSense::GetControllerMacAll(const int fd,
                                   ReportFeatureInMacAll& mac_all) {
  mac_all.ReportID = 0x09;
  if (const auto res =
          ioctl(fd, HIDIOCGFEATURE(sizeof(ReportFeatureInMacAll)), &mac_all);
      res < 0) {
    spdlog::error("GetControllerMacAll failed: {}", strerror(errno));
    return 1;
  }
  if (mac_all.ReportID != 0x09 || mac_all.Hard08 != 0x08 ||
      mac_all.Hard25 != 0x25 || mac_all.Hard00 != 0x00) {
    spdlog::error("GetControllerMacAll invalid response");
    return 1;
  }
  return 0;
}

int DualSense::GetControllerVersion(const int fd,
                                    ReportFeatureInVersion& version) {
  version.Data.ReportID = 0x20;
  if (const auto res =
          ioctl(fd, HIDIOCGFEATURE(sizeof(ReportFeatureInVersion)), &version);
      res < 0) {
    spdlog::error("GetControllerVersion failed: {}", strerror(errno));
    return 1;
  }
  if (version.Data.ReportID != 0x20) {
    spdlog::error("GetControllerVersion invalid response");
    return 1;
  }
  return 0;
}

int DualSense::GetControllerCalibrationData(
    const int fd,
    HardwareCalibrationData& hw_cal_data) {
  static constexpr auto ACC_RES_PER_G = 8192;
  static constexpr auto ACC_RANGE = (4 * ACC_RES_PER_G);
  static constexpr auto GYRO_RES_PER_DEG_S = 1024;
  static constexpr auto GYRO_RANGE = (2048 * GYRO_RES_PER_DEG_S);

  ReportFeatureCalibrationData cal_data{};
  cal_data.Data.ReportID = 0x05;
  if (const auto res = ioctl(
          fd, HIDIOCGFEATURE(sizeof(ReportFeatureCalibrationData)), &cal_data);
      res < 0) {
    spdlog::error("GetControllerCalibrationData failed: {}", strerror(errno));
    return 1;
  }
  if (cal_data.Data.ReportID != 0x05) {
    spdlog::error("GetControllerCalibrationData invalid response");
    return 1;
  }

  // Set gyroscope calibration and normalization parameters.
  const auto speed_2x =
      (cal_data.Data.gyro.speed_plus + cal_data.Data.gyro.speed_minus);
  hw_cal_data.gyro[0].abs_code = ABS_RX;
  hw_cal_data.gyro[0].bias = 0;
  hw_cal_data.gyro[0].sens_numer = speed_2x * GYRO_RES_PER_DEG_S;
  hw_cal_data.gyro[0].sens_denom =
      abs(cal_data.Data.gyro.pitch_plus - cal_data.Data.gyro.pitch_bias) +
      abs(cal_data.Data.gyro.pitch_minus - cal_data.Data.gyro.pitch_bias);

  hw_cal_data.gyro[1].abs_code = ABS_RY;
  hw_cal_data.gyro[1].bias = 0;
  hw_cal_data.gyro[1].sens_numer = speed_2x * GYRO_RES_PER_DEG_S;
  hw_cal_data.gyro[1].sens_denom =
      abs(cal_data.Data.gyro.yaw_plus - cal_data.Data.gyro.yaw_bias) +
      abs(cal_data.Data.gyro.yaw_minus - cal_data.Data.gyro.yaw_bias);

  hw_cal_data.gyro[2].abs_code = ABS_RZ;
  hw_cal_data.gyro[2].bias = 0;
  hw_cal_data.gyro[2].sens_numer = speed_2x * GYRO_RES_PER_DEG_S;
  hw_cal_data.gyro[2].sens_denom =
      abs(cal_data.Data.gyro.roll_plus - cal_data.Data.gyro.roll_bias) +
      abs(cal_data.Data.gyro.roll_minus - cal_data.Data.gyro.roll_bias);

  // Set accelerometer calibration and normalization parameters.
  auto range_2g = cal_data.Data.acc.x_plus - cal_data.Data.acc.x_minus;
  hw_cal_data.accel[0].abs_code = ABS_X;
  hw_cal_data.accel[0].bias =
      static_cast<std::int16_t>(cal_data.Data.acc.x_plus - range_2g / 2);
  hw_cal_data.accel[0].sens_numer = 2 * ACC_RES_PER_G;
  hw_cal_data.accel[0].sens_denom = range_2g;

  range_2g = cal_data.Data.acc.y_plus - cal_data.Data.acc.y_minus;
  hw_cal_data.accel[1].abs_code = ABS_Y;
  hw_cal_data.accel[1].bias =
      static_cast<std::int16_t>(cal_data.Data.acc.y_plus - range_2g / 2);
  hw_cal_data.accel[1].sens_numer = 2 * ACC_RES_PER_G;
  hw_cal_data.accel[1].sens_denom = range_2g;

  range_2g = cal_data.Data.acc.z_plus - cal_data.Data.acc.z_minus;
  hw_cal_data.accel[2].abs_code = ABS_Z;
  hw_cal_data.accel[2].bias =
      static_cast<std::int16_t>(cal_data.Data.acc.z_plus - range_2g / 2);
  hw_cal_data.accel[2].sens_numer = 2 * ACC_RES_PER_G;
  hw_cal_data.accel[2].sens_denom = range_2g;

  // Sanity check gyro calibration data
  size_t i = 0;
  for (auto& [abs_code, bias, sens_numer, sens_denom] : hw_cal_data.gyro) {
    if (sens_denom == 0) {
      abs_code = ABS_RX + i;
      spdlog::warn(
          "Invalid gyro calibration data for axis ({}), disabling calibration.",
          abs_code);
      bias = 0;
      sens_numer = GYRO_RANGE;
      sens_denom = INT16_MAX;
    }
    i++;
  }

  // Sanity check accelerometer calibration data
  i = 0;
  for (auto& [abs_code, bias, sens_numer, sens_denom] : hw_cal_data.accel) {
    if (sens_denom == 0) {
      abs_code = ABS_RX + i;
      spdlog::warn(
          "Invalid accelerometer calibration data for axis ({}), disabling "
          "calibration.",
          abs_code);
      bias = 0;
      sens_numer = ACC_RANGE;
      sens_denom = INT16_MAX;
    }
    i++;
  }

  return 0;
}

std::string DualSense::dpad_to_string(const Direction dpad) {
  switch (dpad) {
    case Direction::North:
      return "North";
    case Direction::NorthEast:
      return "NorthEast";
    case Direction::East:
      return "East";
    case Direction::SouthEast:
      return "SouthEast";
    case Direction::South:
      return "South";
    case Direction::SouthWest:
      return "SouthWest";
    case Direction::West:
      return "West";
    case Direction::NorthWest:
      return "NorthWest";
    default:
      return "None";
  }
}

std::string DualSense::power_state_to_string(const PowerState state) {
  switch (state) {
    case PowerState::Discharging:
      return "Discharging";
    case PowerState::Charging:
      return "Charging";
    case PowerState::Complete:
      return "Complete";
    case PowerState::AbnormalVoltage:
      return "AbnormalVoltage";
    case PowerState::AbnormalTemperature:
      return "AbnormalTemperature";
    case PowerState::ChargingError:
      return "ChargingError";
    default:
      return "Unknown";
  }
}

std::string DualSense::mute_light_to_string(const MuteLight state) {
  switch (state) {
    case MuteLight::Off:
      return "Off";
    case MuteLight::On:
      return "On";
    case MuteLight::Breathing:
      return "Breathing";
    case MuteLight::DoNothing:
      return "DoNothing";
    case MuteLight::NoAction4:
      return "NoAction4";
    case MuteLight::NoAction5:
      return "NoAction5";
    case MuteLight::NoAction6:
      return "NoAction6";
    case MuteLight::NoAction7:
      return "NoAction7";
    default:
      return "Unknown";
  }
}

std::string DualSense::light_brightness_to_string(const LightBrightness state) {
  switch (state) {
    case LightBrightness::Bright:
      return "Bright";
    case LightBrightness::Mid:
      return "Mid";
    case LightBrightness::Dim:
      return "Dim";
    case LightBrightness::NoAction3:
      return "NoAction3";
    case LightBrightness::NoAction4:
      return "NoAction4";
    case LightBrightness::NoAction5:
      return "NoAction5";
    case LightBrightness::NoAction6:
      return "NoAction6";
    case LightBrightness::NoAction7:
      return "NoAction7";
    default:
      return "Unknown";
  }
}

std::string DualSense::light_fade_animation_to_string(
    const LightFadeAnimation state) {
  switch (state) {
    case LightFadeAnimation::Nothing:
      return "Nothing";
    case LightFadeAnimation::FadeIn:
      return "FadeIn";
    case LightFadeAnimation::FadeOut:
      return "FadeOut";
    default:
      return "Unknown";
  }
}

void DualSense::PrintCalibrationData(
    HardwareCalibrationData const& hw_cal_data) {
  spdlog::info("HW Calibration Data");
  for (auto const& [abs_code, bias, sens_numer, sens_denom] :
       hw_cal_data.gyro) {
    spdlog::info("\tGyro {}: bias: {}, sens_numer: {}, sens_denom: {}",
                 abs_code, bias, sens_numer, sens_denom);
  }
  for (auto const& [abs_code, bias, sens_numer, sens_denom] :
       hw_cal_data.accel) {
    spdlog::info("\tAccelerometer {}: bias: {}, sens_numer: {}, sens_denom: {}",
                 abs_code, bias, sens_numer, sens_denom);
  }
}

void DualSense::PrintControllerMacAll(
    ReportFeatureInMacAll const& controller_and_host_mac) {
  spdlog::info("Controller Mac All");
  spdlog::info("\tReport ID: 0x{:02X}", controller_and_host_mac.ReportID);

  std::ostringstream os;
  os << "\tClient: ";
  for (int i = 6; i; --i) {
    os << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<int>(controller_and_host_mac.ClientMac[i]);
    if (i != 1) {
      os << ":";
    }
  }
  spdlog::info(os.str());
  os.clear();
  os.str("");
  os << "\tHost: ";
  for (int i = 6; i; --i) {
    os << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<int>(controller_and_host_mac.HostMac[i]);
    if (i != 1) {
      os << ":";
    }
  }
  spdlog::info(os.str());
}

void DualSense::PrintControllerVersion(ReportFeatureInVersion const& version) {
  spdlog::info("Firmware Info");
  spdlog::info("\tReportID: 0x{:02X}", version.Data.ReportID);
  spdlog::info("\tBuildDate: {}", std::string_view(version.Data.BuildDate, 11));
  spdlog::info("\tBuildTime: {}", std::string_view(version.Data.BuildTime, 8));
  spdlog::info("\tFwType: {}", version.Data.FwType);
  spdlog::info("\tSwSeries: 0x{:04X}", version.Data.SwSeries);
  spdlog::info("\tHardwareInfo: 0x{:08X}", version.Data.HardwareInfo);
  const uint32_t firmware_version = version.Data.FirmwareVersion;
  std::ostringstream firmware_version_str;
  firmware_version_str << std::hex << std::setw(2) << std::setfill('0')
                       << ((firmware_version >> 24) & 0xFF) << "."
                       << std::setw(2) << std::setfill('0')
                       << ((firmware_version >> 16) & 0xFF) << "."
                       << std::setw(4) << std::setfill('0')
                       << (firmware_version & 0xFFFF);
  spdlog::info("\tFirmwareVersion: {}", firmware_version_str.str());
  // spdlog::info("\tFirmwareVersion: 0x{:08X}", version.Data.FirmwareVersion);
  spdlog::info("\tDeviceInfo: {}", version.Data.DeviceInfo);
  spdlog::info("\tUpdateVersion: 0x{:04X}", version.Data.UpdateVersion);
  spdlog::info("\tUpdateImageInfo: 0x{:02}",
               static_cast<int>(version.Data.UpdateImageInfo));
  spdlog::info("\tUpdateUnk: 0x{:02}",
               static_cast<int>(version.Data.UpdateUnk));
  spdlog::info("\tSblFwVersion: 0x{:08X}", version.Data.SblFwVersion);
  spdlog::info("\tVenomFwVersion: 0x{:08X}", version.Data.VenomFwVersion);
  spdlog::info("\tSpiderDspFwVersion: 0x{:08X}",
               version.Data.SpiderDspFwVersion);
}

template <typename T, typename U, typename V>
T mult_frac(T x, U n, V d) {
  T q = x / d;
  T r = x % d;
  return q * n + r * n / d;
}

void DualSense::PrintControllerStateUsb(
    USBGetStateData const& state,
    HardwareCalibrationData const& hw_cal_data) {
  spdlog::info("Controller State (USB)");
  spdlog::info("\tLeftStick: {}, {}", state.LeftStickX, state.LeftStickY);
  spdlog::info("\tRightStick: {}, {}", state.RightStickX, state.RightStickY);
  spdlog::info("\tDPad: {}", dpad_to_string(state.DPad));
  spdlog::info("\tButtonSquare: {}", state.ButtonSquare);
  spdlog::info("\tButtonCross: {}", state.ButtonCross);
  spdlog::info("\tButtonCircle: {}", state.ButtonCircle);
  spdlog::info("\tButtonTriangle: {}", state.ButtonTriangle);
  spdlog::info("\tButtonL1: {}", state.ButtonL1);
  spdlog::info("\tButtonR1: {}", state.ButtonR1);
  spdlog::info("\tButtonL2: {}", state.ButtonL2);
  spdlog::info("\tButtonR2: {}", state.ButtonR2);
  spdlog::info("\tButtonCreate: {}", state.ButtonCreate);
  spdlog::info("\tButtonOptions: {}", state.ButtonOptions);
  spdlog::info("\tButtonL3: {}", state.ButtonL3);
  spdlog::info("\tButtonR3: {}", state.ButtonR3);
  spdlog::info("\tButtonHome: {}", state.ButtonHome);
  spdlog::info("\tButtonPad: {}", state.ButtonPad);
  spdlog::info("\tButtonMute: {}", state.ButtonMute);
  spdlog::info("\tButtonLeftFunction: {}", state.ButtonLeftFunction);
  spdlog::info("\tButtonRightFunction: {}", state.ButtonRightFunction);
  spdlog::info("\tButtonLeftPaddle: {}", state.ButtonLeftPaddle);
  spdlog::info("\tButtonRightPaddle: {}", state.ButtonRightPaddle);
  spdlog::info("\tTimeStamp: {}", state.TimeStamp);
  spdlog::info("\tAngularVelocity (Raw): {}, {}, {}", state.AngularVelocityX,
               state.AngularVelocityY, state.AngularVelocityZ);

  auto gyro_x = mult_frac<int32_t, int16_t, int32_t>(
      hw_cal_data.gyro[0].sens_numer, state.AngularVelocityX,
      hw_cal_data.gyro[0].sens_denom);
  auto gyro_y = mult_frac<int32_t, int16_t, int32_t>(
      hw_cal_data.gyro[1].sens_numer, state.AngularVelocityY,
      hw_cal_data.gyro[1].sens_denom);
  auto gyro_z = mult_frac<int32_t, int16_t, int32_t>(
      hw_cal_data.gyro[2].sens_numer, state.AngularVelocityZ,
      hw_cal_data.gyro[2].sens_denom);
  spdlog::info("\tAngularVelocity (Cal): {}, {}, {}", gyro_x, gyro_y, gyro_z);

  spdlog::info("\tAccelerometer (Raw): {}, {}, {}", state.AccelerometerX,
               state.AccelerometerY, state.AccelerometerZ);

  auto acc_x = mult_frac<int32_t, int16_t, int32_t>(
      hw_cal_data.accel[0].sens_numer, state.AccelerometerX,
      hw_cal_data.accel[0].sens_denom);
  auto acc_y = mult_frac<int32_t, int16_t, int32_t>(
      hw_cal_data.accel[1].sens_numer, state.AccelerometerY,
      hw_cal_data.accel[1].sens_denom);
  auto acc_z = mult_frac<int32_t, int16_t, int32_t>(
      hw_cal_data.accel[2].sens_numer, state.AccelerometerZ,
      hw_cal_data.accel[2].sens_denom);
  spdlog::info("\tAccelerometer (Cal): {}, {}, {}", acc_x, acc_y, acc_z);

  spdlog::info("\tSensorTimestamp: {}", state.SensorTimestamp);
  spdlog::info("\tTemperature: {}", state.Temperature);
  spdlog::info(
      "\tTouchData: timestamp: {}, index: {}, X: {}, Y: {}, NotTouching: {}",
      state.touchData.Timestamp, state.touchData.Finger[0].Index,
      state.touchData.Finger[0].FingerX, state.touchData.Finger[0].FingerY,
      state.touchData.Finger[0].NotTouching);
  spdlog::info(
      "\tTouchData: timestamp: {}, index: {}, X: {}, Y: {}, NotTouching: {}",
      state.touchData.Timestamp, state.touchData.Finger[1].Index,
      state.touchData.Finger[1].FingerX, state.touchData.Finger[1].FingerY,
      state.touchData.Finger[1].NotTouching);
  spdlog::info("\tTriggerRightStopLocation: {}",
               state.TriggerRightStopLocation);
  spdlog::info("\tTriggerRightStatus: {}", state.TriggerRightStatus);
  spdlog::info("\tTriggerLeftStopLocation: {}", state.TriggerLeftStopLocation);
  spdlog::info("\tTriggerLeftStatus: {}", state.TriggerLeftStatus);
  spdlog::info("\tTriggerRightEffect: {}", state.TriggerRightEffect);
  spdlog::info("\tTriggerLeftEffect: {}", state.TriggerLeftEffect);
  spdlog::info("\tPowerPercent: {}", state.PowerPercent);
  spdlog::info("\tPowerState: {}", power_state_to_string(state.powerState));
  spdlog::info("\tPluggedHeadphones: {}", state.PluggedHeadphones);
  spdlog::info("\tPluggedMic: {}", state.PluggedMic);
  spdlog::info("\tMicMuted: {}", state.MicMuted);
  spdlog::info("\tPluggedUsbData: {}", state.PluggedUsbData);
  spdlog::info("\tPluggedUsbPower: {}", state.PluggedUsbPower);
  spdlog::info("\tPluggedExternalMic: {}", state.PluggedExternalMic);
  spdlog::info("\tHapticLowPassFilter: {}", state.HapticLowPassFilter);
}

void DualSense::PrintControllerStateBt(BTSimpleGetStateData const& state) {
  spdlog::info("Controller State (BT)");
  spdlog::info("\tLeftStick: {}, {}", state.LeftStickX, state.LeftStickY);
  spdlog::info("\tRightStick: {}, {}", state.RightStickX, state.RightStickY);
  spdlog::info("\tDPad: {}", dpad_to_string(state.DPad));
  spdlog::info("\tButtonSquare: {}", state.ButtonSquare);
  spdlog::info("\tButtonCross: {}", state.ButtonCross);
  spdlog::info("\tButtonCircle: {}", state.ButtonCircle);
  spdlog::info("\tButtonTriangle: {}", state.ButtonTriangle);
  spdlog::info("\tButtonL1: {}", state.ButtonL1);
  spdlog::info("\tButtonR1: {}", state.ButtonR1);
  spdlog::info("\tButtonL2: {}", state.ButtonL2);
  spdlog::info("\tButtonR2: {}", state.ButtonR2);
  spdlog::info("\tButtonShare: {}", state.ButtonShare);
  spdlog::info("\tButtonOptions: {}", state.ButtonOptions);
  spdlog::info("\tButtonL3: {}", state.ButtonL3);
  spdlog::info("\tButtonR3: {}", state.ButtonR3);
  spdlog::info("\tButtonHome: {}", state.ButtonHome);
  spdlog::info("\tButtonPad: {}", state.ButtonPad);
  spdlog::info("\tCounter: {}", state.Counter);
  spdlog::info("\tTriggerLeft: {}", state.TriggerLeft);
  spdlog::info("\tTriggerRight: {}", state.TriggerRight);
}
