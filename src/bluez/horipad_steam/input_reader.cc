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
#include <unistd.h>

#include "../hidraw.hpp"
#include "input_reader.h"

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

// NOLINTNEXTLINE(readability-static-accessed-through-instance)
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

    while (!stop_flag_) {
      std::uint8_t buffer[sizeof(inputReport12_t)];
      ssize_t result = 0;
      if (result = read(fd, &buffer[0], sizeof(inputReport12_t)); result < 0) {
        spdlog::error("read failed: {}", strerror(errno));
        break;
      }

      if (raw_dev_info.product == 0x01ab || raw_dev_info.product == 0x0196) {
        if (auto report_id = buffer[0]; report_id == 7) {
          const auto* input_report07 =
              reinterpret_cast<inputReport07_t*>(buffer);
          PrintInputReport7(*input_report07);
        } else if (report_id == 10) {
          const auto* input_report10 =
              reinterpret_cast<inputReport10_t*>(buffer);
          PrintInputReport10(*input_report10);
        } else if (report_id == 12) {
          const auto* input_report12 =
              reinterpret_cast<inputReport12_t*>(buffer);
          PrintInputReport12(*input_report12);
        } else if (report_id == 14) {
          const auto* input_report14 =
              reinterpret_cast<inputReport14_t*>(buffer);
          PrintInputReport14(*input_report14);
        } else {
          spdlog::error("Unknown report id: {}", buffer[0]);
        }
      }
    }
    break;
  }

  close(fd);
  stop();

  co_return;  // NOLINT(readability-static-accessed-through-instance)
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

void InputReader::PrintInputReport7(const inputReport07_t& input_report07) {
  spdlog::info("Stick L/R: [{},{}] [{},{}] ", input_report07.GD_GamepadX,
               input_report07.GD_GamepadY, input_report07.GD_GamepadZ,
               input_report07.GD_GamepadRz);
  spdlog::info("L1/L2: {}, {}", input_report07.SIM_GamepadBrake,
               input_report07.SIM_GamepadAccelerator);
  spdlog::info("D-PAD: {}", dpad_to_string(static_cast<Direction>(
                                input_report07.GD_GamepadHatSwitch)));
  spdlog::info("A: {}", input_report07.BTN_GamepadButton1);
  spdlog::info("B: {}", input_report07.BTN_GamepadButton2);
  spdlog::info("Quick Access: {}", input_report07.BTN_GamepadButton3);
  spdlog::info("X: {}", input_report07.BTN_GamepadButton4);
  spdlog::info("Y: {}", input_report07.BTN_GamepadButton5);
  spdlog::info("M1: {}", input_report07.BTN_GamepadButton6);
  spdlog::info("L1: {}", input_report07.BTN_GamepadButton7);
  spdlog::info("R1: {}", input_report07.BTN_GamepadButton8);
  spdlog::info("L2: {}", input_report07.BTN_GamepadButton9);
  spdlog::info("R2: {}", input_report07.BTN_GamepadButton10);
  spdlog::info("View: {}", input_report07.BTN_GamepadButton11);
  spdlog::info("Menu: {}", input_report07.BTN_GamepadButton12);
  spdlog::info("Home: {}", input_report07.BTN_GamepadButton13);
  spdlog::info("Left Stick Btn: {}", input_report07.BTN_GamepadButton14);
  spdlog::info("Right Stick Btn: {}", input_report07.BTN_GamepadButton15);
  spdlog::info("M2: {}", input_report07.BTN_GamepadButton16);
  spdlog::info("Button 17: {}", input_report07.BTN_GamepadButton17);
  spdlog::info("Button 18: {}", input_report07.BTN_GamepadButton18);
  spdlog::info("L4: {}", input_report07.BTN_GamepadButton19);
  spdlog::info("R4: {}", input_report07.BTN_GamepadButton20);
}

void InputReader::PrintInputReport10(const inputReport10_t& input_report10) {
  std::ostringstream os;
  os << CustomHexdump<400, false>(input_report10.VEN_Gamepad0024,
                                  sizeof(inputReport10_t));
  spdlog::info("Input Report 10: {}", os.str());
}

void InputReader::PrintInputReport12(const inputReport12_t& input_report12) {
  std::ostringstream os;
  os << CustomHexdump<400, false>(input_report12.VEN_Gamepad0022,
                                  sizeof(inputReport10_t));
  spdlog::info("Input Report 10: {}", os.str());
}

void InputReader::PrintInputReport14(const inputReport14_t& input_report14) {
  std::ostringstream os;
  os << CustomHexdump<400, false>(input_report14.VEN_Gamepad0026,
                                  sizeof(inputReport10_t));
  spdlog::info("Input Report 14: {}", os.str());
}
