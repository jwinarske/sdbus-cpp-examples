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

#include "systemd1_manager_client.h"

#include <chrono>
#include <thread>

int main() {
  auto connection = sdbus::createSystemBusConnection();
  connection->enterEventLoopAsync();

  Systemd1ManagerClient client(*connection);

  if (const char* unitPathEnv = std::getenv("SYSTEMD_UNIT_PATH")) {
    try {
      auto unitPath = sdbus::ObjectPath(unitPathEnv);
      auto unitProxy = sdbus::createProxy(*connection,
                                          sdbus::ServiceName(Systemd1ManagerClient::SERVICE_NAME),
                                          unitPath);
      unitProxy->callMethod("Start")
               .onInterface(org::freedesktop::systemd1::Unit_proxy::INTERFACE_NAME)
               .withArguments(std::string("replace"));
      spdlog::info("Attempted Start on unit {}", unitPathEnv);
    } catch (const sdbus::Error& e) {
      spdlog::error("Failed to start unit {}: {} - {}", unitPathEnv,
                    e.getName(), e.getMessage());
    }
  }

  using namespace std::chrono_literals;
  std::this_thread::sleep_for(5s); // collect signals

  connection->leaveEventLoop();
  return 0;
}
