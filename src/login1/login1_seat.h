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

#ifndef SRC_LOGIN1_LOGIN1_SEAT_H
#define SRC_LOGIN1_LOGIN1_SEAT_H

#include "../proxy/org/freedesktop/login1/Seat/seat_proxy.h"
#include "../utils.h"

class Login1Seat final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::login1::Seat_proxy> {
 public:
  Login1Seat(sdbus::IConnection& connection,
             const sdbus::ObjectPath& objectPath)
      : ProxyInterfaces{connection,
                        sdbus::ServiceName("org.freedesktop.login1"),
                        objectPath} {
    registerProxy();
    {
      const auto props = this->GetAllAsync(
          Seat_proxy::INTERFACE_NAME,
          [&](std::optional<sdbus::Error> error,
              std::map<sdbus::PropertyName, sdbus::Variant> values) {
            if (!error)
              onPropertiesChanged(
                  sdbus::InterfaceName(Seat_proxy::INTERFACE_NAME), values, {});
            else
              spdlog::error("login1.Seat: {} - {}", error->getName(),
                            error->getMessage());
          });
    }
  }

  virtual ~Login1Seat() { unregisterProxy(); }

 private:
  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    Utils::print_changed_properties(interfaceName, changedProperties,
                                    invalidatedProperties);
  }
};

#endif  // SRC_LOGIN1_LOGIN1_SEAT_H
