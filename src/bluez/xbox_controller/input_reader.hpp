#ifndef SRC_BLUEZ_XBOX_CONTROLLER_INPUT_READER_HPP_
#define SRC_BLUEZ_XBOX_CONTROLLER_INPUT_READER_HPP_

#include <atomic>
#include <iostream>
#include <thread>

#include <sys/poll.h>

#include "hidraw.hpp"

class InputReader {
 public:
#pragma pack(push, 1)

  //--------------------------------------------------------------------------------
  // Generic Desktop Page inputReport 01 (Device --> Host)
  //--------------------------------------------------------------------------------

  typedef struct {
    uint8_t reportId;  // Report ID = 0x01 (1)
    // Collection: CA:Gamepad CP:Pointer
    uint16_t GD_GamepadPointerX;   // Usage 0x00010030: X, Value = 0 to 65535
    uint16_t GD_GamepadPointerY;   // Usage 0x00010031: Y, Value = 0 to 65535
    uint16_t GD_GamepadPointerZ;   // Usage 0x00010032: Z, Value = 0 to 65535
    uint16_t GD_GamepadPointerRz;  // Usage 0x00010035: Rz, Value = 0 to 65535
    // Collection: CA:Gamepad
    uint16_t
        SIM_GamepadBrake : 10;  // Usage 0x000200C5: Brake, Value = 0 to 1023
    uint8_t : 6;                // Pad
    uint16_t SIM_GamepadAccelerator : 10;  // Usage 0x000200C4: Accelerator,
    // Value = 0 to 1023
    uint8_t : 6;  // Pad
    uint8_t
        GD_GamepadHatSwitch : 4;  // Usage 0x00010039: Hat Switch, Value = 1 to
    // 8, Physical = (Value - 1) x 45 in degrees
    uint8_t : 4;                     // Pad
    uint8_t BTN_GamepadButton1 : 1;  // Usage 0x00090001: Button 1
    // Primary/trigger, Value = 0 to 1
    uint8_t BTN_GamepadButton2 : 1;  // Usage 0x00090002: Button 2 Secondary,
    // Value = 0 to 1
    uint8_t BTN_GamepadButton3 : 1;  // Usage 0x00090003: Button 3 Tertiary,
    // Value = 0 to 1
    uint8_t
        BTN_GamepadButton4 : 1;  // Usage 0x00090004: Button 4, Value = 0 to 1
    uint8_t
        BTN_GamepadButton5 : 1;  // Usage 0x00090005: Button 5, Value = 0 to 1
    uint8_t
        BTN_GamepadButton6 : 1;  // Usage 0x00090006: Button 6, Value = 0 to 1
    uint8_t
        BTN_GamepadButton7 : 1;  // Usage 0x00090007: Button 7, Value = 0 to 1
    uint8_t
        BTN_GamepadButton8 : 1;  // Usage 0x00090008: Button 8, Value = 0 to 1
    uint8_t
        BTN_GamepadButton9 : 1;  // Usage 0x00090009: Button 9, Value = 0 to 1
    uint8_t
        BTN_GamepadButton10 : 1;  // Usage 0x0009000A: Button 10, Value = 0 to 1
    uint8_t
        BTN_GamepadButton11 : 1;  // Usage 0x0009000B: Button 11, Value = 0 to 1
    uint8_t
        BTN_GamepadButton12 : 1;  // Usage 0x0009000C: Button 12, Value = 0 to 1
    uint8_t
        BTN_GamepadButton13 : 1;  // Usage 0x0009000D: Button 13, Value = 0 to 1
    uint8_t
        BTN_GamepadButton14 : 1;  // Usage 0x0009000E: Button 14, Value = 0 to 1
    uint8_t
        BTN_GamepadButton15 : 1;  // Usage 0x0009000F: Button 15, Value = 0 to 1
    uint8_t : 1;                  // Pad
    uint8_t CD_GamepadAcBack : 1;  // Usage 0x000C0224: AC Back, Value = 0 to 1
    uint8_t : 7;                   // Pad
  } inputReport01_t;

  //--------------------------------------------------------------------------------
  // Consumer Page inputReport 02 (Device --> Host)
  //--------------------------------------------------------------------------------

  typedef struct {
    uint8_t reportId;  // Report ID = 0x02 (2)
    // Collection: CA:Gamepad CA:ConsumerControl
    uint8_t CD_GamepadConsumerControlAcHome : 1;  // Usage 0x000C0223: AC Home,
    // Value = 0 to 1
    uint8_t : 7;  // Pad
  } inputReport02_t;

