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

#include "timesync1_manager_client.h"

#include <sdbus-c++/sdbus-c++.h>
#include <spdlog/spdlog.h>

extern void exerciseTimesync(Timesync1ManagerClient&);

int main() {
  try {
    auto connection = sdbus::createSystemBusConnection();
    auto proxy = sdbus::createProxy(*connection,
                                     sdbus::ServiceName("org.freedesktop.timesync1"),
                                     sdbus::ObjectPath("/org/freedesktop/timesync1/Manager"));
    Timesync1ManagerClient client(*proxy);
    exerciseTimesync(client);
    spdlog::info("timesync1 Manager example complete");
  } catch (const sdbus::Error& e) {
    spdlog::error("D-Bus error: {} - {}", e.getName(), e.getMessage());
    return 1;
  } catch (const std::exception& e) {
    spdlog::error("Exception: {}", e.what());
    return 1;
  }
  return 0;
}
