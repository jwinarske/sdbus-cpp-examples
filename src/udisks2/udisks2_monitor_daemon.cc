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

#include <sdbus-c++/sdbus-c++.h>

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "../proxy/org/freedesktop/UDisks2/Block/block_proxy.h"

#include <spdlog/spdlog.h>

// Forward declarations
class RemovableDeviceMonitor;

// Callback type that users can customize
using DeviceReadyCallback = std::function<void(
    const sdbus::ObjectPath& blockPath,
    const std::map<sdbus::PropertyName, sdbus::Variant>& blockProperties,
    const std::map<sdbus::PropertyName, sdbus::Variant>& driveProperties,
    const std::map<sdbus::PropertyName, sdbus::Variant>& filesystemProperties)>;

// Helper class to monitor a specific block device for filesystem readiness
class BlockDeviceWatcher final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::UDisks2::Block_proxy> {
 public:
  BlockDeviceWatcher(sdbus::IConnection& connection,
                     const sdbus::ObjectPath& objectPath,
                     sdbus::ObjectPath drivePath,
                     DeviceReadyCallback callback)
      : ProxyInterfaces(connection,
                        sdbus::ServiceName(kInterfaceName),
                        objectPath),
        drive_path_(std::move(drivePath)),
        callback_(std::move(callback)),
        object_path_(objectPath) {
    registerProxy();
    checkAndReportIfReady();
  }

  ~BlockDeviceWatcher() override { unregisterProxy(); }

 private:
  static constexpr char kInterfaceName[] = "org.freedesktop.UDisks2";
  static constexpr char kBlockInterface[] = "org.freedesktop.UDisks2.Block";
  static constexpr char kFilesystemInterface[] =
      "org.freedesktop.UDisks2.Filesystem";

  sdbus::ObjectPath drive_path_;
  DeviceReadyCallback callback_;
  sdbus::ObjectPath object_path_;
  bool reported_ = false;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    if (const sdbus::InterfaceName blockInterfaceName(kBlockInterface);
        interfaceName == blockInterfaceName && !reported_) {
      // Check if IdUsage or other relevant properties changed
      const sdbus::PropertyName idUsageProp("IdUsage");
      if (const sdbus::PropertyName idTypeProp("IdType");
          changedProperties.contains(idUsageProp) ||
          changedProperties.contains(idTypeProp)) {
        checkAndReportIfReady();
      }
    }
  }

  void checkAndReportIfReady() {
    if (reported_) {
      return;
    }

    try {
      // Get block properties
      const sdbus::InterfaceName blockInterfaceName(kBlockInterface);
      auto blockProps = GetAll(blockInterfaceName);

      // Check if this is a filesystem
      std::string idUsage;
      const sdbus::PropertyName idUsageProp("IdUsage");
      if (const auto it = blockProps.find(idUsageProp);
          it != blockProps.end()) {
        idUsage = it->second.get<std::string>();
      }

      // Report if it's a filesystem or already has mount points
      const bool isFilesystem = (idUsage == "filesystem");
      bool hasFilesystem = false;

      // Try to check if Filesystem interface is available
      std::map<sdbus::PropertyName, sdbus::Variant> filesystemProps;
      try {
        const sdbus::InterfaceName filesystemInterfaceName(
            kFilesystemInterface);
        filesystemProps = GetAll(filesystemInterfaceName);
        hasFilesystem = true;
      } catch (...) {
        // Filesystem interface not available yet
      }

      if (isFilesystem || hasFilesystem) {
        // Get drive properties
        std::map<sdbus::PropertyName, sdbus::Variant> driveProps;
        try {
          const auto driveProxy = sdbus::createProxy(
              getProxy().getConnection(), sdbus::ServiceName(kInterfaceName),
              drive_path_);
          const sdbus::InterfaceName driveInterfaceName(
              "org.freedesktop.UDisks2.Drive");
          driveProps =
              driveProxy->getAllProperties().onInterface(driveInterfaceName);
        } catch (const std::exception& e) {
          spdlog::warn("Failed to get drive properties: {}", e.what());
        }

        // Report the device
        reported_ = true;
        if (callback_) {
          callback_(object_path_, blockProps, driveProps, filesystemProps);
        }
      }
    } catch (const std::exception& e) {
      spdlog::debug("Error checking device readiness: {}", e.what());
    }
  }
};

