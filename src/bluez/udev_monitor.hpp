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

#include <atomic>
#include <cerrno>
#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <libudev.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "../utils/logging.h"

// ---------------------------------------------------------------------------
// RAII wrappers for libudev handles
// ---------------------------------------------------------------------------
struct UdevDeleter {
  void operator()(udev* u) const noexcept { udev_unref(u); }
};
using UdevPtr = std::unique_ptr<udev, UdevDeleter>;

struct UdevMonitorDeleter {
  void operator()(udev_monitor* m) const noexcept { udev_monitor_unref(m); }
};
using UdevMonitorPtr = std::unique_ptr<udev_monitor, UdevMonitorDeleter>;

struct UdevDeviceDeleter {
  void operator()(udev_device* d) const noexcept { udev_device_unref(d); }
};
using UdevDevicePtr = std::unique_ptr<udev_device, UdevDeviceDeleter>;

/// Minimal RAII wrapper for an epoll file descriptor.
struct EpollFd {
  explicit EpollFd(const int fd) noexcept : fd_(fd) {}
  ~EpollFd() {
    if (fd_ >= 0)
      ::close(fd_);
  }
  EpollFd(const EpollFd&) = delete;
  EpollFd& operator=(const EpollFd&) = delete;
  [[nodiscard]] bool valid() const noexcept { return fd_ >= 0; }
  [[nodiscard]] int get() const noexcept { return fd_; }

 private:
  int fd_;
};

class UdevMonitor {
 public:
  UdevMonitor(std::vector<std::string> sub_systems,
              const std::function<void(const char*, const char*, const char*)>&
                  callback)
      : sub_systems_(std::move(sub_systems)), callback_(callback) {
    if (pipe(pipe_fds_) == -1) {
      LOG_ERROR("Failed to create pipe: {} ({})", std::strerror(errno), errno);
      pipe_fds_[0] = -1;
      pipe_fds_[1] = -1;
      return;
    }

    // Start a thread (not detached, so we can join it)
    worker_thread_ = std::thread(&UdevMonitor::run, this);
  }

  // Delete copy operations - this class manages a thread and file descriptor
  UdevMonitor(const UdevMonitor&) = delete;
  UdevMonitor& operator=(const UdevMonitor&) = delete;

  // Move operations deleted - moving a class with a running thread is complex
  // and not needed for this use case. If move semantics are required in the
  // future, would need to:
  // 1. Stop the thread in the moved-from object
  // 2. Transfer ownership of file descriptors
  // 3. Start a new thread in the moved-to object
  UdevMonitor(UdevMonitor&&) = delete;
  UdevMonitor& operator=(UdevMonitor&&) = delete;

  virtual ~UdevMonitor() {
    stop();

    // Wait for worker thread to finish before closing pipes
    if (worker_thread_.joinable()) {
      worker_thread_.join();
    }

    // Now safe to close pipes
    if (pipe_fds_[0] != -1) {
      close(pipe_fds_[0]);
      pipe_fds_[0] = -1;
    }
    if (pipe_fds_[1] != -1) {
      close(pipe_fds_[1]);
      pipe_fds_[1] = -1;
    }
  }

  void stop() {
    // Only execute stop logic once
    if (bool expected = true;
        !is_running_.compare_exchange_strong(expected, false)) {
      return;  // Already stopped
    }

    // Signal the worker thread to exit
    if (pipe_fds_[1] != -1) {
      if (const ssize_t wrote = write(pipe_fds_[1], "x", 1); wrote == -1) {
        LOG_ERROR("Failed to write to stop pipe: {} ({})", std::strerror(errno),
                  errno);
      }
    }
  }

 private:
  std::vector<std::string> sub_systems_;
  std::atomic<bool> is_running_{true};
  int pipe_fds_[2]{-1, -1};
  std::function<void(const char*, const char*, const char*)> callback_;
  std::thread worker_thread_;

  void run() {
    // RAII: udev context — automatically udev_unref'd on scope exit
    const UdevPtr udev(udev_new());
    if (!udev) {
      LOG_ERROR("Failed to create udev context");
      is_running_ = false;
      return;
    }

    // RAII: udev monitor — automatically udev_monitor_unref'd on scope exit
    const UdevMonitorPtr mon(udev_monitor_new_from_netlink(udev.get(), "udev"));
    if (!mon) {
      LOG_ERROR("Failed to create udev monitor");
      is_running_ = false;
      return;
    }

    for (const auto& sub_system : sub_systems_) {
      if (int res = udev_monitor_filter_add_match_subsystem_devtype(
              mon.get(), sub_system.c_str(), nullptr);
          res != 0) {
        LOG_ERROR(
            "udev_monitor_filter_add_match_subsystem_devtype failed on {} = {}",
            sub_system, res);
      }
    }
    udev_monitor_enable_receiving(mon.get());
    const auto fd = udev_monitor_get_fd(mon.get());

    // RAII: epoll fd — automatically closed on scope exit
    const EpollFd epoll_fd(epoll_create1(0));
    if (!epoll_fd.valid()) {
      LOG_ERROR("Failed to create epoll: {} ({})", std::strerror(errno), errno);
      is_running_ = false;
      return;
    }

    epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(epoll_fd.get(), EPOLL_CTL_ADD, fd, &ev) == -1) {
      LOG_ERROR("Failed to add udev fd to epoll: {} ({})", std::strerror(errno),
                errno);
      is_running_ = false;
      return;
    }

    ev.data.fd = pipe_fds_[0];
    if (epoll_ctl(epoll_fd.get(), EPOLL_CTL_ADD, pipe_fds_[0], &ev) == -1) {
      LOG_ERROR("Failed to add pipe fd to epoll: {} ({})", std::strerror(errno),
                errno);
      is_running_ = false;
      return;
    }

    while (is_running_) {
      epoll_event events[2];
      const int triggered_event_count =
          epoll_wait(epoll_fd.get(), events, 2, -1);
      if (triggered_event_count == -1) {
        if (errno == EINTR) {
          continue;  // Interrupted by signal, retry
        }
        LOG_ERROR("epoll_wait failed: {} ({})", std::strerror(errno), errno);
        break;
      }

      for (int n = 0; n < triggered_event_count; ++n) {
        if (events[n].data.fd == pipe_fds_[0]) {
          LOG_INFO("Pipe was written to, exiting the loop");
          is_running_ = false;
          break;
        }

        if (events[n].data.fd == fd) {
          // RAII: device handle — automatically udev_device_unref'd
          if (UdevDevicePtr dev(udev_monitor_receive_device(mon.get())); dev) {
            if (callback_) {
              const char* action = udev_device_get_action(dev.get());
              const char* devnode = udev_device_get_devnode(dev.get());
              const char* subsystem = udev_device_get_subsystem(dev.get());

              if (action && subsystem) {
                callback_(action, devnode, subsystem);
              } else {
                LOG_DEBUG(
                    "Skipping callback for device with missing properties: "
                    "action={}, devnode={}, subsystem={}",
                    action ? action : "null", devnode ? devnode : "null",
                    subsystem ? subsystem : "null");
              }
            }
            // dev destructor calls udev_device_unref automatically
          }
        }
      }
    }

    // All resources (epoll_fd, mon, udev) are released by their destructors.
    LOG_DEBUG("UdevMonitor worker thread exiting");
  }
};

#endif  // SRC_BLUEZ_UDEV_MONITOR_HPP_
