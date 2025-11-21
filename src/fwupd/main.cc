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

#include "fwupd_client.h"

#include "../utils/utils.h"

int main() {
  const auto connection = sdbus::createSystemBusConnection();
  connection->enterEventLoopAsync();

  FwupdClient client(*connection);

  // Get and print daemon properties
  {
    std::promise<std::map<sdbus::PropertyName, sdbus::Variant>> promise;
    auto future = promise.get_future();

    client.GetAllAsync(
        FwupdClient::INTERFACE_NAME,
        [&](std::optional<sdbus::Error> error,
            std::map<sdbus::PropertyName, sdbus::Variant> values) {
          if (!error) {
            promise.set_value(std::move(values));
          } else {
            promise.set_exception(std::make_exception_ptr(std::move(*error)));
          }
        });

    try {
      const auto properties = future.get();
      Utils::print_changed_properties(
          sdbus::InterfaceName(FwupdClient::INTERFACE_NAME), properties, {});
    } catch (const sdbus::Error& e) {
      spdlog::warn("Failed to get fwupd properties: {}", e.what());
    }
  }

  // Get and print all devices
  try {
    const auto devices = client.GetDevices();
    spdlog::info("Found {} device(s)", devices.size());

    for (size_t i = 0; i < devices.size(); ++i) {
      std::ostringstream os;
      os << std::endl
         << "Device " << (i + 1) << " of " << devices.size() << std::endl;
      Utils::append_properties(devices[i], os);
      spdlog::info(os.str());
    }
  } catch (const sdbus::Error& e) {
    spdlog::error("Failed to get devices: {}", e.what());
  }

  connection->leaveEventLoop();

  return 0;
}
