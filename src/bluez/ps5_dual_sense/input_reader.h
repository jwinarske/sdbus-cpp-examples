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

#ifndef SRC_BLUEZ_XBOX_CONTROLLER_INPUT_READER_HPP_
#define SRC_BLUEZ_XBOX_CONTROLLER_INPUT_READER_HPP_

#include <array>
#include <atomic>
#include <thread>

#include "../../utils/unique_fd.h"
#include "dual_sense_0ce6.h"

class InputReader {
 public:
  explicit InputReader(std::string device);

  void start();

  void stop();

  ~InputReader();

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
  std::atomic<bool> stop_flag_;
  // eventfd used to interrupt the blocking read loop immediately on stop().
  UniqueFd stop_event_fd_;

  ReportFeatureInMacAll controller_and_host_mac_{};
  ReportFeatureInVersion version_{};
  HardwareCalibrationData hw_cal_data_{};

  // Worker thread that owns the blocking read loop. Joined in the destructor
  // before any other member is torn down, so the loop can never outlive this
  // object (no use-after-free) and never blocks the D-Bus/main thread.
  std::thread thread_;

  void read_input();

  static std::string dpad_to_string(Direction dpad);
  static std::string power_state_to_string(PowerState state);
  static std::string mute_light_to_string(MuteLight state);
  static std::string light_brightness_to_string(LightBrightness state);
  static std::string light_fade_animation_to_string(LightFadeAnimation state);

  static int GetControllerMacAll(int fd, ReportFeatureInMacAll& mac_all);

  static int GetControllerVersion(int fd, ReportFeatureInVersion& version);

  static int GetControllerCalibrationData(int fd,
                                          HardwareCalibrationData& hw_cal_data);

  static void PrintCalibrationData(HardwareCalibrationData const& hw_cal_data);
  static void PrintControllerMacAll(
      ReportFeatureInMacAll const& controller_and_host_mac);
  static void PrintControllerVersion(ReportFeatureInVersion const& version);

  static void PrintControllerStateUsb(
      USBGetStateData const& state,
      HardwareCalibrationData const& hw_cal_data);
  static void PrintControllerStateBt(BTSimpleGetStateData const& state);
};

#endif  // SRC_BLUEZ_XBOX_CONTROLLER_INPUT_READER_HPP_
