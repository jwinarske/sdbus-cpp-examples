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

#include <atomic>
#include <coroutine>

#include "../hidraw.hpp"
#include "dual_sense_0ce6.h"

class InputReader {
 public:
  explicit InputReader(std::string device);

  void start();

  void stop();

  ~InputReader();

 private:
  struct Task {
    struct promise_type {
      static Task get_return_object() { return {}; }
      static std::suspend_never initial_suspend() { return {}; }
      static std::suspend_never final_suspend() noexcept { return {}; }
      static void return_void() {}
      static void unhandled_exception() { std::terminate(); }
    };
  };

  struct CalibrationData {
    std::int32_t abs_code;
    std::int16_t bias;
    std::int32_t sens_numer;
    std::int32_t sens_denom;
  };

  struct HardwareCalibrationData {
    CalibrationData gyro[3];
    CalibrationData accel[3];
  };

  std::string device_;
  std::atomic<bool> stop_flag_;

  ReportFeatureInMacAll controller_and_host_mac_{};
  ReportFeatureInVersion version_{};
  HardwareCalibrationData hw_cal_data_{};

  Task read_input();

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
