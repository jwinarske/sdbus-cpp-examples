
#include "../utils/event_loop.h"
#include "../utils/signal_source.h"
#include "systemd1_manager_client.h"

int main() {
  try {
    // Single-threaded loop drives the D-Bus connection and signal delivery.
    // Construct the SignalSource before any thread starts (e.g. spdlog's
    // flush thread) so SIGINT/SIGTERM are blocked and delivered via the loop.
    EventLoop loop;
    SignalSource signals(loop);
    loop.add(&signals);

    const auto connection = sdbus::createSystemBusConnection();

    Systemd1ManagerClient manager(*connection);

    LOG_INFO("Systemd1 monitor daemon running - Press Ctrl+C to exit");

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