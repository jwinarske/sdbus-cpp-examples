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

#include "../proxy/org/freedesktop/hostname1/hostname1_proxy.h"

#include <chrono>

class Hostname1Client final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::hostname1_proxy> {
 public:
  static constexpr auto INTERFACE_NAME = "org.freedesktop.hostname1";
  static constexpr auto OBJECT_PATH = "/org/freedesktop/hostname1";

  explicit Hostname1Client(sdbus::IConnection& connection);

  virtual ~Hostname1Client();

  void updateHostname1(
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties);

  void printHostname1() const;

  struct hostname1 {
    std::string Hostname;
    std::string StaticHostname;
    std::string PrettyHostname;
    std::string DefaultHostname;
    std::string HostnameSource;
    std::string IconName;
    std::string Chassis;
    std::string Deployment;
    std::string Location;
    std::string KernelName;
    std::string KernelRelease;
    std::string KernelVersion;
    std::string OperatingSystemPrettyName;
    std::string OperatingSystemCPEName;
    std::optional<uint64_t> OperatingSystemSupportEnd;
    std::string HomeURL;
    std::string HardwareVendor;
    std::string HardwareModel;
    std::string FirmwareVersion;
    std::string FirmwareVendor;
    std::optional<uint64_t> FirmwareDate;
    std::optional<std::vector<uint8_t>> MachineID;
    std::optional<std::vector<uint8_t>> BootID;
  };

  [[nodiscard]] const hostname1& getHostname1() const { return hostname1_; }

 private:
  hostname1 hostname1_{};

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;
};

#endif  // SRC_LOGIN1_LOGIN1_MANAGER_CLIENT_H
