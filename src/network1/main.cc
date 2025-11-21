#include "network1_client.h"

#include <chrono>
#include <thread>

int main() {
  const auto connection = sdbus::createSystemBusConnection();
  connection->enterEventLoopAsync();

  Network1ManagerClient client(*connection);

  using namespace std::chrono_literals;
  std::this_thread::sleep_for(2s);  // allow async enumeration

  connection->leaveEventLoop();
  return 0;
}