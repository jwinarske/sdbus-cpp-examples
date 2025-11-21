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

#ifndef SRC_TIMESYNC1_TIMESYNC1_MANAGER_CLIENT_H
#define SRC_TIMESYNC1_TIMESYNC1_MANAGER_CLIENT_H

#include "../proxy/org/freedesktop/timesync1/manager_proxy.h"

#include <sdbus-c++/sdbus-c++.h>
#include <spdlog/spdlog.h>

#include <string>
#include <vector>

class Timesync1ManagerClient
    : public org::freedesktop::timesync1::Manager_proxy {
 public:
  explicit Timesync1ManagerClient(sdbus::IProxy& proxy)
      : org::freedesktop::timesync1::Manager_proxy(proxy) {}

  void dumpServerLists() {
    logVector("LinkNTPServers", LinkNTPServers());
    logVector("SystemNTPServers", SystemNTPServers());
    logVector("RuntimeNTPServers", RuntimeNTPServers());
    logVector("FallbackNTPServers", FallbackNTPServers());
  }
  void dumpTiming() {
    spdlog::info("PollIntervalMinUSec : {}", PollIntervalMinUSec());
    spdlog::info("PollIntervalMaxUSec : {}", PollIntervalMaxUSec());
    spdlog::info("PollIntervalUSec    : {}", PollIntervalUSec());
    spdlog::info("RootDistanceMaxUSec : {}", RootDistanceMaxUSec());
    spdlog::info("Frequency           : {}", Frequency());
  }
  void dumpServer() {
    spdlog::info("ServerName          : {}", ServerName());
    auto addr = ServerAddress();
    spdlog::info("ServerAddress family={} bytes={}", addr.get<0>(),
                 addr.get<1>().size());
  }
  void setRuntimeServersExample() {
    std::vector<std::string> servers{"time1.google.com",
                                      "time.cloudflare.com"};
    try {
      spdlog::info("Setting RuntimeNTPServers...");
      SetRuntimeNTPServers(servers);
      logVector("Updated RuntimeNTPServers", RuntimeNTPServers());
    } catch (const sdbus::Error& e) {
      spdlog::warn("SetRuntimeNTPServers failed: {} ({})", e.getName(),
                   e.getMessage());
    }
  }

 private:
  static void logVector(const char* label, const std::vector<std::string>& v) {
    if (v.empty()) {
      spdlog::info("{}: (empty)", label);
      return;
    }
    for (const auto& s : v)
      spdlog::info("{}: {}", label, s);
  }
};

#endif  // SRC_TIMESYNC1_TIMESYNC1_MANAGER_CLIENT_H
