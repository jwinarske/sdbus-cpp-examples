# UDisks2 Examples

This directory contains examples for interacting with the UDisks2 D-Bus service to manage storage devices.

## Examples

### udisks2_client

The basic UDisks2 client that demonstrates how to enumerate and monitor all storage devices on the system.

**Usage:**
```bash
./udisks2_client
```

### udisks2_monitor_daemon

A daemon that specifically monitors for removable device insertions (USB drives, SD cards, etc.) and prints comprehensive device information when detected.

**Features:**
- Monitors D-Bus system bus for `org.freedesktop.UDisks2` `InterfacesAdded` signals
- Filters for removable devices using the `Drive.Removable` property
- Watches block device properties for filesystem readiness (`IdUsage` == 'filesystem')
- Invokes a customizable C++ lambda callback to print device information
- Prints comprehensive drive information including:
  - Block device properties (device path, UUID, label, size, type)
  - Filesystem properties (type, mount points)
  - Device information (vendor, model, bus, serial, WWN, rotation rate)

**Usage:**
```bash
./udisks2_monitor_daemon
```

**Example Output:**
```
[2025-11-20 23:14:53.075] [info] Starting UDisks2 Removable Device Monitor Daemon
[2025-11-20 23:14:53.075] [info] This daemon monitors for removable device insertions
[2025-11-20 23:14:53.075] [info] Press Ctrl+C to stop...

[2025-11-20 23:14:53.075] [info] Scanning for existing removable devices...
[2025-11-20 23:14:53.081] [info] Initial scan complete. Monitoring for new devices...

[2025-11-20 23:15:10.123] [info] Detected removable device: /org/freedesktop/UDisks2/block_devices/sdc1

========================================
REMOVABLE DEVICE DETECTED
========================================
Block Device Path: /org/freedesktop/UDisks2/block_devices/sdc1

--- BLOCK DEVICE INFORMATION ---
  Device: /dev/sdc1
  Preferred Device: /dev/sdc1
  Size: 15.56 GB (16717512192 bytes)
  UUID: 1234-5678
  Label: MY_USB_DRIVE
  Filesystem Type: vfat
  Usage: filesystem
  Read-Only: No

--- DRIVE INFORMATION ---
  Vendor: SanDisk
  Model: Ultra USB 3.0
  Bus: usb
  Drive Size: 15.56 GB (16717512192 bytes)
  Removable: Yes
  Media Removable: No
  Ejectable: Yes

--- FILESYSTEM INFORMATION ---
  Mount Points:
    - /media/user/MY_USB_DRIVE
  Filesystem Size: 15.55 GB (16708000000 bytes)
========================================
```

**Customization:**

The daemon uses a callback function that can be easily customized. In the source code, you can replace the `printDeviceInformation` function with your own lambda:

```cpp
// Example: Custom callback for your specific use case
auto myCallback = [](
    const sdbus::ObjectPath& blockPath,
    const std::map<sdbus::PropertyName, sdbus::Variant>& blockProperties,
    const std::map<sdbus::PropertyName, sdbus::Variant>& driveProperties,
    const std::map<sdbus::PropertyName, sdbus::Variant>& filesystemProperties) {
    
    // Your custom logic here
    // For example: automatically mount the device, send a notification, etc.
    std::cout << "Custom handler for device: " << blockPath.c_str() << std::endl;
};

// Pass your callback to the monitor
RemovableDeviceMonitor monitor(*connection, myCallback);
```

## Requirements

- sdbus-c++ library
- spdlog library
- systemd/elogind/basu (for sd-bus implementation)
- UDisks2 service running on the system

## Notes

- The daemon requires access to the system D-Bus, which typically requires appropriate permissions
- On some systems, you may need to run as root or configure D-Bus policies
- The monitoring is non-intrusive and only reads device information via D-Bus
