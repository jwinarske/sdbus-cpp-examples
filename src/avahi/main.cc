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

#include "../utils/logging.h"
#include "../utils/utils.h"

int main() {
  const auto connection = sdbus::createSystemBusConnection();
  connection->enterEventLoopAsync();

  if (const auto names = Utils::ListNames(*connection);
      Utils::isServicePresent(names, "org.freedesktop.Avahi")) {
    AvahiServer server(*connection);
    LOG_INFO("API Version: {}", server.GetAPIVersion());
    LOG_INFO("Domain Name: {}", server.GetDomainName());
    LOG_INFO("Host Name: {}", server.GetHostName());
    LOG_INFO("FQDN: {}", server.GetHostNameFqdn());
    LOG_INFO("Local Service Cookie: {}", server.GetLocalServiceCookie());
    LOG_INFO("State: {}", server.GetState());
    LOG_INFO("Version: {}", server.GetVersionString());
    LOG_INFO("NSS Support available: {}",
             server.IsNSSSupportAvailable() ? "Yes" : "No");
  }

  connection->leaveEventLoop();
  return 0;
}
