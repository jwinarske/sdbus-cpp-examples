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

#include <atomic>
#include <thread>

#include <fcntl.h>
#include <sys/epoll.h>

#include "../hidraw.hpp"
#include "input_reader.h"

#include <unistd.h>

InputReader::InputReader(std::string device)
    : device_(std::move(device)), stop_flag_(false) {}

void InputReader::start() {
  spdlog::debug("InputReader start: {}", device_);
  stop_flag_ = false;
  read_input();
}

void InputReader::stop() {
  spdlog::debug("InputReader stop: {}", device_);
  stop_flag_ = true;
}

InputReader::~InputReader() {
  stop();
}

InputReader::Task InputReader::read_input() {
  spdlog::debug("hidraw device: {}", device_);

  const int fd = open(device_.c_str(), O_RDWR);

  while (true) {
    if (fd < 0) {
      spdlog::error("unable to open device");
      break;
    }

    // Raw Info
    hidraw_devinfo raw_dev_info{};
    if (const auto res = ioctl(fd, HIDIOCGRAWINFO, &raw_dev_info); res < 0) {
      spdlog::error("HIDIOCGRAWINFO");
      break;
    }
    spdlog::info("bustype: {}", Hidraw::bus_str(raw_dev_info.bustype));
    spdlog::info("Vendor ID: {:04X}", raw_dev_info.vendor);
    spdlog::info("Product ID: {:04X}", raw_dev_info.product);

    // Raw Name
    char buf[256]{};
    auto res = ioctl(fd, HIDIOCGRAWNAME(sizeof(buf)), buf);
    if (res < 0) {
      spdlog::error("HIDIOCGRAWNAME");
      break;
    }
    spdlog::info("HID Name: {}", buf);

    // Raw Physical Location
    res = ioctl(fd, HIDIOCGRAWPHYS(sizeof(buf)), buf);
    if (res < 0) {
      spdlog::error("HIDIOCGRAWPHYS");
      break;
    }
    spdlog::info("HID Physical Location: {}", buf);

    // Report Descriptor Size
    int desc_size = 0;
    res = ioctl(fd, HIDIOCGRDESCSIZE, &desc_size);
    if (res < 0) {
      spdlog::error("HIDIOCGRDESCSIZE");
      break;
    }
    spdlog::info("Report Descriptor Size: {}", desc_size);

    // Report Descriptor
    hidraw_report_descriptor rpt_desc{};
    rpt_desc.size = desc_size;
    res = ioctl(fd, HIDIOCGRDESC, &rpt_desc);
    if (res < 0) {
      spdlog::error("HIDIOCGRDESC");
      break;
    }

    std::ostringstream os;
    os << "Report Descriptor\n";
    os << CustomHexdump<400, false>(rpt_desc.value, rpt_desc.size);
    spdlog::info(os.str());

    // Get Features
    GetControllerCalibrationData(
        fd, hw_cal_data_);  // enables extended report for BT
    GetControllerMacAll(fd, controller_and_host_mac_);
    GetControllerVersion(fd, version_);

    while (!stop_flag_) {
      std::uint8_t buffer[sizeof(USBGetStateData)];
      ssize_t result = 0;
      if (result = read(fd, &buffer[0], sizeof(USBGetStateData)); result < 0) {
        spdlog::error("GetInputReport4 failed: {}", strerror(errno));
        break;
      }

      if (raw_dev_info.product == 0x0CE6) {
        auto report_id = buffer[0];
        if (report_id == 1) {
          const auto& input_report01 =
              reinterpret_cast<USBGetStateData&>(buffer);
          PrintControllerStateUsb(input_report01, hw_cal_data_);
        } else if (report_id == 49) {
          const auto& input_report31 = reinterpret_cast<ReportIn31&>(buffer);
          if (input_report31.Data.HasHID) {
            spdlog::info("[ReportIn31] Has HID");
            PrintControllerStateUsb(input_report31.Data.State.StateData,
                                    hw_cal_data_);
          } else if (input_report31.Data.HasMic) {
            spdlog::info("[ReportIn31] Has Microphone");
          }
        } else {
          spdlog::error("Unknown report id: {}", report_id);
        }
      }
    }
    break;
  }

  close(fd);
  stop();

  co_return;
}

int InputReader::GetControllerMacAll(const int fd,
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

int InputReader::GetControllerVersion(const int fd,
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

int InputReader::GetControllerCalibrationData(
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

std::string InputReader::dpad_to_string(const Direction dpad) {
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

std::string InputReader::power_state_to_string(const PowerState state) {
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

std::string InputReader::mute_light_to_string(const MuteLight state) {
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

std::string InputReader::light_brightness_to_string(
    const LightBrightness state) {
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

std::string InputReader::light_fade_animation_to_string(
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

void InputReader::PrintCalibrationData(
    HardwareCalibrationData const& hw_cal_data) {
  spdlog::info("HW Calibration Data");
  for (auto const& [abs_code, bias, sens_numer, sens_denom] :
       hw_cal_data.accel) {
    spdlog::info("\tAccel {}: bias: {}, sens_numer: {}, sens_denom: {}",
                 abs_code, bias, sens_numer, sens_denom);
  }
  for (auto const& [abs_code, bias, sens_numer, sens_denom] :
       hw_cal_data.gyro) {
    spdlog::info("\tGyro {}: bias: {}, sens_numer: {}, sens_denom: {}",
                 abs_code, bias, sens_numer, sens_denom);
  }
}

void InputReader::PrintControllerMacAll(
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

void InputReader::PrintControllerVersion(
    ReportFeatureInVersion const& version) {
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

void InputReader::PrintControllerStateUsb(
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

void InputReader::PrintControllerStateBt(BTSimpleGetStateData const& state) {
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
