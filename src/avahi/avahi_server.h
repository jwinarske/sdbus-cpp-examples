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

#ifndef SRC_AVAHI_AVAHI_SERVER_CLIENT_H
#define SRC_AVAHI_AVAHI_SERVER_CLIENT_H

#include "../proxy/org/freedesktop/Avahi/Server/server_proxy.h"
#include "../proxy/org/freedesktop/Avahi/Server2/server2_proxy.h"

#include "../utils.h"

class AvahiServer final
    : public sdbus::ProxyInterfaces<org::freedesktop::Avahi::Server_proxy> {
 public:
  explicit AvahiServer(sdbus::IConnection& connection)
      : ProxyInterfaces{connection, sdbus::ServiceName(kServiceName),
                        sdbus::ObjectPath(kObjectPath)} {
    registerProxy();
    onStateChanged(GetState(), {});
  }

  virtual ~AvahiServer() { unregisterProxy(); }

 private:
  static constexpr auto kServiceName = "org.freedesktop.Avahi";
  static constexpr auto kObjectPath = "/";
  std::int32_t state_{};

  void onStateChanged(const int32_t& state, const std::string& error) override {
    spdlog::info("onStateChanged: state={}, error={}", state, error);
    state_ = state;
  }
};

#endif  // SRC_AVAHI_AVAHI_SERVER_CLIENT_H
