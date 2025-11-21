#include <chrono>
#include <thread>
#include "systemd1_manager_client.h"

int main() {
  auto connection = sdbus::createSystemBusConnection();
  connection->enterEventLoopAsync();

  Systemd1ManagerClient client(*connection);

  // Optional unit start (avoid direct Unit_proxy instantiation; use generic
  // proxy)
  if (const char* unitPathEnv = std::getenv("SYSTEMD_UNIT_PATH")) {
    try {
      const auto unitPath = sdbus::ObjectPath(unitPathEnv);
      const auto unitProxy = sdbus::createProxy(
          *connection, sdbus::ServiceName(Systemd1ManagerClient::SERVICE_NAME),
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
  std::this_thread::sleep_for(5s);  // collect signals

  connection->leaveEventLoop();
  return 0;
}