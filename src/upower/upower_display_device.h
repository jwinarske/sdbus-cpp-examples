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

#ifndef SRC_UPOWER_UPOWER_DISPLAY_DEVICE_H
#define SRC_UPOWER_UPOWER_DISPLAY_DEVICE_H

#include "../proxy/org/freedesktop/UPower/DisplayDevice/display_device_proxy.h"

class UPowerDisplayDevice final
    : public sdbus::ProxyInterfaces<org::freedesktop::UPower::Device_proxy,
                                    sdbus::Properties_proxy> {
 public:
  explicit UPowerDisplayDevice(sdbus::IConnection& connection,
                               const sdbus::ObjectPath& objectPath);

  virtual ~UPowerDisplayDevice();

 private:
  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;
};

#endif  // SRC_UPOWER_UPOWER_DISPLAY_DEVICE_H