  //--------------------------------------------------------------------------------
  // Generic Device Controls Page inputReport 04 (Device --> Host)
  //--------------------------------------------------------------------------------

  typedef struct {
    uint8_t reportId;  // Report ID = 0x04 (4)
    // Collection: CA:Gamepad
    uint8_t GDC_GamepadBatteryStrength;  // Usage 0x00060020: Battery Strength,
    // Value = 0 to 255
  } inputReport04_t;

  //--------------------------------------------------------------------------------
  // Physical Input Device Page outputReport 03 (Device <-- Host)
  //--------------------------------------------------------------------------------

  typedef struct {
    uint8_t reportId;  // Report ID = 0x03 (3)
    // Collection: CA:Gamepad CL:SetEffectReport
    uint8_t
        PID_GamepadSetEffectReportDcEnableActuators : 4;  // Usage 0x000F0097:
    // DC Enable
    // Actuators, Value =
    // 0 to 1
    uint8_t : 4;  // Pad
    uint8_t
        PID_GamepadSetEffectReportMagnitude[4];  // Usage 0x000F0070: Magnitude,
    // Value = 0 to 100
    uint8_t PID_GamepadSetEffectReportDuration;  // Usage 0x000F0050: Duration,
    // Value = 0 to 255, Physical =
    // Value in 10⁻² s units
    uint8_t
        PID_GamepadSetEffectReportStartDelay;  // Usage 0x000F00A7: Start Delay,
    // Value = 0 to 255, Physical =
    // Value in 10⁻² s units
    uint8_t PID_GamepadSetEffectReportLoopCount;  // Usage 0x000F007C: Loop
    // Count, Value = 0 to 255
  } outputReport03_t;
#pragma pack(pop)

  explicit InputReader(const std::string& device)
      : device_(device), stop_flag_(false) {}

  void start() {
    spdlog::debug("InputReader start: {}", device_);
    stop_flag_ = false;
    reader_thread_ = std::thread(&InputReader::readInput, this);
  }

  void stop() {
    spdlog::debug("InputReader stop: {}", device_);
    stop_flag_ = true;
    if (reader_thread_.joinable()) {
      reader_thread_.join();
    }
  }

  ~InputReader() { stop(); }

