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

#ifndef SRC_LOCALE1_LOCALE1_CLIENT_H
#define SRC_LOCALE1_LOCALE1_CLIENT_H

#include "../proxy/org/freedesktop/locale1/locale1_proxy.h"

class Locale1Client final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::locale1_proxy> {
 public:
  static constexpr auto INTERFACE_NAME = "org.freedesktop.locale1";
  static constexpr auto OBJECT_PATH = "/org/freedesktop/locale1";

  explicit Locale1Client(sdbus::IConnection& connection);

  virtual ~Locale1Client();

  void updateLocale1(
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties);

  void printLocale1() const;

  struct locale1 {
    std::vector<std::string> Locale;
    std::string X11Layout;
    std::string X11Model;
    std::string X11Variant;
    std::string X11Options;
    std::string VConsoleKeymap;
    std::string VConsoleKeymapToggle;
  };

  [[nodiscard]] const locale1& getLocale1() const { return locale1_; }

 private:
  locale1 locale1_{};

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;
};

#endif  // SRC_LOCALE1_LOCALE1_CLIENT_H