// Main monitor class
class RemovableDeviceMonitor final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    sdbus::ObjectManager_proxy> {
 public:
  explicit RemovableDeviceMonitor(sdbus::IConnection& connection,
                                  DeviceReadyCallback callback)
      : ProxyInterfaces(connection,
                        sdbus::ServiceName(kInterfaceName),
                        sdbus::ObjectPath(kObjectPath)),
        callback_(std::move(callback)) {
    registerProxy();

    // Process existing objects
    spdlog::info("Scanning for existing removable devices...");
    for (auto managedObjects = GetManagedObjects();
         const auto& [objectPath, interfacesAndProperties] : managedObjects) {
      onInterfacesAdded(objectPath, interfacesAndProperties);
    }
    spdlog::info("Initial scan complete. Monitoring for new devices...");
  }

  ~RemovableDeviceMonitor() override { unregisterProxy(); }

 private:
  static constexpr auto kInterfaceName = "org.freedesktop.UDisks2";
  static constexpr auto kObjectPath = "/org/freedesktop/UDisks2";

  DeviceReadyCallback callback_;
  std::map<sdbus::ObjectPath, std::unique_ptr<BlockDeviceWatcher>> watchers_;
  std::mutex watchers_mutex_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    // Not used for ObjectManager
  }

  void onInterfacesAdded(
      const sdbus::ObjectPath& objectPath,
      const std::map<sdbus::InterfaceName,
                     std::map<sdbus::PropertyName, sdbus::Variant>>&
          interfacesAndProperties) override {
    // Check if this is a Block device
    const sdbus::InterfaceName blockInterface("org.freedesktop.UDisks2.Block");
    auto blockIt = interfacesAndProperties.find(blockInterface);
    if (blockIt == interfacesAndProperties.end()) {
      return;
    }

    const auto& blockProps = blockIt->second;

    // Get the Drive object path
    sdbus::ObjectPath drivePath("/");
    const sdbus::PropertyName driveProp("Drive");
    if (const auto it = blockProps.find(driveProp); it != blockProps.end()) {
      drivePath = it->second.get<sdbus::ObjectPath>();
    }

    // Check if it's a valid drive path (not "/")
    if (drivePath == sdbus::ObjectPath("/")) {
      return;
    }

    // Check if the drive is removable
    try {
      const auto driveProxy =
          sdbus::createProxy(getProxy().getConnection(),
                             sdbus::ServiceName(kInterfaceName), drivePath);

      const sdbus::InterfaceName driveInterfaceName(
          "org.freedesktop.UDisks2.Drive");
      auto driveProps =
          driveProxy->getAllProperties().onInterface(driveInterfaceName);

      bool isRemovable = false;
      const sdbus::PropertyName removableProp("Removable");
      if (const auto it = driveProps.find(removableProp);
          it != driveProps.end()) {
        isRemovable = it->second.get<bool>();
      }

      if (isRemovable) {
        spdlog::info("Detected removable device: {}", objectPath.c_str());

        // Create a watcher for this device
        std::lock_guard<std::mutex> lock(watchers_mutex_);
        if (!watchers_.contains(objectPath)) {
          watchers_[objectPath] = std::make_unique<BlockDeviceWatcher>(
              getProxy().getConnection(), objectPath, drivePath, callback_);
        }
      }
    } catch (const std::exception& e) {
      spdlog::debug("Error checking drive removability for {}: {}",
                    objectPath.c_str(), e.what());
    }
  }

  void onInterfacesRemoved(
      const sdbus::ObjectPath& objectPath,
      const std::vector<sdbus::InterfaceName>& interfaces) override {
    // Clean up watcher when device is removed
    const sdbus::InterfaceName blockInterface("org.freedesktop.UDisks2.Block");
    for (const auto& interface : interfaces) {
      if (interface == blockInterface) {
        std::lock_guard<std::mutex> lock(watchers_mutex_);
        if (auto it = watchers_.find(objectPath); it != watchers_.end()) {
          spdlog::info("Removable device removed: {}", objectPath.c_str());
          watchers_.erase(it);
        }
      }
    }
  }
};

// Helper function to convert a byte array to string
std::string byteArrayToString(const std::vector<uint8_t>& bytes) {
  if (bytes.empty()) {
    return "";
  }
  // Remove trailing null bytes
  size_t len = bytes.size();
  while (len > 0 && bytes[len - 1] == 0) {
    --len;
  }
  // Construct string from raw data and explicit size to avoid iterator
  // arithmetic with mixed signed/unsigned types.
  return {reinterpret_cast<const char*>(bytes.data()), len};
}

