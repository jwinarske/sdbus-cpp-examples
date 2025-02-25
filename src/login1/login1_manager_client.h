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

#ifndef SRC_LOGIN1_LOGIN1_MANAGER_CLIENT_H
#define SRC_LOGIN1_LOGIN1_MANAGER_CLIENT_H

#include "../proxy/org/freedesktop/login1/Manager/manager_proxy.h"

#include "login1_seat.h"
#include "login1_session.h"
#include "login1_user.h"

class Login1ManagerClient final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::login1::Manager_proxy> {
 public:
  explicit Login1ManagerClient(sdbus::IConnection& connection);

  virtual ~Login1ManagerClient();

 private:
  static constexpr auto kBusName = "org.freedesktop.login1";
  static constexpr auto kObjectPath = "/org/freedesktop/login1";

  sdbus::IConnection& connection_;
  sdbus::ObjectPath object_path_;

  std::map<sdbus::ObjectPath, std::unique_ptr<Login1Seat>> seats_;
  std::map<sdbus::ObjectPath, std::unique_ptr<Login1Session>> sessions_;
  std::map<sdbus::ObjectPath, std::unique_ptr<Login1User>> users_;

  void onSessionNew(const std::string& session_id,
                    const sdbus::ObjectPath& object_path) override;

  void onSessionRemoved(const std::string& session_id,
                        const sdbus::ObjectPath& object_path) override;

  void onUserNew(const uint32_t& uid,
                 const sdbus::ObjectPath& object_path) override;

  void onUserRemoved(const uint32_t& uid,
                     const sdbus::ObjectPath& object_path) override;

  void onSeatNew(const std::string& seat_id,
                 const sdbus::ObjectPath& object_path) override;

  void onSeatRemoved(const std::string& seat_id,
                     const sdbus::ObjectPath& object_path) override;

  void onPrepareForShutdown(const bool& start) override;

  void onPrepareForShutdownWithMetadata(
      const bool& start,
      const std::map<std::string, sdbus::Variant>& metadata) override;

  void onPrepareForSleep(const bool& start) override;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;
};

#endif  // SRC_LOGIN1_LOGIN1_MANAGER_CLIENT_H
