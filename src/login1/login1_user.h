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

#include "../proxy/org/freedesktop/login1/User/user_proxy.h"
#include "../utils/utils.h"

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
    {
      const auto props = this->GetAllAsync(
          User_proxy::INTERFACE_NAME,
          [this](std::optional<sdbus::Error> error,
              std::map<sdbus::PropertyName, sdbus::Variant> values) {
            if (!error) {
              onPropertiesChanged(
                  sdbus::InterfaceName(User_proxy::INTERFACE_NAME), values, {});
            } else
              spdlog::error("login1.User: {} - {}", error->getName(),
                            error->getMessage());
          });
    }
  }

  virtual ~Login1User() { unregisterProxy(); }

  void printProperties() {
    try {
      auto props = this->GetAll(User_proxy::INTERFACE_NAME);
      std::ostringstream os;
      os << std::endl;
      os << "========================================" << std::endl;
      os << "USER: " << object_path_ << std::endl;
      os << "========================================" << std::endl;
      Utils::append_properties(props, os);
      os << "========================================" << std::endl;
      spdlog::info(os.str());
    } catch (const sdbus::Error& e) {
      spdlog::error("Failed to get user properties for {}: {} - {}",
                    object_path_, e.getName(), e.getMessage());
    }
  }

 private:
  sdbus::ObjectPath object_path_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    Utils::print_changed_properties(interfaceName, changedProperties,
                                    invalidatedProperties);
    if (!changedProperties.empty()) {
      printProperties();
    }
  }
};

#endif  // SRC_LOGIN1_LOGIN1_USER_H
