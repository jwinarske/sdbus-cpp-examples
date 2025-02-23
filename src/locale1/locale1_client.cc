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

#include "locale1_client.h"

#include "../utils.h"

Locale1Client::Locale1Client(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(kServiceName),
                      sdbus::ObjectPath(kInterfaceName)},
      connection_(connection),
      object_path_(sdbus::ObjectPath(kInterfaceName)) {
  const auto properties = this->GetAll("org.freedesktop.locale1");
  Locale1Client::onPropertiesChanged(
      sdbus::InterfaceName("org.freedesktop.locale1"), properties, {});
  registerProxy();
}

Locale1Client::~Locale1Client() {
  unregisterProxy();
}

void Locale1Client::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  std::stringstream ss;
  ss << std::endl;
  ss << "[" << interfaceName << "] Locale1Client Properties changed"
     << std::endl;
  Utils::append_properties(changedProperties, ss);
  for (const auto& name : invalidatedProperties) {
    ss << "[" << interfaceName << "] Invalidated property: " << name
       << std::endl;
  }
  spdlog::info("{}", ss.str());
}
