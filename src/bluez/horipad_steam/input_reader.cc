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

#include <array>
#include <atomic>
#include <cstring>
#include <thread>

#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "../../utils/logging.h"
#include "../hidraw.hpp"
#include "input_reader.h"

InputReader::InputReader(std::string device)
    : device_(std::move(device)), stop_flag_(false) {}

void InputReader::start() {
  LOG_DEBUG("InputReader start: {}", device_);
  stop_flag_ = false;
  read_input();
}

void InputReader::stop() {
  LOG_DEBUG("InputReader stop: {}", device_);
  stop_flag_ = true;
}

InputReader::~InputReader() {
  stop();
}

// NOLINTNEXTLINE(readability-static-accessed-through-instance)
InputReader::Task InputReader::read_input() {
  LOG_DEBUG("hidraw device: {}", device_);

  const UniqueFd fd(open(device_.c_str(), O_RDWR));

  while (true) {
    if (!fd.valid()) {
      LOG_ERROR("unable to open device");
      break;
    }

    // Raw Info
    hidraw_devinfo raw_dev_info{};
    if (const auto res = ioctl(fd.get(), HIDIOCGRAWINFO, &raw_dev_info);
        res < 0) {
      LOG_ERROR("HIDIOCGRAWINFO");
      break;
    }
    LOG_INFO("bustype: {}", Hidraw::bus_str(raw_dev_info.bustype));
    LOG_INFO("Vendor ID: {:04X}", raw_dev_info.vendor);
    LOG_INFO("Product ID: {:04X}", raw_dev_info.product);

    // Raw Name
    std::array<char, 256> buf{};
    auto res = ioctl(fd.get(), HIDIOCGRAWNAME(buf.size()), buf.data());
    if (res < 0) {
      LOG_ERROR("HIDIOCGRAWNAME");
      break;
    }
    buf.back() = '\0';  // guarantee null-termination
    LOG_INFO("HID Name: {}", buf.data());

    // Raw Physical Location
    res = ioctl(fd.get(), HIDIOCGRAWPHYS(buf.size()), buf.data());
    if (res < 0) {
      LOG_ERROR("HIDIOCGRAWPHYS");
      break;
    }
    buf.back() = '\0';  // guarantee null-termination
    LOG_INFO("HID Physical Location: {}", buf.data());

    // Report Descriptor Size
    int desc_size = 0;
    res = ioctl(fd.get(), HIDIOCGRDESCSIZE, &desc_size);
    if (res < 0) {
      LOG_ERROR("HIDIOCGRDESCSIZE");
      break;
    }
    LOG_INFO("Report Descriptor Size: {}", desc_size);

    if (desc_size < 0 ||
        static_cast<unsigned>(desc_size) > HID_MAX_DESCRIPTOR_SIZE) {
      LOG_ERROR("Invalid report descriptor size: {}", desc_size);
      break;
    }

    // Report Descriptor
    hidraw_report_descriptor rpt_desc{};
    rpt_desc.size = desc_size;
    res = ioctl(fd.get(), HIDIOCGRDESC, &rpt_desc);
    if (res < 0) {
      LOG_ERROR("HIDIOCGRDESC");
      break;
    }

    std::ostringstream os;
    os << "Report Descriptor\n";
    os << CustomHexdump<400, false>(std::data(rpt_desc.value), rpt_desc.size);
    LOG_INFO(os.str());

    while (!stop_flag_) {
      std::array<std::uint8_t, sizeof(inputReport12_t)> buffer{};
      ssize_t result = 0;
      if (result = read(fd.get(), buffer.data(), buffer.size()); result < 0) {
        LOG_ERROR("read failed: {}", strerror(errno));
        break;
      }

      if (raw_dev_info.product == 0x01ab || raw_dev_info.product == 0x0196) {
        if (const auto report_id = buffer.at(0); report_id == 7) {
          inputReport07_t input_report07{};
          std::memcpy(&input_report07, buffer.data(),
                      std::min(sizeof(inputReport07_t), buffer.size()));
          PrintInputReport7(input_report07);
        } else if (report_id == 10) {
          inputReport10_t input_report10{};
          std::memcpy(&input_report10, buffer.data(),
                      std::min(sizeof(inputReport10_t), buffer.size()));
          PrintInputReport10(input_report10);
        } else if (report_id == 12) {
          inputReport12_t input_report12{};
          std::memcpy(&input_report12, buffer.data(),
                      std::min(sizeof(inputReport12_t), buffer.size()));
          PrintInputReport12(input_report12);
        } else if (report_id == 14) {
          inputReport14_t input_report14{};
          std::memcpy(&input_report14, buffer.data(),
                      std::min(sizeof(inputReport14_t), buffer.size()));
          PrintInputReport14(input_report14);
        } else {
          LOG_ERROR("Unknown report id: {}", report_id);
        }
      }
    }
    break;
  }

  // fd is automatically closed by UniqueFd destructor
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
  LOG_INFO("Stick L/R: [{},{}] [{},{}] ", input_report07.GD_GamepadX,
           input_report07.GD_GamepadY, input_report07.GD_GamepadZ,
           input_report07.GD_GamepadRz);
  LOG_INFO("L1/L2: {}, {}", input_report07.SIM_GamepadBrake,
           input_report07.SIM_GamepadAccelerator);
  LOG_INFO("D-PAD: {}", dpad_to_string(static_cast<Direction>(
                            input_report07.GD_GamepadHatSwitch)));
  LOG_INFO("A: {}", input_report07.BTN_GamepadButton1);
  LOG_INFO("B: {}", input_report07.BTN_GamepadButton2);
  LOG_INFO("Quick Access: {}", input_report07.BTN_GamepadButton3);
  LOG_INFO("X: {}", input_report07.BTN_GamepadButton4);
  LOG_INFO("Y: {}", input_report07.BTN_GamepadButton5);
  LOG_INFO("M1: {}", input_report07.BTN_GamepadButton6);
  LOG_INFO("L1: {}", input_report07.BTN_GamepadButton7);
  LOG_INFO("R1: {}", input_report07.BTN_GamepadButton8);
  LOG_INFO("L2: {}", input_report07.BTN_GamepadButton9);
  LOG_INFO("R2: {}", input_report07.BTN_GamepadButton10);
  LOG_INFO("View: {}", input_report07.BTN_GamepadButton11);
  LOG_INFO("Menu: {}", input_report07.BTN_GamepadButton12);
  LOG_INFO("Home: {}", input_report07.BTN_GamepadButton13);
  LOG_INFO("Left Stick Btn: {}", input_report07.BTN_GamepadButton14);
  LOG_INFO("Right Stick Btn: {}", input_report07.BTN_GamepadButton15);
  LOG_INFO("M2: {}", input_report07.BTN_GamepadButton16);
  LOG_INFO("Button 17: {}", input_report07.BTN_GamepadButton17);
  LOG_INFO("Button 18: {}", input_report07.BTN_GamepadButton18);
  LOG_INFO("L4: {}", input_report07.BTN_GamepadButton19);
  LOG_INFO("R4: {}", input_report07.BTN_GamepadButton20);
}

void InputReader::PrintInputReport10(const inputReport10_t& input_report10) {
  std::ostringstream os;
  os << CustomHexdump<400, false>(std::data(input_report10.VEN_Gamepad0024),
                                  sizeof(inputReport10_t));
  LOG_INFO("Input Report 10: {}", os.str());
}

void InputReader::PrintInputReport12(const inputReport12_t& input_report12) {
  std::ostringstream os;
  os << CustomHexdump<400, false>(std::data(input_report12.VEN_Gamepad0022),
                                  sizeof(inputReport10_t));
  LOG_INFO("Input Report 10: {}", os.str());
}

void InputReader::PrintInputReport14(const inputReport14_t& input_report14) {
  std::ostringstream os;
  os << CustomHexdump<400, false>(std::data(input_report14.VEN_Gamepad0026),
                                  sizeof(inputReport10_t));
  LOG_INFO("Input Report 14: {}", os.str());
}
