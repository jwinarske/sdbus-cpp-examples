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

#include <algorithm>
#include <array>
#include <atomic>
#include <cstring>
#include <thread>

#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "../../utils/logging.h"
#include "../hidraw.hpp"
#include "input_reader.h"

InputReader::InputReader(std::string device)
    : device_(std::move(device)),
      stop_flag_(false),
      stop_event_fd_(::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK)) {
  if (!stop_event_fd_.valid()) {
    LOG_ERROR("Failed to create eventfd: {}", strerror(errno));
  }
}

void InputReader::start() {
  LOG_DEBUG("InputReader start: {}", device_);
  if (thread_.joinable()) {
    return;  // already running
  }
  stop_flag_ = false;
  thread_ = std::thread([this] { read_input(); });
}

void InputReader::stop() {
  LOG_DEBUG("InputReader stop: {}", device_);
  stop_flag_ = true;
  // Wake the blocking epoll_wait so the loop observes stop_flag_ immediately.
  if (stop_event_fd_.valid()) {
    constexpr std::uint64_t one = 1;
    if (::write(stop_event_fd_.get(), &one, sizeof(one)) < 0) {
      LOG_ERROR("Failed to signal stop eventfd: {}", strerror(errno));
    }
  }
}

InputReader::~InputReader() {
  stop();
  if (thread_.joinable()) {
    thread_.join();
  }
}

void InputReader::read_input() {
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

    // Wait on both the hidraw fd and the stop eventfd so a blocking read can
    // be interrupted immediately when stop() is called from another thread.
    const UniqueFd epoll_fd(epoll_create1(EPOLL_CLOEXEC));
    if (!epoll_fd.valid()) {
      LOG_ERROR("epoll_create1 failed: {}", strerror(errno));
      break;
    }
    epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = fd.get();
    if (epoll_ctl(epoll_fd.get(), EPOLL_CTL_ADD, fd.get(), &ev) == -1) {
      LOG_ERROR("epoll_ctl(hidraw) failed: {}", strerror(errno));
      break;
    }
    if (stop_event_fd_.valid()) {
      ev.data.fd = stop_event_fd_.get();
      if (epoll_ctl(epoll_fd.get(), EPOLL_CTL_ADD, stop_event_fd_.get(), &ev) ==
          -1) {
        LOG_ERROR("epoll_ctl(stop) failed: {}", strerror(errno));
        break;
      }
    }

    while (!stop_flag_) {
      std::array<epoll_event, 2> events{};
      const int nfds =
          epoll_wait(epoll_fd.get(), events.data(), events.size(), -1);
      if (nfds == -1) {
        if (errno == EINTR) {
          continue;
        }
        LOG_ERROR("epoll_wait failed: {}", strerror(errno));
        break;
      }

      bool stop_requested = false;
      bool data_ready = false;
      for (int i = 0; i < nfds; ++i) {
        if (stop_event_fd_.valid() &&
            events.at(i).data.fd == stop_event_fd_.get()) {
          stop_requested = true;
        } else if (events.at(i).data.fd == fd.get()) {
          data_ready = true;
        }
      }
      if (stop_requested) {
        break;
      }
      if (!data_ready) {
        continue;
      }

      std::array<std::uint8_t, sizeof(inputReport01_t)> buffer{};
      const ssize_t result = read(fd.get(), buffer.data(), buffer.size());
      if (result < 0) {
        if (errno == EINTR || errno == EAGAIN) {
          continue;
        }
        LOG_ERROR("read failed: {}", strerror(errno));
        break;
      }
      if (result == 0) {
        continue;
      }
      const auto bytes_read = static_cast<size_t>(result);

      if (raw_dev_info.product == 0x02FD) {
        if (const auto report_id = buffer.at(0); report_id == 1) {
          inputReport01_t input_report01{};
          std::memcpy(&input_report01, buffer.data(),
                      std::min(sizeof(inputReport01_t), bytes_read));
          PrintInputReport1(input_report01);
        } else if (report_id == 2) {
          inputReport02_t input_report02{};
          std::memcpy(&input_report02, buffer.data(),
                      std::min(sizeof(inputReport02_t), bytes_read));
          PrintInputReport2(input_report02);
        } else if (report_id == 4) {
          inputReport04_t input_report04{};
          std::memcpy(&input_report04, buffer.data(),
                      std::min(sizeof(inputReport04_t), bytes_read));
          PrintInputReport4(input_report04);
        } else {
          LOG_ERROR("Unknown report id: {}", report_id);
        }
      }
    }
    break;
  }

  // fd is automatically closed by UniqueFd destructor.
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

