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

#ifndef SRC_BLUEZ_GATT_DESCRIPTOR1_H
#define SRC_BLUEZ_GATT_DESCRIPTOR1_H

#include "../proxy/org/bluez/GattDescriptor1/gatt_descriptor1_proxy.h"

#include "../utils.h"

class GattDescriptor1 final
    : public sdbus::ProxyInterfaces<org::bluez::GattDescriptor1_proxy> {
 public:
  GattDescriptor1(sdbus::IConnection& connection,
                  const sdbus::ServiceName(&destination),
                  const sdbus::ObjectPath(&objectPath))
      : ProxyInterfaces{connection, destination, objectPath},
        object_path_(objectPath) {}

  ~GattDescriptor1() = default;

 private:
  sdbus::ObjectPath object_path_;
};

#endif  // SRC_BLUEZ_GATT_DESCRIPTOR1_H
