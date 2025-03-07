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

#include <iomanip>

#include "../utils/utils.h"

Timedate1Client::Timedate1Client(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(INTERFACE_NAME),
                      sdbus::ObjectPath(OBJECT_PATH)} {
  registerProxy();
}

Timedate1Client::~Timedate1Client() {
  unregisterProxy();
}

void Timedate1Client::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  Utils::print_changed_properties(interfaceName, changedProperties,
                                  invalidatedProperties);
}

void Timedate1Client::updateTimedate1(
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties) {
  for (const auto& [key, value] : changedProperties) {
    if (key == "Timezone") {
      timedate1_.Timezone = value.get<std::string>();
    } else if (key == "LocalRTC") {
      timedate1_.LocalRTC = value.get<bool>();
    } else if (key == "CanNTP") {
      timedate1_.CanNTP = value.get<bool>();
    } else if (key == "NTP") {
      timedate1_.NTP = value.get<bool>();
    } else if (key == "NTPSynchronized") {
      timedate1_.NTPSynchronized = value.get<bool>();
    } else if (key == "TimeUSec") {
      timedate1_.TimeUSec = value.get<uint64_t>();
    } else if (key == "RTCTimeUSec") {
      timedate1_.RTCTimeUSec = value.get<uint64_t>();
    }
  }
}

void appendTimeUSecAsDate(const uint64_t timeUSec, std::stringstream& ss) {
  const auto timePoint = std::chrono::system_clock::time_point(
      std::chrono::microseconds(timeUSec));
  const std::time_t timeT = std::chrono::system_clock::to_time_t(timePoint);
  const std::tm* tm = std::localtime(&timeT);
  ss << "\tDate: " << std::put_time(tm, "%d-%m-%Y %H:%M:%S") << std::endl;
}

void Timedate1Client::printTimedate1() const {
  std::stringstream ss;

  if (!timedate1_.Timezone.empty()) {
    ss << "Timezone:" << timedate1_.Timezone << std::endl;
  }
  ss << "\tLocalRTC: " << (timedate1_.LocalRTC ? "True" : "False") << std::endl;
  ss << "\tCanNTP: " << (timedate1_.CanNTP ? "True" : "False") << std::endl;
  ss << "\tNTP: " << (timedate1_.NTP ? "True" : "False") << std::endl;
  ss << "\tNTPSynchronized: " << (timedate1_.NTPSynchronized ? "True" : "False")
     << std::endl;
  ss << "\tNTP: " << (timedate1_.NTP ? "True" : "False") << std::endl;
  ss << "\tTimeUSec: " << timedate1_.TimeUSec << std::endl;
  appendTimeUSecAsDate(timedate1_.TimeUSec, ss);
  ss << "\tRTCTimeUSec: " << timedate1_.RTCTimeUSec << std::endl;
  appendTimeUSecAsDate(timedate1_.RTCTimeUSec, ss);

  spdlog::info("\n{}", ss.str());
}