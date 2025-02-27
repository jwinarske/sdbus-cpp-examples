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

#include "hostname1_client.h"

#include "../utils/utils.h"

Hostname1Client::Hostname1Client(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(INTERFACE_NAME),
                      sdbus::ObjectPath(OBJECT_PATH)} {
  registerProxy();
}

Hostname1Client::~Hostname1Client() {
  unregisterProxy();
}

void Hostname1Client::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  updateHostname1(changedProperties);
}

void Hostname1Client::updateHostname1(
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties) {
  for (const auto& [key, value] : changedProperties) {
    if (key == "Hostname") {
      hostname1_.Hostname = value.get<std::string>();
    } else if (key == "StaticHostname") {
      hostname1_.StaticHostname = value.get<std::string>();
    } else if (key == "PrettyHostname") {
      hostname1_.PrettyHostname = value.get<std::string>();
    } else if (key == "DefaultHostname") {
      hostname1_.DefaultHostname = value.get<std::string>();
    } else if (key == "HostnameSource") {
      hostname1_.HostnameSource = value.get<std::string>();
    } else if (key == "IconName") {
      hostname1_.IconName = value.get<std::string>();
    } else if (key == "Chassis") {
      hostname1_.Chassis = value.get<std::string>();
    } else if (key == "Deployment") {
      hostname1_.Deployment = value.get<std::string>();
    } else if (key == "Location") {
      hostname1_.Location = value.get<std::string>();
    } else if (key == "KernelName") {
      hostname1_.KernelName = value.get<std::string>();
    } else if (key == "KernelRelease") {
      hostname1_.KernelRelease = value.get<std::string>();
    } else if (key == "KernelVersion") {
      hostname1_.KernelVersion = value.get<std::string>();
    } else if (key == "OperatingSystemPrettyName") {
      hostname1_.OperatingSystemPrettyName = value.get<std::string>();
    } else if (key == "OperatingSystemCPEName") {
      hostname1_.OperatingSystemCPEName = value.get<std::string>();
    } else if (key == "OperatingSystemSupportEnd") {
      hostname1_.OperatingSystemSupportEnd = value.get<uint64_t>();
    } else if (key == "HomeURL") {
      hostname1_.HomeURL = value.get<std::string>();
    } else if (key == "HardwareVendor") {
      hostname1_.HardwareVendor = value.get<std::string>();
    } else if (key == "HardwareModel") {
      hostname1_.HardwareModel = value.get<std::string>();
    } else if (key == "FirmwareVersion") {
      hostname1_.FirmwareVersion = value.get<std::string>();
    } else if (key == "FirmwareVendor") {
      hostname1_.FirmwareVendor = value.get<std::string>();
    } else if (key == "FirmwareDate") {
      hostname1_.FirmwareDate = value.get<uint64_t>();
    } else if (key == "MachineID") {
      hostname1_.MachineID = value.get<std::vector<uint8_t>>();
    } else if (key == "BootID") {
      hostname1_.BootID = value.get<std::vector<uint8_t>>();
    }
  }
}

void Hostname1Client::printHostname1() const {
  std::stringstream ss;
  if (!hostname1_.Hostname.empty()) {
    ss << "\tHostname: " << hostname1_.Hostname << std::endl;
  }
  if (!hostname1_.StaticHostname.empty()) {
    ss << "\tStaticHostname: " << hostname1_.StaticHostname << std::endl;
  }
  if (!hostname1_.PrettyHostname.empty()) {
    ss << "\tPrettyHostname: " << hostname1_.PrettyHostname << std::endl;
  }
  if (!hostname1_.DefaultHostname.empty()) {
    ss << "\tDefaultHostname: " << hostname1_.DefaultHostname << std::endl;
  }
  if (!hostname1_.HostnameSource.empty()) {
    ss << "\tHostnameSource: " << hostname1_.HostnameSource << std::endl;
  }
  if (!hostname1_.IconName.empty()) {
    ss << "\tIconName: " << hostname1_.IconName << std::endl;
  }
  if (!hostname1_.Chassis.empty()) {
    ss << "\tChassis: " << hostname1_.Chassis << std::endl;
  }
  if (!hostname1_.Deployment.empty()) {
    ss << "\tDeployment: " << hostname1_.Deployment << std::endl;
  }
  if (!hostname1_.Location.empty()) {
    ss << "\tLocation: " << hostname1_.Location << std::endl;
  }
  if (!hostname1_.KernelName.empty()) {
    ss << "\tKernelName: " << hostname1_.KernelName << std::endl;
  }
  if (!hostname1_.KernelRelease.empty()) {
    ss << "\tKernelRelease: " << hostname1_.KernelRelease << std::endl;
  }
  if (!hostname1_.KernelVersion.empty()) {
    ss << "\tKernelVersion: " << hostname1_.KernelVersion << std::endl;
  }
  if (!hostname1_.OperatingSystemPrettyName.empty()) {
    ss << "\tOperatingSystemPrettyName: "
       << hostname1_.OperatingSystemPrettyName << std::endl;
  }
  if (!hostname1_.OperatingSystemCPEName.empty()) {
    ss << "\tOperatingSystemCPEName: " << hostname1_.OperatingSystemCPEName
       << std::endl;
  }
  if (hostname1_.OperatingSystemSupportEnd.has_value()) {
    ss << "\tOperatingSystemSupportEnd: "
       << hostname1_.OperatingSystemSupportEnd.value() << std::endl;
  }
  if (!hostname1_.HomeURL.empty()) {
    ss << "\tHomeURL: " << hostname1_.HomeURL << std::endl;
  }
  if (!hostname1_.HardwareVendor.empty()) {
    ss << "\tHardwareVendor: " << hostname1_.HardwareVendor << std::endl;
  }
  if (!hostname1_.HardwareModel.empty()) {
    ss << "\tHardwareModel: " << hostname1_.HardwareModel << std::endl;
  }
  if (!hostname1_.FirmwareVersion.empty()) {
    ss << "\tFirmwareVersion: " << hostname1_.FirmwareVersion << std::endl;
  }
  if (!hostname1_.FirmwareVendor.empty()) {
    ss << "\tFirmwareVendor: " << hostname1_.FirmwareVendor << std::endl;
  }
  if (hostname1_.FirmwareDate.has_value()) {
    ss << "\tFirmwareDate: " << hostname1_.FirmwareDate.value() << std::endl;
  }
  if (hostname1_.MachineID.has_value()) {
    ss << "\tMachineID: ";
    for (const auto& b : hostname1_.MachineID.value()) {
      ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b)
         << " ";
    }
    ss << std::endl;
  }
  if (hostname1_.BootID.has_value()) {
    ss << "\tBootID: ";
    for (const auto& b : hostname1_.BootID.value()) {
      ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b)
         << " ";
    }
    ss << std::endl;
  }
  spdlog::info("\n{}", ss.str());
}