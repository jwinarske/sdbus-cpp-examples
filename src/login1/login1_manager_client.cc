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

#include "login1_manager_client.h"

#include <sys/socket.h>

#include "../utils.h"

Login1ManagerClient::Login1ManagerClient(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(kBusName),
                      sdbus::ObjectPath(kObjectPath)},
      connection_(connection),
      object_path_(sdbus::ObjectPath(kObjectPath)) {
  const auto properties = this->GetAll("org.freedesktop.login1.Manager");
  Login1ManagerClient::onPropertiesChanged(
      sdbus::InterfaceName("org.freedesktop.login1.Manager"), properties, {});
  for (auto seats = this->ListSeats(); const auto& value : seats) {
    // [('seat0', '/org/freedesktop/login1/seat/seat0')]
    auto name = value.get<0>();
    auto path = value.get<1>();
    onSeatNew(name, path);
  }
  for (auto sessions = this->ListSessions(); const auto& value : sessions) {
    // [('2', 1000, 'joel', 'seat0', '/org/freedesktop/login1/session/_32')]
    auto user_name = value.get<2>();
    auto path = value.get<4>();
    onSessionNew(user_name, path);
  }
  for (auto users = this->ListUsers(); const auto& value : users) {
    // [(1000, 'joel', '/org/freedesktop/login1/user/_1000')]
    auto id = value.get<0>();
    auto path = value.get<2>();
    onUserNew(id, path);
  }
  registerProxy();
}

Login1ManagerClient::~Login1ManagerClient() {
  unregisterProxy();
  for (const auto& [objectPath, seat] : seats_) {
    if (seats_.contains(objectPath)) {
      seats_[objectPath].reset();
      seats_.erase(objectPath);
    }
  }
  for (const auto& [objectPath, session] : sessions_) {
    if (sessions_.contains(objectPath)) {
      sessions_[objectPath].reset();
      sessions_.erase(objectPath);
    }
  }
  for (const auto& [objectPath, seat] : users_) {
    if (users_.contains(objectPath)) {
      users_[objectPath].reset();
      users_.erase(objectPath);
    }
  }
}

void Login1ManagerClient::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  Utils::print_changed_properties(interfaceName, changedProperties,
                                  invalidatedProperties);
}

void Login1ManagerClient::onSessionNew(const std::string& session_id,
                                       const sdbus::ObjectPath& object_path) {
  spdlog::info("onSessionNew: {}: {}", session_id, object_path);
  if (!sessions_.contains(object_path)) {
    sessions_[object_path] =
        std::make_unique<Login1Session>(connection_, object_path);
  }
}

void Login1ManagerClient::onSessionRemoved(
    const std::string& session_id,
    const sdbus::ObjectPath& object_path) {
  spdlog::info("onSessionRemoved: {}: {}", session_id, object_path);
  if (sessions_.contains(object_path)) {
    sessions_[object_path].reset();
    sessions_.erase(object_path);
  }
}

void Login1ManagerClient::onUserNew(const uint32_t& uid,
                                    const sdbus::ObjectPath& object_path) {
  spdlog::info("onUserNew: {}: {}", uid, object_path);
  if (!users_.contains(object_path)) {
    users_[object_path] =
        std::make_unique<Login1User>(connection_, object_path);
  }
}

void Login1ManagerClient::onUserRemoved(const uint32_t& uid,
                                        const sdbus::ObjectPath& object_path) {
  spdlog::info("onUserRemoved: {}: {}", uid, object_path);
  if (users_.contains(object_path)) {
    users_[object_path].reset();
    users_.erase(object_path);
  }
}

void Login1ManagerClient::onSeatNew(const std::string& seat_id,
                                    const sdbus::ObjectPath& object_path) {
  spdlog::info("onSeatNew: {}: {}", seat_id, object_path);
  if (!seats_.contains(object_path)) {
    seats_[object_path] =
        std::make_unique<Login1Seat>(connection_, object_path);
  }
}

void Login1ManagerClient::onSeatRemoved(const std::string& seat_id,
                                        const sdbus::ObjectPath& object_path) {
  spdlog::info("onSeatRemoved: {}: {}", seat_id, object_path);
  if (seats_.contains(object_path)) {
    seats_[object_path].reset();
    seats_.erase(object_path);
  }
}

void Login1ManagerClient::onPrepareForShutdown(const bool& start) {
  spdlog::info("onPrepareForShutdown: {}", start);
}

void Login1ManagerClient::onPrepareForShutdownWithMetadata(
    const bool& start,
    const std::map<std::string, sdbus::Variant>& metadata) {
  spdlog::info("onPrepareForShutdownWithMetadata: {}", start);
  std::stringstream ss;
  ss << std::endl;
  Utils::append_properties(metadata, ss);
  spdlog::info("{}", ss.str());
}

void Login1ManagerClient::onPrepareForSleep(const bool& start) {
  spdlog::info("onPrepareForSleep: {}", start);
}
