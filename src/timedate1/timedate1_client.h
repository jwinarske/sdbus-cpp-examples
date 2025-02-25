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

#ifndef SRC_TIMEDATE1_TIMEDATE1_CLIENT_H
#define SRC_TIMEDATE1_TIMEDATE1_CLIENT_H

#include "../proxy/org/freedesktop/timedate1/timedate1_proxy.h"

class Timedate1Client final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::timedate1_proxy> {
 public:
  static constexpr auto INTERFACE_NAME = "org.freedesktop.timedate1";
  static constexpr auto OBJECT_PATH = "/org/freedesktop/timedate1";

  explicit Timedate1Client(sdbus::IConnection& connection);

  virtual ~Timedate1Client();

  void updateTimedate1(
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties);

  void printTimedate1() const;

  struct timedate1 {
    std::string Timezone;
    bool LocalRTC;
    bool CanNTP;
    bool NTP;
    bool NTPSynchronized;
    uint64_t TimeUSec;
    uint64_t RTCTimeUSec;
  };

  [[nodiscard]] const timedate1& getLocale1() const { return timedate1_; }

 private:
  timedate1 timedate1_{};

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;
};

#endif  // SRC_TIMEDATE1_TIMEDATE1_CLIENT_H
