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

#include "fwupd_client.h"

#include "../utils/utils.h"

FwupdClient::FwupdClient(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(INTERFACE_NAME),
                      sdbus::ObjectPath(OBJECT_PATH)} {
  registerProxy();
}

FwupdClient::~FwupdClient() {
  unregisterProxy();
}

void FwupdClient::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  Utils::print_changed_properties(interfaceName, changedProperties,
                                  invalidatedProperties);
}

void FwupdClient::onChanged() {
  spdlog::info("onChanged");
}

void FwupdClient::onDeviceAdded(
    const std::map<std::string, sdbus::Variant>& device) {
  std::ostringstream os;
  os << std::endl << "[FwupdClient] onDeviceAdded" << std::endl;
  Utils::append_properties(device, os);
  spdlog::info(os.str());
}

void FwupdClient::onDeviceRemoved(
    const std::map<std::string, sdbus::Variant>& device) {
  std::ostringstream os;
  os << std::endl << "[FwupdClient] onDeviceRemoved" << std::endl;
  Utils::append_properties(device, os);
  spdlog::info(os.str());
}

void FwupdClient::onDeviceChanged(
    const std::map<std::string, sdbus::Variant>& device) {
  std::ostringstream os;
  os << std::endl << "[FwupdClient] onDeviceChanged" << std::endl;
  Utils::append_properties(device, os);
  spdlog::info(os.str());
}

void FwupdClient::onDeviceRequest(
    const std::map<std::string, sdbus::Variant>& request) {
  std::ostringstream os;
  os << std::endl << "[FwupdClient] onDeviceRequest" << std::endl;
  Utils::append_properties(request, os);
  spdlog::info(os.str());
}
