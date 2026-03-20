// Copyright (c) 2026 Jian De
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

#include "../utils/signal_handler.h"
#include "connman_client.h"

inline constexpr auto kInitialReconnectDelay = std::chrono::seconds(1);
inline constexpr auto kMaxReconnectDelay = std::chrono::seconds(30);

int main() {
  logging_config::initializeLogging("connman_client");
  installSignalHandlers();

  auto reconnect_delay = kInitialReconnectDelay;

  while (g_running) {
    try {
      auto connection = sdbus::createSystemBusConnection();
      connection->enterEventLoopAsync();

      {
        ConnmanManagerClient client(*connection);

        LOG_INFO("ConnMan monitor running - Press Ctrl+C to exit");
        reconnect_delay = kInitialReconnectDelay;

        auto result = monitorLoop(*connection);

        if (result) {
          LOG_ERROR("Monitor loop: {}", *result);
        } else {
          LOG_INFO("Shutting down...");
        }
      }  // client destroyed before leaving event loop

      connection->leaveEventLoop();

      if (!g_running) {
        return 0;
      }

    } catch (const sdbus::Error& e) {
      LOG_ERROR("D-Bus error: {} - {}", e.getName(), e.getMessage());
    } catch (const std::exception& e) {
      LOG_ERROR("Exception: {}", e.what());
    }

    if (!g_running) {
      break;
    }

    LOG_INFO("Reconnecting in {} seconds...",
             std::chrono::duration_cast<std::chrono::seconds>(reconnect_delay)
                 .count());
    std::this_thread::sleep_for(reconnect_delay);
    reconnect_delay = std::min(
        reconnect_delay * 2,
        std::chrono::duration_cast<std::chrono::seconds>(kMaxReconnectDelay));
  }

  return 1;
}