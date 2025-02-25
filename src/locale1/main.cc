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

#include "locale1_client.h"

int main() {
  const auto connection = sdbus::createSystemBusConnection();
  connection->enterEventLoopAsync();

  Locale1Client client(*connection);

  std::promise<std::map<sdbus::PropertyName, sdbus::Variant>> promise;
  auto future = promise.get_future();

  client.GetAllAsync(
      Locale1Client::INTERFACE_NAME,
      [&](std::optional<sdbus::Error> error,
          std::map<sdbus::PropertyName, sdbus::Variant> values) {
        if (!error)
          promise.set_value(std::move(values));
        else
          promise.set_exception(std::make_exception_ptr(*std::move(error)));
      });

  const auto properties = future.get();
  client.updateLocale1(properties);

  client.printLocale1();

  connection->leaveEventLoop();

  return 0;
}
