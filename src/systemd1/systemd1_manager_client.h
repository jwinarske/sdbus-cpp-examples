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

#ifndef SRC_SYSTEMD1_SYSTEMD1_MANAGER_CLIENT_H
#define SRC_SYSTEMD1_SYSTEMD1_MANAGER_CLIENT_H

#include "../proxy/org/freedesktop/systemd1/Manager/manager_proxy.h"
#include "../proxy/org/freedesktop/systemd1/Unit/unit_proxy.h"
#include "../utils/utils.h"

class Systemd1ManagerClient final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::systemd1::Manager_proxy> {
 public:
  static constexpr auto SERVICE_NAME = "org.freedesktop.systemd1";
  static constexpr auto OBJECT_PATH = "/org/freedesktop/systemd1";

  explicit Systemd1ManagerClient(sdbus::IConnection& connection);
  ~Systemd1ManagerClient() override;

  // Implement Properties_proxy pure virtual to avoid abstract type
  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;

  void onUnitNew(const std::string& id, const sdbus::ObjectPath& unit) override;
  void onUnitRemoved(const std::string& id,
                     const sdbus::ObjectPath& unit) override;
  void onJobNew(const uint32_t& id,
                const sdbus::ObjectPath& job,
                const std::string& unit) override;
  void onJobRemoved(const uint32_t& id,
                    const sdbus::ObjectPath& job,
                    const std::string& unit,
                    const std::string& result) override;
  void onStartupFinished(const uint64_t& firmware,
                         const uint64_t& loader,
                         const uint64_t& kernel,
                         const uint64_t& initrd,
                         const uint64_t& userspace,
                         const uint64_t& total) override;
  void onUnitFilesChanged() override;
  void onReloading(const bool& active) override;

 private:
  std::vector<std::string> activeUnits_;
};

#endif  // SRC_SYSTEMD1_SYSTEMD1_MANAGER_CLIENT_H