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

#include "../../utils/event_loop.h"
#include "../../utils/signal_source.h"
#include "dual_sense.h"

int main() {
  try {
    // Single-threaded loop: it drives the D-Bus connection, the udev monitor,
    // and signal delivery, so every callback runs on this thread. Construct the
    // SignalSource first so SIGINT/SIGTERM are blocked before any thread is
    // started (e.g. spdlog's periodic flush thread) and can only be delivered
    // via the loop's signalfd.
    EventLoop loop;
    SignalSource signals(loop);
    loop.add(&signals);

    spdlog::set_level(spdlog::level::debug);
    spdlog::flush_every(std::chrono::seconds(5));

    const auto connection = sdbus::createSystemBusConnection();

    DualSense client(*connection);
    loop.add(&client);  // DualSense is a UdevMonitor (an EventSource)

    LOG_INFO("PS5 DualSense client running - Press Ctrl+C to exit");

    const int rc = loop.run(*connection);
    LOG_INFO("Shutting down...");
    return rc;

  } catch (const sdbus::Error& e) {
    LOG_ERROR("D-Bus error: {} - {}", e.getName(), e.getMessage());
    return 1;
  } catch (const std::exception& e) {
    LOG_ERROR("Exception: {}", e.what());
    return 1;
  }
}
