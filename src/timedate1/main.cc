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

#include "timedate1_client.h"

#include "../utils.h"

int main() {
  const auto connection = sdbus::createSystemBusConnection();
  connection->enterEventLoopAsync();

  Timedate1Client client(*connection);

  std::future<std::map<sdbus::PropertyName, sdbus::Variant>> futures[4];
  std::promise<std::map<sdbus::PropertyName, sdbus::Variant>> promises[4];

  for (int i = 0; i < 4; ++i) {
    futures[i] = promises[i].get_future();
    client.GetAllAsync(
        Timedate1Client::INTERFACE_NAME,
        [&, i](std::optional<sdbus::Error> error,
               std::map<sdbus::PropertyName, sdbus::Variant> values) {
          if (!error)
            promises[i].set_value(std::move(values));
          else
            promises[i].set_exception(
                std::make_exception_ptr(std::move(*error)));
        });
  }

  for (auto& future : futures) {
    try {
      const auto properties = future.get();
      client.updateTimedate1(properties);
      client.printTimedate1();
    } catch (const std::exception& e) {
      spdlog::error("Error: {}", e.what());
    }
  }

  connection->leaveEventLoop();

  return 0;
}
