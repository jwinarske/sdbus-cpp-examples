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

#ifndef SRC_BLUEZ_PS5_DUAL_SENSE_INPUT_READER_HPP_
#define SRC_BLUEZ_PS5_DUAL_SENSE_INPUT_READER_HPP_

#include <array>
#include <cstdint>
#include <expected>
#include <string>
#include <system_error>

#include "../../utils/event_loop.h"
#include "../../utils/unique_fd.h"
#include "dual_sense_0ce6.h"

/// Reads and decodes hidraw input reports for a PS5 DualSense controller as an
/// EventSource. The device is opened and its feature reports fetched in the
/// constructor; dispatch() then reads and prints one input report per readable
/// event on the loop thread. Register it with the loop only when valid().
class InputReader final : public EventSource {
 public:
  explicit InputReader(std::string device);
  ~InputReader() override = default;

  [[nodiscard]] bool valid() const { return fd_.valid(); }

  [[nodiscard]] int fd() const override { return fd_.get(); }
  void dispatch(short revents) override;

 private:
  struct CalibrationData {
    std::int32_t abs_code;
    std::int16_t bias;
    std::int32_t sens_numer;
    std::int32_t sens_denom;
  };

  struct HardwareCalibrationData {
    std::array<CalibrationData, 3> gyro;
    std::array<CalibrationData, 3> accel;
  };

  std::string device_;
  UniqueFd fd_;
  std::uint16_t product_ = 0;

  ReportFeatureInMacAll controller_and_host_mac_{};
  ReportFeatureInVersion version_{};
  HardwareCalibrationData hw_cal_data_{};

  // Opens the device, reads its descriptors and feature reports; leaves fd_
  // invalid on failure.
  void open_and_init();

  static std::string dpad_to_string(Direction dpad);
  static std::string power_state_to_string(PowerState state);
  static std::string mute_light_to_string(MuteLight state);
  static std::string light_brightness_to_string(LightBrightness state);
  static std::string light_fade_animation_to_string(LightFadeAnimation state);

  [[nodiscard]] static std::expected<void, std::error_code> GetControllerMacAll(
      int fd,
      ReportFeatureInMacAll& mac_all);

  [[nodiscard]] static std::expected<void, std::error_code>
  GetControllerVersion(int fd, ReportFeatureInVersion& version);

  [[nodiscard]] static std::expected<void, std::error_code>
  GetControllerCalibrationData(int fd, HardwareCalibrationData& hw_cal_data);

  static void PrintCalibrationData(HardwareCalibrationData const& hw_cal_data);
  static void PrintControllerMacAll(
      ReportFeatureInMacAll const& controller_and_host_mac);
  static void PrintControllerVersion(ReportFeatureInVersion const& version);

  static void PrintControllerStateUsb(
      USBGetStateData const& state,
      HardwareCalibrationData const& hw_cal_data);
  static void PrintControllerStateBt(BTSimpleGetStateData const& state);
};

#endif  // SRC_BLUEZ_PS5_DUAL_SENSE_INPUT_READER_HPP_
