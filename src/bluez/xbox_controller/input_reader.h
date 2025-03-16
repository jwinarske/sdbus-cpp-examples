#ifndef SRC_BLUEZ_XBOX_CONTROLLER_INPUT_READER_HPP_
#define SRC_BLUEZ_XBOX_CONTROLLER_INPUT_READER_HPP_

#include <atomic>
#include <coroutine>
#include <iostream>

#include "hidraw.hpp"
#include "xbox_wireless_controller_02fd.h"

class InputReader {
 public:
  enum class Direction : uint8_t {
    None = 0,
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest
  };

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

  std::string device_;
  std::atomic<bool> stop_flag_;

  Task read_input();

  static int GetInputReport(int fd, std::uint8_t* report, std::size_t length);

  static void PrintInputReport1(const inputReport01_t& input_report01);

  static void PrintInputReport2(const inputReport02_t& input_report02);

  static void PrintOutputReport3(const outputReport03_t& output_report03);

  static void PrintInputReport4(const inputReport04_t& output_report04);

  static const char* bus_str(std::uint32_t bus);

  static std::string dpad_to_string(Direction dpad);
};

#endif  // SRC_BLUEZ_XBOX_CONTROLLER_INPUT_READER_HPP_
