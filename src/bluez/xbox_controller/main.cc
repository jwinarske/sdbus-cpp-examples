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

#include "../../utils/event_loop.h"
#include "../../utils/signal_source.h"
#include "xbox_controller.h"

int main() {
  try {
    const auto connection = sdbus::createSystemBusConnection();

    // Single-threaded loop: it drives the D-Bus connection, the udev monitor,
    // and signal delivery, so every callback runs on this thread.
    EventLoop loop;
    SignalSource signals(loop);
    loop.add(&signals);

    XboxController client(*connection, loop);
    loop.add(&client);  // XboxController is a UdevMonitor (an EventSource)

    LOG_INFO("Xbox controller client running - Press Ctrl+C to exit");

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
