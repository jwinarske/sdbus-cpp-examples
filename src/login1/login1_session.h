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

#ifndef SRC_LOGIN1_LOGIN1_SESSION_H
#define SRC_LOGIN1_LOGIN1_SESSION_H

#include "../proxy/org/freedesktop/login1/Session/session_proxy.h"
#include "../utils.h"

class Login1Session final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::login1::Session_proxy> {
 public:
  Login1Session(sdbus::IConnection& connection,
                const sdbus::ObjectPath& objectPath)
      : ProxyInterfaces{connection,
                        sdbus::ServiceName("org.freedesktop.login1"),
                        objectPath} {
    registerProxy();
    {
      const auto props = this->GetAllAsync(
          Session_proxy::INTERFACE_NAME,
          [&](std::optional<sdbus::Error> error,
              std::map<sdbus::PropertyName, sdbus::Variant> values) {
            if (!error)
              onPropertiesChanged(
                  sdbus::InterfaceName(Session_proxy::INTERFACE_NAME), values,
                  {});
            else
              spdlog::error("login1.Session: {} - {}", error->getName(),
                            error->getMessage());
          });
    }
  }

  virtual ~Login1Session() { unregisterProxy(); }

 private:
  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    Utils::print_changed_properties(interfaceName, changedProperties,
                                    invalidatedProperties);
  }

  void onPauseDevice(const uint32_t& major,
                     const uint32_t& minor,
                     const std::string& type) override {
    spdlog::info("[Login1Session] onPauseDevice: major={}, minor={}, type={}",
                 major, minor, type);
  }

  void onResumeDevice(const uint32_t& major,
                      const uint32_t& minor,
                      const sdbus::UnixFd& fd) override {
    spdlog::info("[Login1Session] onResumeDevice: major={}, minor={}, fd={}",
                 major, minor, fd.get());
  }

  void onLock() override { spdlog::info("[Login1Session] onLock"); }

  void onUnlock() override { spdlog::info("[Login1Session] onUnlock"); }
};

#endif  // SRC_LOGIN1_LOGIN1_SESSION_H