// Helper to format size in human-readable format
std::string formatSize(const uint64_t bytes) {
  const char* units[] = {"B", "KB", "MB", "GB", "TB"};
  int unit = 0;
  auto size = static_cast<double>(bytes);

  while (size >= 1024.0 && unit < 4) {
    size /= 1024.0;
    unit++;
  }

  std::ostringstream oss;
  oss << std::fixed << std::setprecision(2) << size << " " << units[unit];
  return oss.str();
}

// Default callback that prints all available information
void printDeviceInformation(
    const sdbus::ObjectPath& blockPath,
    const std::map<sdbus::PropertyName, sdbus::Variant>& blockProperties,
    const std::map<sdbus::PropertyName, sdbus::Variant>& driveProperties,
    const std::map<sdbus::PropertyName, sdbus::Variant>& filesystemProperties) {
  std::cout << "\n";
  std::cout << "========================================\n";
  std::cout << "REMOVABLE DEVICE DETECTED\n";
  std::cout << "========================================\n";
  std::cout << "Block Device Path: " << blockPath.c_str() << "\n";
  std::cout << "\n";

  // Block Device Properties
  std::cout << "--- BLOCK DEVICE INFORMATION ---\n";

  if (const auto it = blockProperties.find(sdbus::PropertyName("Device"));
      it != blockProperties.end()) {
    const auto device = it->second.get<std::vector<uint8_t>>();
    std::cout << "  Device: " << byteArrayToString(device) << "\n";
  }

  if (const auto it =
          blockProperties.find(sdbus::PropertyName("PreferredDevice"));
      it != blockProperties.end()) {
    const auto device = it->second.get<std::vector<uint8_t>>();
    std::cout << "  Preferred Device: " << byteArrayToString(device) << "\n";
  }

  if (const auto it = blockProperties.find(sdbus::PropertyName("Size"));
      it != blockProperties.end()) {
    const auto size = it->second.get<uint64_t>();
    std::cout << "  Size: " << formatSize(size) << " (" << size << " bytes)\n";
  }

  if (const auto it = blockProperties.find(sdbus::PropertyName("IdUUID"));
      it != blockProperties.end()) {
    std::cout << "  UUID: " << it->second.get<std::string>() << "\n";
  }

  if (const auto it = blockProperties.find(sdbus::PropertyName("IdLabel"));
      it != blockProperties.end()) {
    std::cout << "  Label: " << it->second.get<std::string>() << "\n";
  }

  if (const auto it = blockProperties.find(sdbus::PropertyName("IdType"));
      it != blockProperties.end()) {
    std::cout << "  Filesystem Type: " << it->second.get<std::string>() << "\n";
  }

  if (const auto it = blockProperties.find(sdbus::PropertyName("IdUsage"));
      it != blockProperties.end()) {
    std::cout << "  Usage: " << it->second.get<std::string>() << "\n";
  }

  if (const auto it = blockProperties.find(sdbus::PropertyName("IdVersion"));
      it != blockProperties.end()) {
    std::cout << "  Version: " << it->second.get<std::string>() << "\n";
  }

  if (const auto it = blockProperties.find(sdbus::PropertyName("ReadOnly"));
      it != blockProperties.end()) {
    std::cout << "  Read-Only: " << (it->second.get<bool>() ? "Yes" : "No")
              << "\n";
  }

  if (const auto it = blockProperties.find(sdbus::PropertyName("Symlinks"));
      it != blockProperties.end()) {
    if (const auto symlinks =
            it->second.get<std::vector<std::vector<uint8_t>>>();
        !symlinks.empty()) {
      std::cout << "  Symlinks:\n";
      for (const auto& symlink : symlinks) {
        std::cout << "    - " << byteArrayToString(symlink) << "\n";
      }
    }
  }

  // Drive Properties
  if (!driveProperties.empty()) {
    std::cout << "\n--- DRIVE INFORMATION ---\n";

    if (const auto it = driveProperties.find(sdbus::PropertyName("Vendor"));
        it != driveProperties.end()) {
      std::cout << "  Vendor: " << it->second.get<std::string>() << "\n";
    }

    if (const auto it = driveProperties.find(sdbus::PropertyName("Model"));
        it != driveProperties.end()) {
      std::cout << "  Model: " << it->second.get<std::string>() << "\n";
    }

    if (const auto it = driveProperties.find(sdbus::PropertyName("Revision"));
        it != driveProperties.end()) {
      std::cout << "  Revision: " << it->second.get<std::string>() << "\n";
    }

    if (const auto it = driveProperties.find(sdbus::PropertyName("Serial"));
        it != driveProperties.end()) {
      std::cout << "  Serial: " << it->second.get<std::string>() << "\n";
    }

    if (const auto it = driveProperties.find(sdbus::PropertyName("WWN"));
        it != driveProperties.end()) {
      std::cout << "  WWN: " << it->second.get<std::string>() << "\n";
    }

    if (const auto it =
            driveProperties.find(sdbus::PropertyName("ConnectionBus"));
        it != driveProperties.end()) {
      std::cout << "  Bus: " << it->second.get<std::string>() << "\n";
    }

    if (const auto it = driveProperties.find(sdbus::PropertyName("Size"));
        it != driveProperties.end()) {
      const auto size = it->second.get<uint64_t>();
      std::cout << "  Drive Size: " << formatSize(size) << " (" << size
                << " bytes)\n";
    }

    if (const auto it = driveProperties.find(sdbus::PropertyName("Removable"));
        it != driveProperties.end()) {
      std::cout << "  Removable: " << (it->second.get<bool>() ? "Yes" : "No")
                << "\n";
    }

    if (const auto it =
            driveProperties.find(sdbus::PropertyName("MediaRemovable"));
        it != driveProperties.end()) {
      std::cout << "  Media Removable: "
                << (it->second.get<bool>() ? "Yes" : "No") << "\n";
    }

    if (const auto it = driveProperties.find(sdbus::PropertyName("Ejectable"));
        it != driveProperties.end()) {
      std::cout << "  Ejectable: " << (it->second.get<bool>() ? "Yes" : "No")
                << "\n";
    }

    if (const auto it = driveProperties.find(sdbus::PropertyName("Media"));
        it != driveProperties.end()) {
      std::cout << "  Media: " << it->second.get<std::string>() << "\n";
    }

    if (const auto it =
            driveProperties.find(sdbus::PropertyName("RotationRate"));
        it != driveProperties.end()) {
      if (const auto rate = it->second.get<int32_t>(); rate > 0) {
        std::cout << "  Rotation Rate: " << rate << " RPM\n";
      } else if (rate == 0) {
        std::cout << "  Type: SSD (Solid State Drive)\n";
      }
    }
  }

  // Filesystem Properties
  if (!filesystemProperties.empty()) {
    std::cout << "\n--- FILESYSTEM INFORMATION ---\n";

    if (const auto it =
            filesystemProperties.find(sdbus::PropertyName("MountPoints"));
        it != filesystemProperties.end()) {
      if (const auto mountPoints =
              it->second.get<std::vector<std::vector<uint8_t>>>();
          !mountPoints.empty()) {
        std::cout << "  Mount Points:\n";
        for (const auto& mp : mountPoints) {
          std::cout << "    - " << byteArrayToString(mp) << "\n";
        }
      } else {
        std::cout << "  Mount Points: (not mounted)\n";
      }
    }

    if (const auto it = filesystemProperties.find(sdbus::PropertyName("Size"));
        it != filesystemProperties.end()) {
      const auto size = it->second.get<uint64_t>();
      std::cout << "  Filesystem Size: " << formatSize(size) << " (" << size
                << " bytes)\n";
    }
  }

  std::cout << "========================================\n";
  std::cout << std::endl;
}

int main() {
  try {
    spdlog::info("Starting UDisks2 Removable Device Monitor Daemon");
    spdlog::info("This daemon monitors for removable device insertions");
    spdlog::info("Press Ctrl+C to stop...\n");

    // Create the system bus connection
    const auto connection = sdbus::createSystemBusConnection();

    // Enter the event loop asynchronously
    connection->enterEventLoopAsync();

    // Create the monitor with the default callback
    // Users can replace printDeviceInformation with their own lambda
    RemovableDeviceMonitor monitor(*connection, printDeviceInformation);

    // Keep running until interrupted by signal (e.g., Ctrl+C)
    // The event loop runs in a separate thread handling D-Bus messages
    // Note: In a production daemon, implement proper signal handling (SIGTERM,
    // SIGINT) to gracefully shut down and call connection->leaveEventLoop()
    using namespace std::chrono_literals;
    while (true) {
      std::this_thread::sleep_for(1000ms);
    }
  } catch (const std::exception& e) {
    spdlog::error("Fatal error: {}", e.what());
    return 1;
  }

  return 0;
}