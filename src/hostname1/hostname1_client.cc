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

#include <iomanip>

#include "../utils/utils.h"

Hostname1Client::Hostname1Client(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(INTERFACE_NAME),
                      sdbus::ObjectPath(OBJECT_PATH)} {
  registerProxy();

  if (const std::string description = Describe(); !description.empty()) {
    if (const auto s = glz::read_json<Hostname1>(description)) {
      printHostname1(s.value());
    }
  }
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
    } else if (key == "OperatingSystemHomeURL") {
      hostname1_.OperatingSystemHomeURL = value.get<std::string>();
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
      MachineID_ = value.get<std::vector<uint8_t>>();
    } else if (key == "BootID") {
      BootID_ = value.get<std::vector<uint8_t>>();
    }
  }
}

void Hostname1Client::printHostname1() const {
  std::ostringstream os;
  if (!hostname1_.Hostname.empty()) {
    os << "\tHostname: " << hostname1_.Hostname << std::endl;
  }
  if (hostname1_.StaticHostname.empty()) {
    os << "\tStaticHostname: " << hostname1_.StaticHostname << std::endl;
  }
  if (hostname1_.PrettyHostname.has_value()) {
    os << "\tPrettyHostname: " << hostname1_.PrettyHostname.value()
       << std::endl;
  }
  if (hostname1_.DefaultHostname.has_value()) {
    os << "\tDefaultHostname: " << hostname1_.DefaultHostname.value()
       << std::endl;
  }
  if (hostname1_.HostnameSource.has_value()) {
    os << "\tHostnameSource: " << hostname1_.HostnameSource.value()
       << std::endl;
  }
  if (hostname1_.IconName.has_value()) {
    os << "\tIconName: " << hostname1_.IconName.value() << std::endl;
  }
  if (hostname1_.Chassis.has_value()) {
    os << "\tChassis: " << hostname1_.Chassis.value() << std::endl;
  }
  if (hostname1_.Deployment.has_value()) {
    os << "\tDeployment: " << hostname1_.Deployment.value() << std::endl;
  }
  if (hostname1_.Location.has_value()) {
    os << "\tLocation: " << hostname1_.Location.value() << std::endl;
  }
  if (!hostname1_.KernelName.empty()) {
    os << "\tKernelName: " << hostname1_.KernelName << std::endl;
  }
  if (!hostname1_.KernelRelease.empty()) {
    os << "\tKernelRelease: " << hostname1_.KernelRelease << std::endl;
  }
  if (!hostname1_.KernelVersion.empty()) {
    os << "\tKernelVersion: " << hostname1_.KernelVersion << std::endl;
  }
  if (!hostname1_.OperatingSystemPrettyName.empty()) {
    os << "\tOperatingSystemPrettyName: "
       << hostname1_.OperatingSystemPrettyName << std::endl;
  }
  if (!hostname1_.OperatingSystemCPEName.empty()) {
    os << "\tOperatingSystemCPEName: " << hostname1_.OperatingSystemCPEName
       << std::endl;
  }
  if (hostname1_.OperatingSystemSupportEnd.has_value()) {
    os << "\tOperatingSystemSupportEnd: "
       << hostname1_.OperatingSystemSupportEnd.value() << std::endl;
  }
  if (hostname1_.OperatingSystemHomeURL.has_value()) {
    os << "\tOperatingSystemHomeUrl: "
       << hostname1_.OperatingSystemHomeURL.value() << std::endl;
  }
  if (hostname1_.HardwareVendor.has_value()) {
    os << "\tHardwareVendor: " << hostname1_.HardwareVendor.value()
       << std::endl;
  }
  if (hostname1_.HardwareModel.has_value()) {
    os << "\tHardwareModel: " << hostname1_.HardwareModel.value() << std::endl;
  }
  if (hostname1_.FirmwareVersion.has_value()) {
    os << "\tFirmwareVersion: " << hostname1_.FirmwareVersion.value()
       << std::endl;
  }
  if (hostname1_.FirmwareVendor.has_value()) {
    os << "\tFirmwareVendor: " << hostname1_.FirmwareVendor.value()
       << std::endl;
  }
  if (hostname1_.FirmwareDate.has_value()) {
    os << "\tFirmwareDate: " << hostname1_.FirmwareDate.value() << std::endl;
  }
  if (MachineID_.has_value()) {
    os << "\tMachineID: ";
    for (const auto& b : MachineID_.value()) {
      os << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b)
         << " ";
    }
    os << std::endl;
  }
  if (BootID_.has_value()) {
    os << "\tBootID: ";
    for (const auto& b : BootID_.value()) {
      os << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b)
         << " ";
    }
    os << std::endl;
  }
  spdlog::info("\n{}", os.str());
}

