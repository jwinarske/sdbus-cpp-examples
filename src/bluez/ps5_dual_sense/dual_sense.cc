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
            vid != kVendorId || pid != kProductId || did != kDeviceId) {
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
          if (vid == kVendorId && pid == kProductId && did == kDeviceId) {
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
        if (vid == kVendorId && pid == kProductId && did == kDeviceId) {
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

#if 0
int DualSense::GetControllerStateUsb(const int fd, ReportIn01USB& state_data) {
  state_data.ReportID = 0x01;

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
#endif
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

  if (const auto result = read(fd, &state_data, sizeof(ReportIn31));
      result < 0) {
    spdlog::error("GetControllerStateUsb failed: {}", strerror(errno));
    return 1;
  }

  if (state_data.Data.ReportID != 0x31) {
    spdlog::error("GetControllerStateUsb invalid response");
    return 1;
  }

  return 0;
}

int DualSense::GetControllerMacAll(
    const int fd,
    ReportFeatureInMacAll& controller_and_host_mac) {
  controller_and_host_mac.ReportID = 0x09;
  if (const auto res = ioctl(fd, HIDIOCGFEATURE(sizeof(ReportFeatureInMacAll)),
                             &controller_and_host_mac);
      res < 0) {
    spdlog::error("GetControllerMacAll failed: {}", strerror(errno));
    return 1;
  }
  if (controller_and_host_mac.ReportID != 0x09 ||
      controller_and_host_mac.Hard08 != 0x08 ||
      controller_and_host_mac.Hard25 != 0x25 ||
      controller_and_host_mac.Hard00 != 0x00) {
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

std::string DualSense::DpadToString(const Direction dpad) {
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

std::string DualSense::PowerStateToString(const PowerState state) {
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

std::string DualSense::MuteLightToString(const MuteLight state) {
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

std::string DualSense::LightBrightnessToString(const LightBrightness state) {
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

std::string DualSense::LightFadeAnimationToString(
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
  spdlog::info("\tFwType: 0x{:04X}", version.Data.FwType);
  spdlog::info("\tSwSeries: 0x{:04X}", version.Data.SwSeries);
  spdlog::info("\tHardwareInfo: 0x{:08X}", version.Data.HardwareInfo);
  spdlog::info("\tFirmwareVersion: 0x{:08X}", version.Data.FirmwareVersion);
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

void DualSense::PrintControllerStateUsb(USBGetStateData const& state) {
  spdlog::info("LeftStick: {}, {}", state.LeftStickX, state.LeftStickY);
  spdlog::info("RightStick: {}, {}", state.RightStickX, state.RightStickY);
  spdlog::info("DPad: {}", DpadToString(state.DPad));
  spdlog::info("ButtonSquare: {}", state.ButtonSquare);
  spdlog::info("ButtonCross: {}", state.ButtonCross);
  spdlog::info("ButtonCircle: {}", state.ButtonCircle);
  spdlog::info("ButtonTriangle: {}", state.ButtonTriangle);
  spdlog::info("ButtonL1: {}", state.ButtonL1);
  spdlog::info("ButtonR1: {}", state.ButtonR1);
  spdlog::info("ButtonL2: {}", state.ButtonL2);
  spdlog::info("ButtonR2: {}", state.ButtonR2);
  spdlog::info("ButtonCreate: {}", state.ButtonCreate);
  spdlog::info("ButtonOptions: {}", state.ButtonOptions);
  spdlog::info("ButtonL3: {}", state.ButtonL3);
  spdlog::info("ButtonR3: {}", state.ButtonR3);
  spdlog::info("ButtonHome: {}", state.ButtonHome);
  spdlog::info("ButtonPad: {}", state.ButtonPad);
  spdlog::info("ButtonMute: {}", state.ButtonMute);
  spdlog::info("ButtonLeftFunction: {}", state.ButtonLeftFunction);
  spdlog::info("ButtonRightFunction: {}", state.ButtonRightFunction);
  spdlog::info("ButtonLeftPaddle: {}", state.ButtonLeftPaddle);
  spdlog::info("ButtonRightPaddle: {}", state.ButtonRightPaddle);
  spdlog::info("TriggerRightStopLocation: {}", state.TriggerRightStopLocation);
  spdlog::info("TriggerRightStatus: {}", state.TriggerRightStatus);
  spdlog::info("TriggerLeftStopLocation: {}", state.TriggerLeftStopLocation);
  spdlog::info("TriggerLeftStatus: {}", state.TriggerLeftStatus);
  spdlog::info("TriggerRightEffect: {}", state.TriggerRightEffect);
  spdlog::info("TriggerLeftEffect: {}", state.TriggerLeftEffect);
  spdlog::info("PowerPercent: {}", state.PowerPercent);
  spdlog::info("PowerState: {}", PowerStateToString(state.powerState));
  spdlog::info("PluggedHeadphones: {}", state.PluggedHeadphones);
  spdlog::info("PluggedMic: {}", state.PluggedMic);
  spdlog::info("MicMuted: {}", state.MicMuted);
  spdlog::info("PluggedUsbData: {}", state.PluggedUsbData);
  spdlog::info("PluggedUsbPower: {}", state.PluggedUsbPower);
  spdlog::info("PluggedExternalMic: {}", state.PluggedExternalMic);
  spdlog::info("HapticLowPassFilter: {}", state.HapticLowPassFilter);
}

void DualSense::PrintControllerStateBt(BTSimpleGetStateData const& state) {
  spdlog::info("LeftStick: {}, {}", state.LeftStickX, state.LeftStickY);
  spdlog::info("RightStick: {}, {}", state.RightStickX, state.RightStickY);
  spdlog::info("DPad: {}", DpadToString(state.DPad));
  spdlog::info("ButtonSquare: {}", state.ButtonSquare);
  spdlog::info("ButtonCross: {}", state.ButtonCross);
  spdlog::info("ButtonCircle: {}", state.ButtonCircle);
  spdlog::info("ButtonTriangle: {}", state.ButtonTriangle);
  spdlog::info("ButtonL1: {}", state.ButtonL1);
  spdlog::info("ButtonR1: {}", state.ButtonR1);
  spdlog::info("ButtonL2: {}", state.ButtonL2);
  spdlog::info("ButtonR2: {}", state.ButtonR2);
  spdlog::info("ButtonShare: {}", state.ButtonShare);
  spdlog::info("ButtonOptions: {}", state.ButtonOptions);
  spdlog::info("ButtonL3: {}", state.ButtonL3);
  spdlog::info("ButtonR3: {}", state.ButtonR3);
  spdlog::info("ButtonHome: {}", state.ButtonHome);
  spdlog::info("ButtonPad: {}", state.ButtonPad);
  spdlog::info("Counter: {}", state.Counter);
  spdlog::info("TriggerLeft: {}", state.TriggerLeft);
  spdlog::info("TriggerRight: {}", state.TriggerRight);
}
