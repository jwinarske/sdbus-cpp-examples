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

#ifndef SRC_REALTIMEKIT1_REALTIMEKIT1_MANAGER_CLIENT_H
#define SRC_REALTIMEKIT1_REALTIMEKIT1_MANAGER_CLIENT_H

#include "../proxy/org/freedesktop/RealtimeKit1/realtime_kit1_proxy.h"

#include <sdbus-c++/sdbus-c++.h>
#include <spdlog/spdlog.h>

#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdint>
#include <string>

class RealtimeKit1ManagerClient : public org::freedesktop::RealtimeKit1_proxy {
 public:
  explicit RealtimeKit1ManagerClient(sdbus::IProxy& proxy)
      : org::freedesktop::RealtimeKit1_proxy(proxy) {}

  void dumpProperties() {
    spdlog::info("RTTimeUSecMax       : {}", RTTimeUSecMax());
    spdlog::info("MaxRealtimePriority : {}", MaxRealtimePriority());
    spdlog::info("MinNiceLevel        : {}", MinNiceLevel());
  }

  bool tryHighPriority(int32_t niceLevel) {
    try {
      auto tid = currentTid();
      spdlog::info("MakeThreadHighPriority thread={} priority={}", tid,
                   niceLevel);
      MakeThreadHighPriority(tid, niceLevel);
      spdlog::info("High priority change succeeded");
      return true;
    } catch (const sdbus::Error& e) {
      spdlog::warn("High priority change failed: {} ({})", e.getName(),
                   e.getMessage());
      return false;
    }
  }

  bool tryRealtime(uint32_t rtPriority) {
    try {
      auto tid = currentTid();
      spdlog::info("MakeThreadRealtime thread={} rtPriority={}", tid,
                   rtPriority);
      MakeThreadRealtime(tid, rtPriority);
      spdlog::info("Realtime change succeeded");
      return true;
    } catch (const sdbus::Error& e) {
      spdlog::warn("Realtime change failed: {} ({})", e.getName(),
                   e.getMessage());
      return false;
    }
  }

 private:
  static uint64_t currentTid() {
#ifdef SYS_gettid
    return static_cast<uint64_t>(::syscall(SYS_gettid));
#else
    return static_cast<uint64_t>(::getpid());
#endif
  }
};

#endif  // SRC_REALTIMEKIT1_REALTIMEKIT1_MANAGER_CLIENT_H
