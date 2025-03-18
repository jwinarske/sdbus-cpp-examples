Wireless HORIPAD for Steam

## Supported Controllers

HORIPAD Steam Controller - Bluetooth and USB

## Installation

Install udev rules to allow access to the controller

```
sudo cp 99-horipad-stream.rules /etc/udev/rules.d/
```

Reload and trigger to apply changes

```
sudo udevadm control --reload-rules
sudo udevadm trigger
```
