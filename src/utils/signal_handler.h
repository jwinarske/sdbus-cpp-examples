// Copyright (c) 2026 Joel Winarske
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

#ifndef SRC_UTILS_SIGNAL_HANDLER_H
#define SRC_UTILS_SIGNAL_HANDLER_H

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstring>
#include <memory>
#include <optional>

#include <unistd.h>

#include <sdbus-c++/sdbus-c++.h>
#include "logging.h"

/**
 * @brief Global flag to control application lifecycle
 *
 * Set to false by signal handlers to trigger graceful shutdown.
 */
inline std::atomic<bool> g_running{true};

inline constexpr auto kConnectionCheckInterval = std::chrono::seconds(30);
inline constexpr auto kMonitorSleepInterval = std::chrono::milliseconds(100);
inline constexpr auto kLogFlushInterval = std::chrono::seconds(5);

/**
 * @brief Signal handler for graceful shutdown
 *
 * Handles SIGINT (Ctrl+C) and SIGTERM (kill) signals.
 *
 * @param signum Signal number received
 */
inline void signalHandler(int signum) {
  auto signame = "UNKNOWN";
  switch (signum) {
    case SIGINT:
      signame = "SIGINT";
      break;
    case SIGTERM:
      signame = "SIGTERM";
      break;
    default:
      break;
  }

  // Only use async-signal-safe operations in signal handlers.
  // write() and _exit() are safe; spdlog is not.
  const char prefix[] = "Received signal ";
  const char suffix[] = " - initiating graceful shutdown\n";
  // NOLINTNEXTLINE(bugprone-unused-return-value)
  (void)::write(STDERR_FILENO, prefix, sizeof(prefix) - 1);
  // NOLINTNEXTLINE(bugprone-unused-return-value)
  (void)::write(STDERR_FILENO, signame, std::strlen(signame));
  // NOLINTNEXTLINE(bugprone-unused-return-value)
  (void)::write(STDERR_FILENO, suffix, sizeof(suffix) - 1);
  g_running = false;
}

/**
 * @brief Install signal handlers for graceful shutdown
 *
 * Sets up handlers for SIGINT, SIGTERM, and ignores SIGPIPE.
 */
inline void installSignalHandlers() {
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);
  std::signal(SIGPIPE, SIG_IGN);  // Prevent crashes on broken pipes
}

/**
 * @brief Check if D-Bus connection is still alive
 *
 * Attempts to call a simple method to verify the connection is working.
 *
 * @param connection D-Bus connection to check
 * @return true if the connection is alive, false otherwise
 */
inline bool isConnectionAlive(sdbus::IConnection& connection) {
  try {
    // Try to get the connection's unique name - this requires communication
    // with the bus
    const auto proxy = sdbus::createProxy(
        connection, sdbus::ServiceName("org.freedesktop.DBus"),
        sdbus::ObjectPath("/org/freedesktop/DBus"));

    // Call GetId - a simple method that should always work if the connection is
    // alive
    std::string bus_id;
    proxy->callMethod("GetId")
        .onInterface("org.freedesktop.DBus")
        .storeResultsTo(bus_id);

    return true;
  } catch (const sdbus::Error& e) {
    LOG_WARN("D-Bus connection check failed: {}", e.what());
    return false;
  } catch (const std::exception& e) {
    LOG_WARN("Connection check exception: {}", e.what());
    return false;
  }
}

/**
 * @brief Monitor loop with connection health checks
 *
 * Periodically checks if the D-Bus connection is still alive and if the
 * application should continue running.
 *
 * @param connection D-Bus connection to monitor
 * @param check_interval How often to check connection health
 * @param sleep_interval How long to sleep between checks
 * @return std::optional<std::string> Error message if connection lost, nullopt
 * if graceful shutdown
 */
inline std::optional<std::string> monitorLoop(
    sdbus::IConnection& connection,
    const std::chrono::seconds check_interval = kConnectionCheckInterval,
    const std::chrono::milliseconds sleep_interval = kMonitorSleepInterval) {
  auto last_check = std::chrono::steady_clock::now();

  while (g_running) {
    std::this_thread::sleep_for(sleep_interval);

    if (auto now = std::chrono::steady_clock::now();
        now - last_check >= check_interval) {
      if (!isConnectionAlive(connection)) {
        return "D-Bus connection lost";
      }
      last_check = now;
    }
  }

  return std::nullopt;  // Graceful shutdown via signal
}

#endif  // SRC_UTILS_SIGNAL_HANDLER_H
