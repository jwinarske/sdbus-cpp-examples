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

#include "../utils/event_loop.h"
#include "../utils/signal_source.h"
#include "upower_client.h"

int main() {
  try {
    // Single-threaded loop drives the D-Bus connection and signal delivery.
    // Construct the SignalSource before any thread starts (e.g. spdlog's
    // flush thread) so SIGINT/SIGTERM are blocked and delivered via the loop.
    EventLoop loop;
    SignalSource signals(loop);
    loop.add(&signals);

    const auto connection = sdbus::createSystemBusConnection();

    UPowerClient client(
        *connection,
        sdbus::ObjectPath("/org/freedesktop/UPower/devices/DisplayDevice"));

    LOG_INFO("UPower monitor daemon running - Press Ctrl+C to exit");

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
