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

#include "../utils/utils.h"

Locale1Client::Locale1Client(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(INTERFACE_NAME),
                      sdbus::ObjectPath(OBJECT_PATH)} {
  registerProxy();
}

Locale1Client::~Locale1Client() {
  unregisterProxy();
}

void Locale1Client::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  updateLocale1(changedProperties);
}

void Locale1Client::updateLocale1(
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties) {
  for (const auto& [key, value] : changedProperties) {
    if (key == "Locale") {
      locale1_.Locale = value.get<std::vector<std::string>>();
    } else if (key == "X11Layout") {
      locale1_.X11Layout = value.get<std::string>();
    } else if (key == "X11Model") {
      locale1_.X11Model = value.get<std::string>();
    } else if (key == "X11Variant") {
      locale1_.X11Variant = value.get<std::string>();
    } else if (key == "X11Options") {
      locale1_.X11Options = value.get<std::string>();
    } else if (key == "VConsoleKeymap") {
      locale1_.VConsoleKeymap = value.get<std::string>();
    } else if (key == "VConsoleKeymapToggle") {
      locale1_.VConsoleKeymapToggle = value.get<std::string>();
    }
  }
}

void Locale1Client::printLocale1() const {
  std::ostringstream os;

  if (!locale1_.Locale.empty()) {
    os << "Locale:" << std::endl;
    for (const auto& s : locale1_.Locale) {
      os << "\t" << s << " ";
    }
    os << std::endl;
  }
  if (!locale1_.X11Layout.empty()) {
    os << "\tX11Layout: " << locale1_.X11Layout << std::endl;
  }
  if (!locale1_.X11Model.empty()) {
    os << "\tX11Model: " << locale1_.X11Model << std::endl;
  }
  if (!locale1_.X11Variant.empty()) {
    os << "\tX11Variant: " << locale1_.X11Variant << std::endl;
  }
  if (!locale1_.X11Options.empty()) {
    os << "\tX11Options: " << locale1_.X11Options << std::endl;
  }
  if (!locale1_.VConsoleKeymap.empty()) {
    os << "\tVConsoleKeymap: " << locale1_.VConsoleKeymap << std::endl;
  }
  if (!locale1_.VConsoleKeymapToggle.empty()) {
    os << "\tVConsoleKeymapToggle: " << locale1_.VConsoleKeymapToggle
       << std::endl;
  }

  spdlog::info("\n{}", os.str());
}