 private:
  static const char* bus_str(const std::uint32_t bus) {
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

  void readInput() {
    spdlog::debug("hidraw device: {}", device_);

    const int fd = open(device_.c_str(), O_RDWR);

    if (fd < 0) {
      spdlog::error("unable to open device");
      stop();
      return;
    }

    // Raw Info
    hidraw_devinfo raw_dev_info{};
    auto res = ioctl(fd, HIDIOCGRAWINFO, &raw_dev_info);
    if (res < 0) {
      spdlog::error("HIDIOCGRAWINFO");
      close(fd);
      stop();
      return;
    }

    spdlog::info("bustype: {}", bus_str(raw_dev_info.bustype));
    spdlog::info("Vendor ID: {:04X}", raw_dev_info.vendor);
    spdlog::info("Product ID: {:04X}", raw_dev_info.product);

    // Raw Name
    char buf[256]{};
    res = ioctl(fd, HIDIOCGRAWNAME(sizeof(buf)), buf);
    if (res < 0) {
      spdlog::error("HIDIOCGRAWNAME");
      close(fd);
      stop();
      return;
    }
    spdlog::info("HID Name: {}", buf);

    // Raw Physical Location
    res = ioctl(fd, HIDIOCGRAWPHYS(sizeof(buf)), buf);
    if (res < 0) {
      spdlog::error("HIDIOCGRAWPHYS");
      close(fd);
      stop();
      return;
    }
    spdlog::info("HID Physical Location: {}", buf);

    // Report Descriptor Size
    int desc_size = 0;
    res = ioctl(fd, HIDIOCGRDESCSIZE, &desc_size);
    if (res < 0) {
      spdlog::error("HIDIOCGRDESCSIZE");
      close(fd);
      stop();
      return;
    }
    spdlog::info("Report Descriptor Size: {}", desc_size);

    // Report Descriptor
    hidraw_report_descriptor rpt_desc{};
    rpt_desc.size = desc_size;
    res = ioctl(fd, HIDIOCGRDESC, &rpt_desc);
    if (res < 0) {
      spdlog::error("HIDIOCGRDESC");
      close(fd);
      stop();
      return;
    }

    std::ostringstream os;
    os << "Report Descriptor\n";
    os << CustomHexdump<400, false>(rpt_desc.value, rpt_desc.size);
    spdlog::info(os.str());

    while (!stop_flag_) {
      std::uint8_t buffer[sizeof(inputReport01_t)];
      GetInputReport(fd, &buffer[0], sizeof(inputReport01_t));
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

    close(fd);
  }

  std::thread reader_thread_;
  std::string device_;
  std::atomic<bool> stop_flag_;

  static int GetInputReport(const int fd,
                            std::uint8_t* report,
                            const std::size_t report_len) {
    pollfd pfd{};
    pfd.fd = fd;
    pfd.events = POLLIN;

    const int poll_res = poll(&pfd, 1, 0);
    if (poll_res == -1) {
      spdlog::error("poll failed: {}", strerror(errno));
      return 1;
    }

    // data not available, use epoll to block until data is available

    if (poll_res == 0) {
      // Create an epoll instance
      const int epoll_fd = epoll_create1(0);
      if (epoll_fd == -1) {
        spdlog::error("Failed to create epoll instance: {}", strerror(errno));
        return 1;
      }

      // Block using epoll to wait for data to be available

      // Add the file descriptor to the epoll instance
      epoll_event event{};
      event.events = EPOLLIN;
      event.data.fd = fd;
      if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
        spdlog::error("Failed to add file descriptor to epoll: {}",
                      strerror(errno));
        close(epoll_fd);
        return 1;
      }

      // Wait for events
      epoll_event events[1];
      if (const int n = epoll_wait(epoll_fd, events, 1, -1); n == -1) {
        spdlog::error("epoll_wait failed: {}", strerror(errno));
        close(epoll_fd);
        return 1;
      }
      // Read data if available
      if (!(events[0].events & EPOLLIN)) {
        spdlog::error("epoll event EPOLLIN not set");
      }
      close(epoll_fd);
    }

    ssize_t result = 0;
    if (result = read(fd, report, report_len); result < 0) {
      spdlog::error("GetInputReport4 failed: {}", strerror(errno));
      return 1;
    }

    return 0;
  }

  static void PrintInputReport1(const inputReport01_t& input_report01) {
    spdlog::info(
        "Stick L/R: [{},{}] [{},{}] ", input_report01.GD_GamepadPointerX,
        input_report01.GD_GamepadPointerY, input_report01.GD_GamepadPointerZ,
        input_report01.GD_GamepadPointerRz);
    spdlog::info("Trigger L/R: {}, {}", input_report01.SIM_GamepadBrake,
                 input_report01.SIM_GamepadAccelerator);
    spdlog::info("D-PAD: {}", input_report01.GD_GamepadHatSwitch);
    spdlog::info("A: {}", input_report01.BTN_GamepadButton1);
    spdlog::info("B: {}", input_report01.BTN_GamepadButton2);
    spdlog::info("Button3: {}", input_report01.BTN_GamepadButton3);
    spdlog::info("X: {}", input_report01.BTN_GamepadButton4);
    spdlog::info("Y: {}", input_report01.BTN_GamepadButton5);
    spdlog::info("Button6: {}", input_report01.BTN_GamepadButton6);
    spdlog::info("Left Bumper: {}", input_report01.BTN_GamepadButton7);
    spdlog::info("Button8: {}", input_report01.BTN_GamepadButton1);
    spdlog::info("Button9: {}", input_report01.BTN_GamepadButton9);
    spdlog::info("Button10: {}", input_report01.BTN_GamepadButton10);
    spdlog::info("Button11: {}", input_report01.BTN_GamepadButton11);
    spdlog::info("Menu: {}", input_report01.BTN_GamepadButton12);
    spdlog::info("Button13: {}", input_report01.BTN_GamepadButton13);
    spdlog::info("Left Stick Btn: {}", input_report01.BTN_GamepadButton14);
    spdlog::info("Right Stick Btn: {}", input_report01.BTN_GamepadButton15);
    spdlog::info("Back: {}", input_report01.CD_GamepadAcBack);
  }

  static void PrintInputReport2(const inputReport02_t& input_report02) {
    spdlog::info("Home Button: {}",
                 input_report02.CD_GamepadConsumerControlAcHome);
  }

  static void PrintOutputReport3(const outputReport03_t& output_report03) {
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

  static void PrintInputReport4(const inputReport04_t& output_report04) {
    float battery_percentage =
        (static_cast<float>(output_report04.GDC_GamepadBatteryStrength) /
         255.0f) *
        100.0f;
    battery_percentage =
        std::round(battery_percentage);  // Round the percentage
    spdlog::info("Battery: {}", battery_percentage);
  }
};

#endif  // SRC_BLUEZ_XBOX_CONTROLLER_INPUT_READER_HPP_
