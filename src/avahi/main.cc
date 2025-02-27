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

#include "avahi_server.h"

#include "../utils/utils.h"

int main() {
  const auto connection = sdbus::createSystemBusConnection();
  connection->enterEventLoopAsync();

  if (const auto names = Utils::ListNames(*connection);
      Utils::isServicePresent(names, "org.freedesktop.Avahi")) {
    AvahiServer server(*connection);
    spdlog::info("API Version: {}", server.GetAPIVersion());
    spdlog::info("Domain Name: {}", server.GetDomainName());
    spdlog::info("Host Name: {}", server.GetHostName());
    spdlog::info("FQDN: {}", server.GetHostNameFqdn());
    spdlog::info("Local Service Cookie: {}", server.GetLocalServiceCookie());
    spdlog::info("State: {}", server.GetState());
    spdlog::info("Version: {}", server.GetVersionString());
    spdlog::info("NSSS upport available: {}",
                 server.IsNSSSupportAvailable() ? "Yes" : "No");
  }

  connection->leaveEventLoop();
  return 0;
}
