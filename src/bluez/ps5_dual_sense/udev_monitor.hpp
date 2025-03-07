// File: hidraw.hpp

#pragma once

#include <functional>
#include <string>
#include <vector>

#include <unistd.h>

#include <libudev.h>

class UdevMonitor {
 public:
  /**
   * \brief Constructor for UdevMonitor.
   *
   * \param sub_system The subsystem to monitor.
   * \param callback The callback function to call with device information.
   */
  UdevMonitor(
      const char* sub_system,
      const std::function<
          void(const char*, const char*, const char*, const char*)>& callback)
      : callback_(callback) {
    sub_system_ = sub_system;
    if (pipe(pipe_fds_) == -1) {
      return;
    }
  }

  /**
   * \brief Destructor for UdevMonitor.
   */
  virtual ~UdevMonitor() {
    if (running_) {
      stop();
    }
    close(pipe_fds_[0]);
    close(pipe_fds_[1]);
  }

  /**
   * \brief Starts the udev monitoring.
   */
  void run() const {
    const auto udev = udev_new();
    const auto mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, sub_system_.c_str(),
                                                    nullptr);
    udev_monitor_enable_receiving(mon);
    const auto fd = udev_monitor_get_fd(mon);

    while (running_) {
      fd_set fds;
      FD_ZERO(&fds);
      FD_SET(fd, &fds);
      FD_SET(pipe_fds_[0], &fds);

      const int max_fd = std::max(fd, pipe_fds_[0]);

      // blocking
      if (const int ret = select(max_fd + 1, &fds, nullptr, nullptr, nullptr);
          ret > 0) {
        if (FD_ISSET(pipe_fds_[0], &fds)) {
          // Pipe was written to, exit the loop
          break;
        }

        if (FD_ISSET(fd, &fds)) {
          if (const auto dev = udev_monitor_receive_device(mon)) {
            if (callback_) {
              callback_(
                  udev_device_get_action(dev), udev_device_get_devnode(dev),
                  udev_device_get_subsystem(dev), udev_device_get_devtype(dev));
            }
            udev_device_unref(dev);
          }
        }
      }
    }

    udev_monitor_unref(mon);
    udev_unref(udev);
  }

  /**
   * \brief Stops the udev monitoring.
   */
  void stop() {
    running_ = false;
    write(pipe_fds_[1], "x", 1);
  }

 private:
  std::string sub_system_; /**< The subsystem to monitor */

  std::atomic<bool> running_ = {
      true}; /**< Atomic flag to control the running state */

  int pipe_fds_[2]{}; /**< Pipe file descriptors for signaling */

  /**< Callback function to call with device information */
  const std::function<void(const char* action,
                           const char* dev_node,
                           const char* sub_system,
                           const char* dev_type)>& callback_;
};