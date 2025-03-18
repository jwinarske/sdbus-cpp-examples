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

#include "horipad_steam.h"

#include <poll.h>

#include "../hidraw.hpp"

const std::vector<std::pair<std::string, std::string>> input_match_params_bt = {
    {"ID_BUS", "bluetooth"},
    {"NAME", "\"HORIPAD STEAM\""},
    {"TAGS", ":seat:"}};

const std::vector<std::pair<std::string, std::string>> input_match_params_usb =
    {{"ID_BUS", "usb"},
     {"ID_MODEL", "HORIPAD_STEAM"},
     {"ID_VENDOR_ID", "0f0d"},
     {"ID_MODEL_ID", "01ab"},
     {"TAGS", ":seat:"}};

HoripadSteam::HoripadSteam(sdbus::IConnection& connection)
    : ProxyInterfaces(connection,
                      sdbus::ServiceName(INTERFACE_NAME),
                      sdbus::ObjectPath("/")),
      UdevMonitor({"hidraw", "input"},
                  [&](const char* action,
                      const char* dev_node,
                      const char* sub_system) {
                    spdlog::debug("Action: {}, Device: {}, Subsystem: {}",
                                  action ? action : "",
                                  dev_node ? dev_node : "",
                                  sub_system ? sub_system : "");
                    if (std::strcmp(sub_system, "hidraw") == 0) {
                      if (std::strcmp(action, "remove") == 0) {
                        input_reader_->stop();
                        input_reader_.reset();
                      }
                      if (!get_hidraw_devices(input_match_params_bt)) {
                        get_hidraw_devices(input_match_params_usb);
                      }
                    }
                  }) {
  if (!get_hidraw_devices(input_match_params_bt)) {
    get_hidraw_devices(input_match_params_usb);
  }

  registerProxy();
  for (const auto& [object, interfacesAndProperties] : GetManagedObjects()) {
    onInterfacesAdded(object, interfacesAndProperties);
  }
}

HoripadSteam::~HoripadSteam() {
  stop();
  unregisterProxy();
}

void HoripadSteam::onInterfacesAdded(
    const sdbus::ObjectPath& objectPath,
    const std::map<sdbus::InterfaceName,
                   std::map<sdbus::PropertyName, sdbus::Variant>>&
        interfacesAndProperties) {
  for (const auto& [interface, properties] : interfacesAndProperties) {
    if (interface == PROPERTIES_INTERFACE_NAME ||
        interface == INTROSPECTABLE_INTERFACE_NAME) {
      continue;
    }
    if (interface == org::bluez::Adapter1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(adapters_mutex_);
      if (!adapters_.contains(objectPath)) {
        auto adapter1 = std::make_unique<Adapter1>(
            getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
            objectPath, properties);
        adapters_[objectPath] = std::move(adapter1);
      }
    } else if (interface == org::bluez::Device1_proxy::INTERFACE_NAME) {
      auto mod_alias_key = sdbus::MemberName("Modalias");
      if (!properties.contains(mod_alias_key))
        continue;

      auto mod_alias = Device1::parse_modalias(
          properties.at(mod_alias_key).get<std::string>());
      if (mod_alias.has_value()) {
        spdlog::debug("VID: {}, PID: {}, DID: {}", mod_alias.value().vid,
                      mod_alias.value().pid, mod_alias.value().did);
        if (auto [vid, pid, did] = mod_alias.value();
            vid != VENDOR_ID || pid != PRODUCT_ID) {
          continue;
        }
        spdlog::debug("VID: {}, PID: {}, DID: {}", mod_alias.value().vid,
                      mod_alias.value().pid, mod_alias.value().did);
      } else {
        spdlog::debug("modalias has no value assigned: {}", objectPath);
        continue;
      }

      std::scoped_lock lock(devices_mutex_);
      if (!devices_.contains(objectPath)) {
        auto device = std::make_unique<Device1>(
            getProxy().getConnection(), sdbus::ServiceName(INTERFACE_NAME),
            objectPath, properties);

        if (auto props = device->GetProperties(); props.modalias.has_value()) {
          auto [vid, pid, did] = props.modalias.value();
          spdlog::info("Adding: {}, {}, {}", vid, pid, did);
          if (vid == VENDOR_ID && pid == PRODUCT_ID) {
            if (props.connected && props.paired && props.trusted) {
              const auto dev_key =
                  create_device_key_from_serial_number(props.address);
              HidDevicesLock();
              if (HidDevicesContains(dev_key)) {
                spdlog::info("Adding hidraw device: {}", dev_key);
                if (!input_reader_) {
                  input_reader_ =
                      std::make_unique<InputReader>(GetHidDevice(dev_key));
                  input_reader_->start();
                }
              }
              HidDevicesUnlock();
            }
          }
        }

        devices_[objectPath] = std::move(device);
      }
    } else if (interface == org::bluez::Input1_proxy::INTERFACE_NAME) {
      std::lock_guard lock(input1_mutex_);
      if (!input1_.contains(objectPath)) {
        input1_[objectPath] = std::make_unique<Input1>(
            getProxy().getConnection(),
            sdbus::ServiceName(org::bluez::Input1_proxy::INTERFACE_NAME),
            objectPath);
      }
    }
  }
}

void HoripadSteam::onInterfacesRemoved(
    const sdbus::ObjectPath& objectPath,
    const std::vector<sdbus::InterfaceName>& interfaces) {
  for (const auto& interface : interfaces) {
    if (interface == org::bluez::Adapter1_proxy::INTERFACE_NAME) {
      std::scoped_lock lock(adapters_mutex_);
      if (!adapters_.contains(objectPath)) {
        if (adapters_.contains(objectPath)) {
          adapters_[objectPath].reset();
          adapters_.erase(objectPath);
        }
      }
    } else if (interface == org::bluez::Device1_proxy::INTERFACE_NAME) {
      std::scoped_lock devices_lock(devices_mutex_);
      if (!devices_.contains(objectPath)) {
        if (devices_.contains(objectPath)) {
          devices_[objectPath].reset();
          devices_.erase(objectPath);
        }
      }
    } else if (interface == org::bluez::Input1_proxy::INTERFACE_NAME) {
      std::lock_guard lock(input1_mutex_);
      if (!input1_.contains(objectPath)) {
        input1_[objectPath].reset();
        input1_.erase(objectPath);
      }
    }
  }
  for (auto it = interfaces.begin(); it != interfaces.end(); ++it) {
    std::scoped_lock lock(devices_mutex_);
    if (devices_.contains(objectPath)) {
      auto& device = devices_[objectPath];
      device.reset();
      devices_.erase(objectPath);
    }
  }
}
