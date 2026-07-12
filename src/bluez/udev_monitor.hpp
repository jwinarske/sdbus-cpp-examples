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

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <libudev.h>

#include "../utils/event_loop.h"
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

/// Monitors udev for device add/remove events as an EventSource. The netlink
/// socket fd is polled by an EventLoop; dispatch() drains the pending device
/// events and invokes the callback for each. Single-threaded: no internal
/// thread, so device callbacks run on the same loop thread as the D-Bus
/// callbacks and need no cross-thread synchronisation.
class UdevMonitor : public EventSource {
 public:
  UdevMonitor(
      const std::vector<std::string>& sub_systems,
      std::function<void(const char*, const char*, const char*)> callback)
      : callback_(std::move(callback)) {
    udev_ = UdevPtr(udev_new());
    if (!udev_) {
      LOG_ERROR("Failed to create udev context");
      return;
    }

    mon_ = UdevMonitorPtr(udev_monitor_new_from_netlink(udev_.get(), "udev"));
    if (!mon_) {
      LOG_ERROR("Failed to create udev monitor");
      return;
    }

    for (const auto& sub_system : sub_systems) {
      if (const int res = udev_monitor_filter_add_match_subsystem_devtype(
              mon_.get(), sub_system.c_str(), nullptr);
          res != 0) {
        LOG_ERROR(
            "udev_monitor_filter_add_match_subsystem_devtype failed on {} = {}",
            sub_system, res);
      }
    }
    udev_monitor_enable_receiving(mon_.get());
    fd_ = udev_monitor_get_fd(mon_.get());
  }

  UdevMonitor(const UdevMonitor&) = delete;
  UdevMonitor& operator=(const UdevMonitor&) = delete;
  UdevMonitor(UdevMonitor&&) = delete;
  UdevMonitor& operator=(UdevMonitor&&) = delete;
  ~UdevMonitor() override = default;

  [[nodiscard]] int fd() const override { return fd_; }

  void dispatch(short /*revents*/) override {
    // Drain every device event queued on the netlink socket.
    while (UdevDevicePtr dev{udev_monitor_receive_device(mon_.get())}) {
      if (!callback_) {
        continue;
      }
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
  }

 private:
  std::function<void(const char*, const char*, const char*)> callback_;
  UdevPtr udev_;
  UdevMonitorPtr mon_;
  int fd_ = -1;
};

#endif  // SRC_BLUEZ_UDEV_MONITOR_HPP_
