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

#include <spdlog/spdlog.h>
#include "xbox_controller.h"

int main() {
  spdlog::set_level(spdlog::level::debug);
  spdlog::flush_every(std::chrono::seconds(5));

  const auto connection = sdbus::createSystemBusConnection();
  connection->enterEventLoopAsync();

  XboxController client(*connection);

  using namespace std::chrono_literals;
  std::this_thread::sleep_for(120000ms);
  connection->leaveEventLoop();

  return 0;
}
