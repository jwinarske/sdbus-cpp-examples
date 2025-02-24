#!/bin/bash -ex

PROXY=src/proxy
XML2CPP=$(find -iname sdbus-c++-xml2cpp)

mkdir -p ${PROXY}/org/bluez/{Adapter1,AgentManager,BatteryProviderManager1,Device1,GattManager1,LEAdvertisingManager1,Media1,NetworkServer1,Profile1} |true

${XML2CPP} --verbose --proxy=${PROXY}/org/bluez/Adapter1/adapter1_proxy.h interfaces/org/bluez/Adapter1/Adapter1.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/bluez/AgentManager/agent_manager1_proxy.h interfaces/org/bluez/AgentManager1/AgentManager1.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/bluez/BatteryProviderManager1/battery_provider_manager1_proxy.h interfaces/org/bluez/BatteryProviderManager1/BatteryProviderManager1.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/bluez/Device1/device1_proxy.h interfaces/org/bluez/Device1/Device1.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/bluez/GattManager1/gatt_manager1_proxy.h interfaces/org/bluez/GattManager1/GattManager1.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/bluez/LEAdvertisingManager1/le_advertising_manager1_proxy.h interfaces/org/bluez/LEAdvertisingManager1/LEAdvertisingManager1.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/bluez/Media1/media1_proxy.h interfaces/org/bluez/Media1/Media1.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/bluez/NetworkServer1/network_server1_proxy.h interfaces/org/bluez/NetworkServer1/NetworkServer1.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/bluez/Profile1/profile1_proxy.h interfaces/org/bluez/Profile1/Profile1.xml

mkdir -p ${PROXY}/org/freedesktop/UPower/DisplayDevice |true

