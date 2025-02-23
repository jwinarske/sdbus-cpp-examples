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

#include "../proxy/Locale1_proxy.h"

static constexpr auto kServiceName = "org.freedesktop.locale1";
static constexpr auto kInterfaceName = "/org/freedesktop/locale1";

class Locale1Client final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::locale1_proxy> {
 public:
  explicit Locale1Client(sdbus::IConnection& connection);

  ~Locale1Client();

 private:
  sdbus::IConnection& connection_;
  sdbus::ObjectPath object_path_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;
};

#endif  // SRC_LOCALE1_LOCALE1_CLIENT_H