void InputReader::PrintInputReport1(const inputReport01_t& input_report01) {
  LOG_INFO("Stick L/R: [{},{}] [{},{}] ", input_report01.GD_GamepadPointerX,
           input_report01.GD_GamepadPointerY, input_report01.GD_GamepadPointerZ,
           input_report01.GD_GamepadPointerRz);
  LOG_INFO("Trigger L/R: {}, {}", input_report01.SIM_GamepadBrake,
           input_report01.SIM_GamepadAccelerator);
  LOG_INFO("D-PAD: {}", dpad_to_string(static_cast<Direction>(
                            input_report01.GD_GamepadHatSwitch)));
  LOG_INFO("A: {}", input_report01.BTN_GamepadButton1);
  LOG_INFO("B: {}", input_report01.BTN_GamepadButton2);
  LOG_INFO("Button3: {}", input_report01.BTN_GamepadButton3);
  LOG_INFO("X: {}", input_report01.BTN_GamepadButton4);
  LOG_INFO("Y: {}", input_report01.BTN_GamepadButton5);
  LOG_INFO("Button6: {}", input_report01.BTN_GamepadButton6);
  LOG_INFO("Left Bumper: {}", input_report01.BTN_GamepadButton7);
  LOG_INFO("Right Bumper: {}", input_report01.BTN_GamepadButton8);
  LOG_INFO("Button9: {}", input_report01.BTN_GamepadButton9);
  LOG_INFO("Button10: {}", input_report01.BTN_GamepadButton10);
  LOG_INFO("Button11: {}", input_report01.BTN_GamepadButton11);
  LOG_INFO("Menu: {}", input_report01.BTN_GamepadButton12);
  LOG_INFO("Button13: {}", input_report01.BTN_GamepadButton13);
  LOG_INFO("Left Stick Btn: {}", input_report01.BTN_GamepadButton14);
  LOG_INFO("Right Stick Btn: {}", input_report01.BTN_GamepadButton15);
  LOG_INFO("Back: {}", input_report01.CD_GamepadAcBack);
}

void InputReader::PrintInputReport2(const inputReport02_t& input_report02) {
  LOG_INFO("Home Button: {}", input_report02.CD_GamepadConsumerControlAcHome);
}

void InputReader::PrintOutputReport3(const outputReport03_t& output_report03) {
  LOG_INFO("OutputReport3: {}", output_report03.reportId);
  LOG_INFO("PID_GamepadSetEffectReportDcEnableActuators: {}",
           output_report03.PID_GamepadSetEffectReportDcEnableActuators);
  LOG_INFO("PID_GamepadSetEffectReportMagnitude: {}, {}, {}, {}",
           output_report03.PID_GamepadSetEffectReportMagnitude[0],
           output_report03.PID_GamepadSetEffectReportMagnitude[1],
           output_report03.PID_GamepadSetEffectReportMagnitude[2],
           output_report03.PID_GamepadSetEffectReportMagnitude[3]);
  LOG_INFO("PID_GamepadSetEffectReportDuration: {}",
           output_report03.PID_GamepadSetEffectReportDuration);
  LOG_INFO("PID_GamepadSetEffectReportStartDelay: {}",
           output_report03.PID_GamepadSetEffectReportStartDelay);
  LOG_INFO("PID_GamepadSetEffectReportLoopCount: {}",
           output_report03.PID_GamepadSetEffectReportLoopCount);
}

void InputReader::PrintInputReport4(const inputReport04_t& output_report04) {
  float battery_percentage =
      (static_cast<float>(output_report04.GDC_GamepadBatteryStrength) /
       255.0f) *
      100.0f;
  battery_percentage = std::round(battery_percentage);  // Round the percentage
  LOG_INFO("Battery: {}", battery_percentage);
}
