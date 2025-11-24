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

  struct Hostname1 {
    std::string Hostname;
    std::string StaticHostname;
    std::optional<std::string> PrettyHostname;
    std::optional<std::string> DefaultHostname;
    std::optional<std::string> HostnameSource;
    std::optional<std::string> IconName;
    std::optional<std::string> Chassis;
    std::optional<std::string> ChassisAssetTag;
    std::optional<std::string> Deployment;
    std::optional<std::string> Location;
    std::string KernelName;
    std::string KernelRelease;
    std::string KernelVersion;
    std::string OperatingSystemPrettyName;
    std::string OperatingSystemCPEName;
    std::optional<std::string> OperatingSystemHomeURL;
    std::optional<uint64_t> OperatingSystemSupportEnd;
    std::optional<std::vector<std::string>> OperatingSystemReleaseData;
    std::optional<std::string> OperatingSystemImageID;
    std::optional<std::string> OperatingSystemImageVersion;
    std::optional<std::vector<std::string>> MachineInformationData;
    std::optional<std::string> HardwareVendor;
    std::optional<std::string> HardwareModel;
    std::optional<std::string> HardwareSerial;
    std::optional<std::string> HardwareSKU;
    std::optional<std::string> HardwareVersion;
    std::optional<std::string> FirmwareVersion;
    std::optional<std::string> FirmwareVendor;
    std::optional<uint64_t> FirmwareDate;
    std::optional<std::string> MachineID;
    std::optional<std::string> BootID;
    std::optional<std::string> ProductUUID;
    std::optional<std::string> VSockCID;
  };

  explicit Hostname1Client(sdbus::IConnection& connection);

  virtual ~Hostname1Client();

  void updateHostname1(
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties);

  void printHostname1() const;
  static void printHostname1(const Hostname1& val);

  [[nodiscard]] const Hostname1& getHostname1() const { return hostname1_; }

 private:
  Hostname1 hostname1_{};

  std::optional<std::vector<uint8_t>> MachineID_;
  std::optional<std::vector<uint8_t>> BootID_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;
};

#endif  // SRC_LOGIN1_LOGIN1_MANAGER_CLIENT_H
