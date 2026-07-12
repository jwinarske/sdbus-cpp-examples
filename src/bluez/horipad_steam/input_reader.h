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

#ifndef SRC_BLUEZ_HORIPAD_STEAM_INPUT_READER_HPP_
#define SRC_BLUEZ_HORIPAD_STEAM_INPUT_READER_HPP_

#include <cstdint>
#include <string>

#include "../../utils/event_loop.h"
#include "../../utils/unique_fd.h"
#include "horipad_stream_01ab_0196.h"

/// Reads and decodes hidraw input reports for a HoriPad Steam controller as an
/// EventSource: the hidraw fd is polled by the EventLoop and dispatch() reads
/// and prints one report per readable event. Register it with the loop only
/// when valid().
class InputReader final : public EventSource {
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
  ~InputReader() override = default;

  [[nodiscard]] bool valid() const { return fd_.valid(); }

  [[nodiscard]] int fd() const override { return fd_.get(); }
  void dispatch(short revents) override;

 private:
  std::string device_;
  UniqueFd fd_;
  std::uint16_t product_ = 0;

  // Opens the device and reads its descriptor; leaves fd_ invalid on failure.
  void open_and_init();

  static std::string dpad_to_string(Direction dpad);

  static void PrintInputReport7(const inputReport07_t& input_report07);

  static void PrintInputReport10(const inputReport10_t& input_report10);

  static void PrintInputReport12(const inputReport12_t& input_report12);

  static void PrintInputReport14(const inputReport14_t& input_report14);
};

#endif  // SRC_BLUEZ_HORIPAD_STEAM_INPUT_READER_HPP_
