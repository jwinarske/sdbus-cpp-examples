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

#ifndef SRC_BLUEZ_UDEV_MONITOR_HPP_
#define SRC_BLUEZ_UDEV_MONITOR_HPP_

#include <libudev.h>
#include <spdlog/spdlog.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <atomic>
#include <functional>
#include <string>
#include <thread>
#include <utility>
#include <vector>

class UdevMonitor {
 public:
  UdevMonitor(std::vector<std::string> sub_systems,
              const std::function<void(const char*, const char*, const char*)>&
                  callback)
      : sub_systems_(std::move(sub_systems)), callback_(callback) {
    if (pipe(pipe_fds_) == -1) {
      return;
    }
    std::thread(&UdevMonitor::run, this).detach();
  }

  virtual ~UdevMonitor() {
    if (is_running_) {
      stop();
    }
    close(pipe_fds_[0]);
    close(pipe_fds_[1]);
  }

  void stop() {
    is_running_ = false;
    write(pipe_fds_[1], "x", 1);
  }

 private:
  std::vector<std::string> sub_systems_;
  std::atomic<bool> is_running_{true};
  int pipe_fds_[2]{};
  std::function<void(const char*, const char*, const char*)> callback_;

  void run() {
    const auto udev = udev_new();
    const auto mon = udev_monitor_new_from_netlink(udev, "udev");
    for (const auto& sub_system : sub_systems_) {
      if (int res = udev_monitor_filter_add_match_subsystem_devtype(
              mon, sub_system.c_str(), nullptr);
          res != 0) {
        spdlog::error(
            "udev_monitor_filter_add_match_subsystem_devtype failed on {} = {}",
            sub_system, res);
      }
    }
    udev_monitor_enable_receiving(mon);
    const auto fd = udev_monitor_get_fd(mon);

    const int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
      return;
    }

    epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
      close(epoll_fd);
      return;
    }

    ev.data.fd = pipe_fds_[0];
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pipe_fds_[0], &ev) == -1) {
      close(epoll_fd);
      return;
    }

    while (is_running_) {
      epoll_event events[2];
      const int triggered_event_count = epoll_wait(epoll_fd, events, 2, -1);
      if (triggered_event_count == -1) {
        break;
      }

      for (int n = 0; n < triggered_event_count; ++n) {
        if (events[n].data.fd == pipe_fds_[0]) {
          spdlog::info("Pipe was written to, exiting the loop");
          is_running_ = false;
          break;
        }

        if (events[n].data.fd == fd) {
          if (const auto dev = udev_monitor_receive_device(mon)) {
            if (callback_) {
              callback_(udev_device_get_action(dev),
                        udev_device_get_devnode(dev),
                        udev_device_get_subsystem(dev));
            }
            udev_device_unref(dev);
          }
        }
      }
    }

    udev_monitor_unref(mon);
    udev_unref(udev);
  }
};

#endif  // SRC_BLUEZ_UDEV_MONITOR_HPP_
