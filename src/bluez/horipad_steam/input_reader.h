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
#include <thread>

#include "../../utils/unique_fd.h"
#include "horipad_stream_01ab_0196.h"

class InputReader {
 public:
  enum class Direction : uint8_t {
    North = 0,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest,
    None,
  };

  explicit InputReader(std::string device);

  void start();

  void stop();

  ~InputReader();

 private:
  std::string device_;
  std::atomic<bool> stop_flag_;
  // eventfd used to interrupt the blocking read loop immediately on stop().
  UniqueFd stop_event_fd_;
  // Worker thread that owns the blocking read loop. Joined in the destructor
  // before any other member is torn down, so the loop can never outlive this
  // object (no use-after-free) and never blocks the D-Bus/main thread.
  std::thread thread_;

  void read_input();

  static std::string dpad_to_string(Direction dpad);

  static void PrintInputReport7(const inputReport07_t& input_report07);

  static void PrintInputReport10(const inputReport10_t& input_report10);

  static void PrintInputReport12(const inputReport12_t& input_report12);

  static void PrintInputReport14(const inputReport14_t& input_report14);
};

#endif  // SRC_BLUEZ_XBOX_CONTROLLER_INPUT_READER_HPP_