void Hostname1Client::printHostname1(const Hostname1& val) {
  std::ostringstream os;
  if (!val.Hostname.empty()) {
    os << "\tHostname: " << val.Hostname << std::endl;
  }
  if (val.StaticHostname.empty()) {
    os << "\tStaticHostname: " << val.StaticHostname << std::endl;
  }
  if (val.PrettyHostname.has_value()) {
    os << "\tPrettyHostname: " << val.PrettyHostname.value() << std::endl;
  }
  if (val.DefaultHostname.has_value()) {
    os << "\tDefaultHostname: " << val.DefaultHostname.value() << std::endl;
  }
  if (val.HostnameSource.has_value()) {
    os << "\tHostnameSource: " << val.HostnameSource.value() << std::endl;
  }
  if (val.IconName.has_value()) {
    os << "\tIconName: " << val.IconName.value() << std::endl;
  }
  if (val.Chassis.has_value()) {
    os << "\tChassis: " << val.Chassis.value() << std::endl;
  }
  if (val.Deployment.has_value()) {
    os << "\tDeployment: " << val.Deployment.value() << std::endl;
  }
  if (val.Location.has_value()) {
    os << "\tLocation: " << val.Location.value() << std::endl;
  }
  if (!val.KernelName.empty()) {
    os << "\tKernelName: " << val.KernelName << std::endl;
  }
  if (!val.KernelRelease.empty()) {
    os << "\tKernelRelease: " << val.KernelRelease << std::endl;
  }
  if (!val.KernelVersion.empty()) {
    os << "\tKernelVersion: " << val.KernelVersion << std::endl;
  }
  if (!val.OperatingSystemPrettyName.empty()) {
    os << "\tOperatingSystemPrettyName: " << val.OperatingSystemPrettyName
       << std::endl;
  }
  if (val.OperatingSystemReleaseData.has_value()) {
    os << "\tOperatingSystemReleaseData: " << std::endl;
    for (const auto& item : val.OperatingSystemReleaseData.value()) {
      os << "\t\t" << item << std::endl;
    }
  }
  if (!val.OperatingSystemCPEName.empty()) {
    os << "\tOperatingSystemCPEName: " << val.OperatingSystemCPEName
       << std::endl;
  }
  if (val.OperatingSystemSupportEnd.has_value()) {
    os << "\tOperatingSystemSupportEnd: "
       << val.OperatingSystemSupportEnd.value() << std::endl;
  }
  if (val.OperatingSystemHomeURL.has_value()) {
    os << "\tOperatingSystemHomeURL: " << val.OperatingSystemHomeURL.value()
       << std::endl;
  }
  if (val.HardwareVendor.has_value()) {
    os << "\tHardwareVendor: " << val.HardwareVendor.value() << std::endl;
  }
  if (val.HardwareModel.has_value()) {
    os << "\tHardwareModel: " << val.HardwareModel.value() << std::endl;
  }
  if (val.FirmwareVersion.has_value()) {
    os << "\tFirmwareVersion: " << val.FirmwareVersion.value() << std::endl;
  }
  if (val.FirmwareVendor.has_value()) {
    os << "\tFirmwareVendor: " << val.FirmwareVendor.value() << std::endl;
  }
  if (val.FirmwareDate.has_value()) {
    os << "\tFirmwareDate: " << val.FirmwareDate.value() << std::endl;
  }
  if (val.MachineID.has_value()) {
    os << "\tMachineID: " << val.MachineID.value() << std::endl;
  }
  if (val.BootID.has_value()) {
    os << "\tBootID: " << val.BootID.value() << std::endl;
  }
  spdlog::info("\n{}", os.str());
}