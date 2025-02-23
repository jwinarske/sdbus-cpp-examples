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

#ifndef SRC_LOGIN1_LOGIN1_USER_H
#define SRC_LOGIN1_LOGIN1_USER_H

#include "../proxy/Login1User_proxy.h"
#include "../utils.h"

class Login1User final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::login1::User_proxy> {
 public:
  Login1User(sdbus::IConnection& connection,
             const sdbus::ObjectPath& objectPath)
      : ProxyInterfaces{connection,
                        sdbus::ServiceName("org.freedesktop.login1"),
                        objectPath},
        object_path_(objectPath) {
    registerProxy();
    const auto properties = this->GetAll("org.freedesktop.login1.User");
    Login1User::onPropertiesChanged(
        sdbus::InterfaceName("org.freedesktop.login1.User"), properties, {});
  }

  ~Login1User() { unregisterProxy(); }

 private:
  sdbus::ObjectPath object_path_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    std::stringstream ss;
    ss << std::endl;
    ss << "[" << interfaceName << "] Login1User Properties changed"
       << std::endl;
    Utils::append_properties(changedProperties, ss);
    for (const auto& name : invalidatedProperties) {
      ss << "[" << interfaceName << "] Invalidated property: " << name
         << std::endl;
    }
    spdlog::info("{}", ss.str());
  }
};

#endif  // SRC_LOGIN1_LOGIN1_USER_H
