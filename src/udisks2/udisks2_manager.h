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

#ifndef SRC_UDISKS2_UDISKS_MANAGER_H
#define SRC_UDISKS2_UDISKS_MANAGER_H

#include "../proxy/org/freedesktop/UDisks2/Manager/manager_proxy.h"
#include "udisks2_block.h"
#include "udisks2_drive.h"
#include "udisks2_filesystem.h"
#include "udisks2_manager_nvme.h"
#include "udisks2_nvme_controller.h"
#include "udisks2_nvme_namespace.h"
#include "udisks2_partition.h"
#include "udisks2_partition_table.h"
#include "udisks2_swapspace.h"

class UDisks2Manager final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    sdbus::ObjectManager_proxy> {
 public:
  explicit UDisks2Manager(sdbus::IConnection& connection);
  virtual ~UDisks2Manager();

 private:
  static constexpr auto INTERFACE_NAME = "org.freedesktop.UDisks2";
  static constexpr auto OBJECT_PATH = "/org/freedesktop/UDisks2";

  std::unique_ptr<UDisks2ManagerNvme> manager_nvme_;

  std::map<sdbus::ObjectPath, std::unique_ptr<UDisks2Block>> blocks_;
  std::mutex block_mutex_;

  std::map<sdbus::ObjectPath, std::unique_ptr<UDisks2Drive>> drives_;
  std::mutex drive_mutex_;

  std::map<sdbus::ObjectPath, std::unique_ptr<UDisks2NvmeNamespace>>
      namespaces_;
  std::mutex namespace_mutex_;

  std::map<sdbus::ObjectPath, std::unique_ptr<UDisks2PartitionTable>>
      partition_tables_;
  std::mutex partition_table_mutex_;

  std::map<sdbus::ObjectPath, std::unique_ptr<UDisks2Partition>> partitions_;
  std::mutex partition_mutex_;

  std::map<sdbus::ObjectPath, std::unique_ptr<UDisks2Filesystem>> filesystems_;
  std::mutex filesystem_mutex_;

  std::map<sdbus::ObjectPath, std::unique_ptr<UDisks2Swapspace>> swapspaces_;
  std::mutex swapspace_mutex_;

  std::map<sdbus::ObjectPath, std::unique_ptr<UDisks2NvmeController>>
      nvme_controllers_;
  std::mutex nvme_controller_mutex_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;

  void onInterfacesAdded(
      const sdbus::ObjectPath& objectPath,
      const std::map<sdbus::InterfaceName,
                     std::map<sdbus::PropertyName, sdbus::Variant>>&
          interfacesAndProperties) override;

  void onInterfacesRemoved(
      const sdbus::ObjectPath& objectPath,
      const std::vector<sdbus::InterfaceName>& interfaces) override;
};

#endif  // SRC_UDISKS2_UDISKS_MANAGER_H
