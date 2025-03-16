XBox Wireless Controller

## Supported Controllers

Xbox One S - Bluetooth and USB

## Installation

Install udev rules to allow access to the controller

```
sudo cp 99-box-controller.rules /etc/udev/rules.d/
```

Reload and trigger to apply changes

```
sudo udevadm control --reload-rules
sudo udevadm trigger
```
