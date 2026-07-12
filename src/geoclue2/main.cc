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

#include <chrono>

#include "../utils/signal_handler.h"
#include "geoclue2_manager.h"

int main() {
  try {
    installSignalHandlers();

    const auto connection = sdbus::createSystemBusConnection();
    connection->enterEventLoopAsync();

    const GeoClue2Manager manager(
        *connection, [&](const GeoClue2Location& location) {
          const auto [Accuracy, Altitude, Description, Heading, Latitude,
                      Longitude, Speed, Timestamp] = location.Properties();
          LOG_INFO("Timestamp: {}.{}", Timestamp.tv_sec, Timestamp.tv_nsec);
          LOG_INFO("Lat/Long: {}, {}", Latitude, Longitude);
          LOG_INFO("Heading: {}", Heading);
          LOG_INFO("Speed: {}", Speed);
          LOG_INFO("Accuracy: {}", Accuracy);
          LOG_INFO("Altitude: {}", Altitude);
          LOG_INFO("Description: {}", Description);
        });

    const auto& client = manager.Client();
    if (!client) {
      LOG_ERROR("GeoClue2 did not return a client object; cannot continue");
      connection->leaveEventLoop();
      return 1;
    }

    // `desktop id` must be set for Start to work
    client->DesktopId("org.example.geoclue2");
    client->Start();

    LOG_INFO("Geoclue2 monitor daemon running - Press Ctrl+C to exit");

    auto result = monitorLoop(*connection);

    if (result) {
      LOG_ERROR("Exiting due to: {}", *result);
    } else {
      LOG_INFO("Shutting down...");
    }

    connection->leaveEventLoop();
    return result ? 1 : 0;

  } catch (const sdbus::Error& e) {
    LOG_ERROR("D-Bus error: {} - {}", e.getName(), e.getMessage());
    return 1;
  } catch (const std::exception& e) {
    LOG_ERROR("Exception: {}", e.what());
    return 1;
  }
}
