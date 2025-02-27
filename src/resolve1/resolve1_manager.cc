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

#include "resolve1_manager.h"

#include "../utils.h"

Resolve1Manager::Resolve1Manager(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(kServiceName),
                      sdbus::ObjectPath(kInterfaceName)} {
  registerProxy();
  const auto properties = this->GetAll("org.freedesktop.resolve1.Manager");
  Resolve1Manager::onPropertiesChanged(
      sdbus::InterfaceName("org.freedesktop.resolve1.Manager"), properties, {});
}

Resolve1Manager::~Resolve1Manager() {
  unregisterProxy();
}

void Resolve1Manager::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  Utils::print_changed_properties(interfaceName, changedProperties,
                                  invalidatedProperties);
}