${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/UPower/upower_proxy.h interfaces/org/freedesktop/UPower/UPower.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/UPower/DisplayDevice/display_device_proxy.h interfaces/org/freedesktop/UPower/DisplayDevice/DisplayDevice.xml

mkdir -p ${PROXY}/org/freedesktop/login1/{Manager,Seat,Session,User} |true

${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/login1/Manager/manager_proxy.h interfaces/org/freedesktop/login1/Manager/Manager.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/login1/Seat/seat_proxy.h interfaces/org/freedesktop/login1/Seat/Seat.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/login1/Session/session_proxy.h interfaces/org/freedesktop/login1/Session/Session.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/login1/User/user_proxy.h interfaces/org/freedesktop/login1/User/User.xml

mkdir -p ${PROXY}/org/freedesktop/hostname1 |true

${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/hostname1/hostname1_proxy.h interfaces/org/freedesktop/hostname1/hostname1.xml

mkdir -p ${PROXY}/org/freedesktop/locale1 |true

${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/locale1/locale1_proxy.h interfaces/org/freedesktop/locale1/locale1.xml

mkdir -p ${PROXY}/org/freedesktop/timedate1 |true

${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/timedate1/timedate1_proxy.h interfaces/org/freedesktop/timedate1/timedate1.xml

mkdir -p ${PROXY}/org/freedesktop/GeoClue2/{Manager,Client/Location} |true

${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/GeoClue2/Manager/manager_proxy.h interfaces/org/freedesktop/GeoClue2/Manager/Manager.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/GeoClue2/Client/client_proxy.h interfaces/org/freedesktop/GeoClue2/Client/Client.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/GeoClue2/Client/Location/location_proxy.h interfaces/org/freedesktop/GeoClue2/Client/Location/Location.xml

mkdir -p ${PROXY}/org/freedesktop/resolve1/{Manager,Link} |true

${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/resolve1/Manager/manager_proxy.h interfaces/org/freedesktop/resolve1/Manager/Manager.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/resolve1/Manager/link_proxy.h interfaces/org/freedesktop/resolve1/Link/Link.xml

mkdir -p ${PROXY}/org/freedesktop/Avahi/{Server,Server2} |true

${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/Avahi/Server/server_proxy.h interfaces/org/freedesktop/Avahi/Server/Server.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/Avahi/Server2/server2_proxy.h interfaces/org/freedesktop/Avahi/Server2/Server2.xml

mkdir -p ${PROXY}/org/freedesktop/UDisks2/{Block,Drive,Manager,Manager/NVMe,NVMe/Controller,NVMe/Namespace,Filesystem,Partition,PartitionTable,Swapspace} |true

${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/UDisks2/Block/block_proxy.h                   interfaces/org/freedesktop/UDisks2/Block/Block.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/UDisks2/Drive/drive_proxy.h                   interfaces/org/freedesktop/UDisks2/Drive/Drive.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/UDisks2/Manager/manager_proxy.h               interfaces/org/freedesktop/UDisks2/Manager/Manager.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/UDisks2/Manager/NVMe/nvme_proxy.h             interfaces/org/freedesktop/UDisks2/Manager/NVMe/NVMe.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/UDisks2/NVMe/controller_proxy.h               interfaces/org/freedesktop/UDisks2/NVMe/Controller/Controller.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/UDisks2/NVMe/Namespace/namespace_proxy.h      interfaces/org/freedesktop/UDisks2/NVMe/Namespace/Namespace.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/UDisks2/Filesystem/filesystem_proxy.h         interfaces/org/freedesktop/UDisks2/Filesystem/Filesystem.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/UDisks2/Partition/partition_proxy.h           interfaces/org/freedesktop/UDisks2/Partition/Partition.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/UDisks2/PartitionTable/partition_table_proxy.h interfaces/org/freedesktop/UDisks2/PartitionTable/PartitionTable.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/UDisks2/Swapspace/swapspace_proxy.h           interfaces/org/freedesktop/UDisks2/Swapspace/Swapspace.xml

mkdir -p ${PROXY}/org/freedesktop/NetworkManager/{AgentManager,Connection/Active,Device,Device/Loopback,Device/Statistics,Device/Wireless,DnsManager,DHCP4Config,IP4Config,IP6Config,Settings,Settings/Connection} |true

${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/NetworkManager/network_manager_proxy.h interfaces/org/freedesktop/NetworkManager/NetworkManager.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/NetworkManager/AgentManager/agent_manager_proxy.h interfaces/org/freedesktop/NetworkManager/AgentManager/AgentManager.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/NetworkManager/Connection/Active/active_proxy.h interfaces/org/freedesktop/NetworkManager/Connection/Active/Active.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/NetworkManager/Device/Loopback/loopback_proxy.h interfaces/org/freedesktop/NetworkManager/Device/Loopback/Loopback.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/NetworkManager/Device/Statistics/statistics_proxy.h interfaces/org/freedesktop/NetworkManager/Device/Statistics/Statistics.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/NetworkManager/Device/Wireless/wireless_proxy.h interfaces/org/freedesktop/NetworkManager/Device/Wireless/Wireless.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/NetworkManager/Device/device_proxy.h interfaces/org/freedesktop/NetworkManager/Device/Device.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/NetworkManager/DHCP4Config/dhcp4_config_proxy.h interfaces/org/freedesktop/NetworkManager/DHCP4Config/DHCP4Config.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/NetworkManager/DnsManager/dns_manager_proxy.h interfaces/org/freedesktop/NetworkManager/DnsManager/DnsManager.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/NetworkManager/IP4Config/ip4_config_proxy.h interfaces/org/freedesktop/NetworkManager/IP4Config/IP4Config.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/NetworkManager/IP6Config/ip6_config_proxy.h interfaces/org/freedesktop/NetworkManager/IP6Config/IP6Config.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/NetworkManager/Settings/Connection/connection_proxy.h interfaces/org/freedesktop/NetworkManager/Settings/Connection/Connection.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/NetworkManager/Settings/settings_proxy.h interfaces/org/freedesktop/NetworkManager/Settings/Settings.xml

mkdir -p ${PROXY}/org/freedesktop/network1/{Link,Manager,Network} |true

${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/network1/Link/link_proxy.h interfaces/org/freedesktop/network1/Link/Link.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/network1/Network/network_proxy.h interfaces/org/freedesktop/network1/Network/Network.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/network1/Manager/manager_proxy.h interfaces/org/freedesktop/network1/Manager/Manager.xml

mkdir -p ${PROXY}/org/freedesktop/PolicyKit1/Authority |true

${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/PolicyKit1/Authority/authority_proxy.h interfaces/org/freedesktop/PolicyKit1/Authority/Authority.xml

mkdir -p ${PROXY}/org/freedesktop/systemd1/{Manager,Service,Unit} |true

${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/systemd1/Manager/manager_proxy.h interfaces/org/freedesktop/systemd1/Manager/Manager.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/systemd1/Service/service_proxy.h interfaces/org/freedesktop/systemd1/Service/Service.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/freedesktop/systemd1/Unit/unit_proxy.h interfaces/org/freedesktop/systemd1/Unit/Unit.xml

mkdir -p ${PROXY}/fi/w1/wpa_supplicant1 |true

${XML2CPP} --verbose --proxy=${PROXY}/fi/w1/wpa_supplicant1/wpa_supplicant1_proxy.h interfaces/fi/w1/wpa_supplicant1/wpa_supplicant1.xml

mkdir -p ${PROXY}/net/connman/{Clock,Manager,Service,Technology} |true

${XML2CPP} --verbose --proxy=${PROXY}/net/connman/Clock/clock_proxy.h interfaces/net/connman/Clock/Clock.xml
${XML2CPP} --verbose --proxy=${PROXY}/net/connman/Manager/manager_proxy.h interfaces/net/connman/Manager/Manager.xml
${XML2CPP} --verbose --proxy=${PROXY}/net/connman/Service/service_proxy.h interfaces/net/connman/Service/Service.xml
${XML2CPP} --verbose --proxy=${PROXY}/net/connman/Technology/technology_proxy.h interfaces/net/connman/Technology/Technology.xml

mkdir -p ${PROXY}/org/ofono/{HandsfreeAudioManager,Manager} |true

${XML2CPP} --verbose --proxy=${PROXY}/org/ofono/HandsfreeAudioManager/handsfree_audio_manager_proxy.h interfaces/org/ofono/HandsfreeAudioManager/HandsfreeAudioManager.xml
${XML2CPP} --verbose --proxy=${PROXY}/org/ofono/Manager/manager_proxy.h interfaces/org/ofono/Manager/Manager.xml

