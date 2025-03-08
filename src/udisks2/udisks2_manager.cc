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

#include "udisks2_manager.h"

#include "../utils/utils.h"
#include "udisks2_manager_nvme.h"

UDisks2Manager::UDisks2Manager(sdbus::IConnection& connection)
    : ProxyInterfaces(connection,
                      sdbus::ServiceName(INTERFACE_NAME),
                      sdbus::ObjectPath(OBJECT_PATH)) {
  registerProxy();
  for (auto properties = GetManagedObjects();
       const auto& [object, interfacesAndProperties] : properties) {
    UDisks2Manager::onInterfacesAdded(object, interfacesAndProperties);
  }
}

UDisks2Manager::~UDisks2Manager() {
  unregisterProxy();
}

void UDisks2Manager::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {}

void UDisks2Manager::onInterfacesAdded(
    const sdbus::ObjectPath& objectPath,
    const std::map<sdbus::InterfaceName,
                   std::map<sdbus::PropertyName, sdbus::Variant>>&
        interfacesAndProperties) {
  for (const auto& [interface, properties] : interfacesAndProperties) {
    std::ostringstream os;
    os << std::endl;
    Utils::append_properties(properties, os);
    spdlog::info("[{}] Add - {}\n{}", objectPath, interface, os.str());
    if ("org.freedesktop.UDisks2.Manager" == interface) {
      continue;
    }
    if ("org.freedesktop.UDisks2.Manager.NVMe" == interface) {
      manager_nvme_ = std::make_unique<UDisks2ManagerNvme>(
          getProxy().getConnection(), objectPath);
    } else if ("org.freedesktop.UDisks2.Block" == interface) {
      std::lock_guard lock(block_mutex_);
      if (!blocks_.contains(objectPath)) {
        blocks_[objectPath] = std::make_unique<UDisks2Block>(
            getProxy().getConnection(), objectPath);
      }
    } else if ("org.freedesktop.UDisks2.Drive" == interface) {
      std::lock_guard lock(drive_mutex_);
      if (!drives_.contains(objectPath)) {
        drives_[objectPath] = std::make_unique<UDisks2Drive>(
            getProxy().getConnection(), objectPath);
      }
    } else if ("org.freedesktop.UDisks2.NVMe.Namespace" == interface) {
      std::lock_guard lock(namespace_mutex_);
      if (!namespaces_.contains(objectPath)) {
        namespaces_[objectPath] = std::make_unique<UDisks2NvmeNamespace>(
            getProxy().getConnection(), objectPath);
      }
    } else if ("org.freedesktop.UDisks2.NVMe.Controller" == interface) {
      std::lock_guard lock(nvme_controller_mutex_);
      if (!nvme_controllers_.contains(objectPath)) {
        nvme_controllers_[objectPath] = std::make_unique<UDisks2NvmeController>(
            getProxy().getConnection(), objectPath);
      }
    } else if ("org.freedesktop.UDisks2.PartitionTable" == interface) {
      std::lock_guard lock(partition_table_mutex_);
      if (!partition_tables_.contains(objectPath)) {
        partition_tables_[objectPath] = std::make_unique<UDisks2PartitionTable>(
            getProxy().getConnection(), objectPath);
      }
    } else if ("org.freedesktop.UDisks2.Partition" == interface) {
      std::lock_guard lock(partition_mutex_);
      if (!partitions_.contains(objectPath)) {
        partitions_[objectPath] = std::make_unique<UDisks2Partition>(
            getProxy().getConnection(), objectPath);
      }
    } else if ("org.freedesktop.UDisks2.Filesystem" == interface) {
      std::lock_guard lock(filesystem_mutex_);
      if (!filesystems_.contains(objectPath)) {
        filesystems_[objectPath] = std::make_unique<UDisks2Filesystem>(
            getProxy().getConnection(), objectPath);
      }
    } else if ("org.freedesktop.UDisks2.Swapspace" == interface) {
      std::lock_guard lock(swapspace_mutex_);
      if (!swapspaces_.contains(objectPath)) {
        swapspaces_[objectPath] = std::make_unique<UDisks2Swapspace>(
            getProxy().getConnection(), objectPath);
      }
    } else {
      spdlog::info("not handled interface: {}", interface);
    }
  }
}

void UDisks2Manager::onInterfacesRemoved(
    const sdbus::ObjectPath& objectPath,
    const std::vector<sdbus::InterfaceName>& interfaces) {
  for (const auto& interface : interfaces) {
    spdlog::info("[{}] Remove - {}", objectPath, interface);
  }
}