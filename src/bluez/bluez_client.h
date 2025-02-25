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

#ifndef SRC_BLUEZ_BLUEZ_CLIENT_H
#define SRC_BLUEZ_BLUEZ_CLIENT_H

#include "adapter1.h"
#include "battery_provider_manager1.h"
#include "device1.h"
#include "gatt_manager1.h"
#include "le_advertising_manager1.h"
#include "media1.h"
#include "network_server1.h"

class BluezClient final
    : public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy> {
 public:
  explicit BluezClient(sdbus::IConnection& connection);

  virtual ~BluezClient();

 private:
  static constexpr char INTERFACE_NAME[] = "org.bluez";
  sdbus::IConnection& connection_;

  std::map<sdbus::ObjectPath, std::unique_ptr<Adapter1>> adapters_;
  std::map<sdbus::ObjectPath, std::unique_ptr<Device1>> devices_;

  std::mutex adapter_mutex_;
  std::mutex device_mutex_;

  std::unique_ptr<BatteryProviderManager1> battery_provider_manager1_;
  std::unique_ptr<GattManager1> gatt_manager1_;
  std::unique_ptr<LEAdvertisingManager1> le_advertising_manager1_;
  std::unique_ptr<Media1> media1_;
  std::unique_ptr<NetworkServer1> network_server1_;

  void onInterfacesAdded(
      const sdbus::ObjectPath& objectPath,
      const std::map<sdbus::InterfaceName,
                     std::map<sdbus::PropertyName, sdbus::Variant>>&
          interfacesAndProperties) override;

  void onInterfacesRemoved(
      const sdbus::ObjectPath& objectPath,
      const std::vector<sdbus::InterfaceName>& interfaces) override;
};

#endif  // SRC_BLUEZ_BLUEZ_CLIENT_H
