#include <atomic>
#include <thread>
#include <utility>

#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/poll.h>

#include "hidraw.hpp"
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
    spdlog::info("bustype: {}", bus_str(raw_dev_info.bustype));
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
      std::uint8_t buffer[sizeof(inputReport01_t)];
      ssize_t result = 0;
      if (result = read(fd, &buffer[0], sizeof(inputReport01_t)); result < 0) {
        spdlog::error("GetInputReport4 failed: {}", strerror(errno));
        break;
      }

      if (raw_dev_info.product == 0x02FD) {
        if (buffer[0] == 1) {
          const auto* input_report01 =
              reinterpret_cast<inputReport01_t*>(buffer);
          PrintInputReport1(*input_report01);
        } else if (buffer[0] == 2) {
          const auto* input_report02 =
              reinterpret_cast<inputReport02_t*>(buffer);
          PrintInputReport2(*input_report02);
        } else if (buffer[0] == 4) {
          const auto* input_report04 =
              reinterpret_cast<inputReport04_t*>(buffer);
          PrintInputReport4(*input_report04);
        } else {
          spdlog::error("Unknown report id: {}", buffer[0]);
        }
      }
    }
    break;
  }

  close(fd);
  stop();

  co_return;
}

void InputReader::PrintInputReport1(const inputReport01_t& input_report01) {
  spdlog::info("Stick L/R: [{},{}] [{},{}] ", input_report01.GD_GamepadPointerX,
               input_report01.GD_GamepadPointerY,
               input_report01.GD_GamepadPointerZ,
               input_report01.GD_GamepadPointerRz);
  spdlog::info("Trigger L/R: {}, {}", input_report01.SIM_GamepadBrake,
               input_report01.SIM_GamepadAccelerator);
  spdlog::info("D-PAD: {}", dpad_to_string(static_cast<Direction>(
                                input_report01.GD_GamepadHatSwitch)));
  spdlog::info("A: {}", input_report01.BTN_GamepadButton1);
  spdlog::info("B: {}", input_report01.BTN_GamepadButton2);
  spdlog::info("Button3: {}", input_report01.BTN_GamepadButton3);
  spdlog::info("X: {}", input_report01.BTN_GamepadButton4);
  spdlog::info("Y: {}", input_report01.BTN_GamepadButton5);
  spdlog::info("Button6: {}", input_report01.BTN_GamepadButton6);
  spdlog::info("Left Bumper: {}", input_report01.BTN_GamepadButton7);
  spdlog::info("Right Bumper: {}", input_report01.BTN_GamepadButton8);
  spdlog::info("Button9: {}", input_report01.BTN_GamepadButton9);
  spdlog::info("Button10: {}", input_report01.BTN_GamepadButton10);
  spdlog::info("Button11: {}", input_report01.BTN_GamepadButton11);
  spdlog::info("Menu: {}", input_report01.BTN_GamepadButton12);
  spdlog::info("Button13: {}", input_report01.BTN_GamepadButton13);
  spdlog::info("Left Stick Btn: {}", input_report01.BTN_GamepadButton14);
  spdlog::info("Right Stick Btn: {}", input_report01.BTN_GamepadButton15);
  spdlog::info("Back: {}", input_report01.CD_GamepadAcBack);
}

void InputReader::PrintInputReport2(const inputReport02_t& input_report02) {
  spdlog::info("Home Button: {}",
               input_report02.CD_GamepadConsumerControlAcHome);
}

void InputReader::PrintOutputReport3(const outputReport03_t& output_report03) {
  spdlog::info("OutputReport3: {}", output_report03.reportId);
  spdlog::info("PID_GamepadSetEffectReportDcEnableActuators: {}",
               output_report03.PID_GamepadSetEffectReportDcEnableActuators);
  spdlog::info("PID_GamepadSetEffectReportMagnitude: {}, {}, {}, {}",
               output_report03.PID_GamepadSetEffectReportMagnitude[0],
               output_report03.PID_GamepadSetEffectReportMagnitude[1],
               output_report03.PID_GamepadSetEffectReportMagnitude[2],
               output_report03.PID_GamepadSetEffectReportMagnitude[3]);
  spdlog::info("PID_GamepadSetEffectReportDuration: {}",
               output_report03.PID_GamepadSetEffectReportDuration);
  spdlog::info("PID_GamepadSetEffectReportStartDelay: {}",
               output_report03.PID_GamepadSetEffectReportStartDelay);
  spdlog::info("PID_GamepadSetEffectReportLoopCount: {}",
               output_report03.PID_GamepadSetEffectReportLoopCount);
}

void InputReader::PrintInputReport4(const inputReport04_t& output_report04) {
  float battery_percentage =
      (static_cast<float>(output_report04.GDC_GamepadBatteryStrength) /
       255.0f) *
      100.0f;
  battery_percentage = std::round(battery_percentage);  // Round the percentage
  spdlog::info("Battery: {}", battery_percentage);
}

const char* InputReader::bus_str(const std::uint32_t bus) {
  switch (bus) {
    case BUS_USB:
      return "USB";
    case BUS_HIL:
      return "HIL";
    case BUS_BLUETOOTH:
      return "Bluetooth";
    case BUS_VIRTUAL:
      return "Virtual";
    default:
      return "Other";
  }
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
