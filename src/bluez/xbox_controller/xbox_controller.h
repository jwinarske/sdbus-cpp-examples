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

#ifndef SRC_BLUEZ_XBOX_CONTROLLER_XBOX_CONTROLLER_H
#define SRC_BLUEZ_XBOX_CONTROLLER_XBOX_CONTROLLER_H

#include <vector>

#include "../../upower/upower_client.h"
#include "../adapter1.h"
#include "../device1.h"
#include "../input1.h"
#include "../udev_monitor.hpp"

#include "input_reader.h"

class UPowerDisplayDevice;
class XboxController final
    : public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy>,
      public Hidraw,
      public UdevMonitor {
 public:
  explicit XboxController(sdbus::IConnection& connection);

  ~XboxController() override;

 private:
  const std::string VENDOR_ID = "045E";
  const std::string PRODUCT_ID0 = "02FD";
  const std::string PRODUCT_ID1 = "0B13";

  static constexpr auto INTERFACE_NAME = "org.bluez";
  static constexpr auto PROPERTIES_INTERFACE_NAME =
      "org.freedesktop.DBus.Properties";
  static constexpr auto INTROSPECTABLE_INTERFACE_NAME =
      "org.freedesktop.DBus.Introspectable";

  std::mutex adapters_mutex_;
  std::map<sdbus::ObjectPath, std::unique_ptr<Adapter1>> adapters_;

  std::mutex devices_mutex_;
  std::map<sdbus::ObjectPath, std::unique_ptr<Device1>> devices_;

  std::mutex input1_mutex_;
  std::map<sdbus::ObjectPath, std::unique_ptr<Input1>> input1_;

  std::mutex upower_display_devices_mutex_;
  std::map<std::string, std::unique_ptr<UPowerClient>> upower_clients_;

  std::unique_ptr<InputReader> input_reader_;

  void onInterfacesAdded(
      const sdbus::ObjectPath& objectPath,
      const std::map<sdbus::InterfaceName,
                     std::map<sdbus::PropertyName, sdbus::Variant>>&
          interfacesAndProperties) override;

  void onInterfacesRemoved(
      const sdbus::ObjectPath& objectPath,
      const std::vector<sdbus::InterfaceName>& interfaces) override;

  static std::string convert_mac_to_upower_path(const std::string& mac_address);
};

#endif  // SRC_BLUEZ_XBOX_CONTROLLER_XBOX_CONTROLLER_H
