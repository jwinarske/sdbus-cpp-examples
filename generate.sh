#!/bin/bash

PROXY=src/proxy
XML2CPP=$(find -iname sdbus-c++-xml2cpp)

mkdir -p ${PROXY} |true

${XML2CPP} --verbose --proxy=${PROXY}/Adapter1_proxy.h interfaces/org/bluez/Adapter1/org.bluez.Adapter1.xml
${XML2CPP} --verbose --proxy=${PROXY}/Device1_proxy.h interfaces/org/bluez/Device1/org.bluez.Device1.xml
${XML2CPP} --verbose --proxy=${PROXY}/AgentManager1_proxy.h interfaces/org/bluez/AgentManager1/org.bluez.AgentManager1.xml

${XML2CPP} --verbose --proxy=${PROXY}/UPower_proxy.h interfaces/org/freedesktop/UPower/org.freedesktop.UPower.xml
${XML2CPP} --verbose --proxy=${PROXY}/UPowerDisplayDevice_proxy.h interfaces/org/freedesktop/UPower/DisplayDevice/org.freedesktop.UPower.DisplayDevice.xml

${XML2CPP} --verbose --proxy=${PROXY}/Login1Manager_proxy.h interfaces/org/freedesktop/login1/org.freedesktop.login1.Manager.xml
${XML2CPP} --verbose --proxy=${PROXY}/Login1Seat_proxy.h interfaces/org/freedesktop/login1/org.freedesktop.login1.Seat.xml
${XML2CPP} --verbose --proxy=${PROXY}/Login1Session_proxy.h interfaces/org/freedesktop/login1/org.freedesktop.login1.Session.xml
${XML2CPP} --verbose --proxy=${PROXY}/Login1User_proxy.h interfaces/org/freedesktop/login1/org.freedesktop.login1.User.xml

${XML2CPP} --verbose --proxy=${PROXY}/Hostname1_proxy.h interfaces/org/freedesktop/hostname1/org.freedesktop.hostname1.xml
${XML2CPP} --verbose --proxy=${PROXY}/Locale1_proxy.h interfaces/org/freedesktop/hostname1/org.freedesktop.locale1.xml
${XML2CPP} --verbose --proxy=${PROXY}/Timedate1_proxy.h interfaces/org/freedesktop/hostname1/org.freedesktop.timedate1.xml

${XML2CPP} --verbose --proxy=${PROXY}/GeoClue2Manager_proxy.h interfaces/org/freedesktop/GeoClue2/org.freedesktop.GeoClue2.xml
${XML2CPP} --verbose --proxy=${PROXY}/GeoClue2Client_proxy.h interfaces/org/freedesktop/GeoClue2/Client/org.freedesktop.GeoClue2.Client.xml
${XML2CPP} --verbose --proxy=${PROXY}/GeoClue2ClientLocation_proxy.h interfaces/org/freedesktop/GeoClue2/Client/Location/org.freedesktop.GeoClue2.Client.Location.xml

${XML2CPP} --verbose --proxy=${PROXY}/Resolve1Manager_proxy.h interfaces/org/freedesktop/resolve1/Manager/org.freedesktop.resolve1.Manager.xml
${XML2CPP} --verbose --proxy=${PROXY}/Resolve1Link_proxy.h interfaces/org/freedesktop/resolve1/Link/org.freedesktop.resolve1.Link.xml

${XML2CPP} --verbose --proxy=${PROXY}/AvahiServer_proxy.h interfaces/org/freedesktop/Avahi/Server/org.freedesktop.Avahi.Server.xml
${XML2CPP} --verbose --proxy=${PROXY}/AvahiServer2_proxy.h interfaces/org/freedesktop/Avahi/Server2/org.freedesktop.Avahi.Server2.xml

${XML2CPP} --verbose --proxy=${PROXY}/UDisks2Block_proxy.h interfaces/org/freedesktop/UDisks2/Block/org.freedesktop.UDisks2.Block.xml
${XML2CPP} --verbose --proxy=${PROXY}/UDisks2Drive_proxy.h interfaces/org/freedesktop/UDisks2/Drive/org.freedesktop.UDisks2.Drive.xml
${XML2CPP} --verbose --proxy=${PROXY}/UDisks2ManagerNvme_proxy.h interfaces/org/freedesktop/UDisks2/Manager/NVMe/org.freedesktop.UDisks2.Manager.NVMe.xml
${XML2CPP} --verbose --proxy=${PROXY}/UDisks2Manager_proxy.h interfaces/org/freedesktop/UDisks2/Manager/org.freedesktop.UDisks2.Manager.xml
${XML2CPP} --verbose --proxy=${PROXY}/UDisks2NvmeController_proxy.h interfaces/org/freedesktop/UDisks2/NVMe/Controller/org.freedesktop.UDisks2.NVMe.Controller.xml
${XML2CPP} --verbose --proxy=${PROXY}/UDisks2NvmeNamespace_proxy.h interfaces/org/freedesktop/UDisks2/NVMe/Namespace/org.freedesktop.UDisks2.NVMe.Namespace.xml
${XML2CPP} --verbose --proxy=${PROXY}/UDisks2Swapspace_proxy.h interfaces/org/freedesktop/UDisks2/Swapspace/org.freedesktop.UDisks2.Swapspace.xml
${XML2CPP} --verbose --proxy=${PROXY}/UDisks2Filesystem_proxy.h interfaces/org/freedesktop/UDisks2/Filesystem/org.freedesktop.UDisks2.Filesystem.xml
${XML2CPP} --verbose --proxy=${PROXY}/UDisks2Partition_proxy.h interfaces/org/freedesktop/UDisks2/Partition/org.freedesktop.UDisks2.Partition.xml
${XML2CPP} --verbose --proxy=${PROXY}/UDisks2PartitionTable_proxy.h interfaces/org/freedesktop/UDisks2/PartitionTable/org.freedesktop.UDisks2.PartitionTable.xml


mkdir -p ${PROXY}/NetworkManager |true
${XML2CPP} --verbose --proxy=${PROXY}/NetworkManager/NetworkManager_proxy.h interfaces/NetworkManager/NetworkManager.xml
mkdir -p ${PROXY}/NetworkManager/AgentManager |true
${XML2CPP} --verbose --proxy=${PROXY}/NetworkManager/AgentManager/AgentManager_proxy.h interfaces/org/freedesktop/NetworkManager/AgentManager/AgentManager.xml
mkdir -p ${PROXY}/NetworkManager/Connection/Active |true
${XML2CPP} --verbose --proxy=${PROXY}/NetworkManager/Connection/Active/Active_proxy.h interfaces/org/freedesktop/NetworkManager/Connection/Active/Active.xml
mkdir -p ${PROXY}/NetworkManager/Device/Loopback |true
${XML2CPP} --verbose --proxy=${PROXY}/NetworkManager/Device/Loopback/Loopback_proxy.h interfaces/org/freedesktop/NetworkManager/Device/Loopback/Loopback.xml
mkdir -p ${PROXY}/NetworkManager/Device/Statistics |true
${XML2CPP} --verbose --proxy=${PROXY}/NetworkManager/Device/Statistics/Statistics_proxy.h interfaces/org/freedesktop/NetworkManager/Device/Statistics/Statistics.xml
mkdir -p ${PROXY}/NetworkManager/Device |true
${XML2CPP} --verbose --proxy=${PROXY}/NetworkManager/Device/Device_proxy.h interfaces/org/freedesktop/NetworkManager/Device/Device.xml
mkdir -p ${PROXY}/NetworkManager/DHCP4Config |true
${XML2CPP} --verbose --proxy=${PROXY}/NetworkManager/DHCP4Config/DHCP4Config_proxy.h interfaces/org/freedesktop/NetworkManager/DHCP4Config/DHCP4Config.xml
mkdir -p ${PROXY}/NetworkManager/DnsManager |true
${XML2CPP} --verbose --proxy=${PROXY}/NetworkManager/DnsManager/DnsManager_proxy.h interfaces/org/freedesktop/NetworkManager/DnsManager/DnsManager.xml
mkdir -p ${PROXY}/NetworkManager/IP4Config |true
${XML2CPP} --verbose --proxy=${PROXY}/NetworkManager/IP4Config/IP4Config_proxy.h interfaces/org/freedesktop/NetworkManager/IP4Config/IP4Config.xml
mkdir -p ${PROXY}/NetworkManager/IP6Config |true
${XML2CPP} --verbose --proxy=${PROXY}/NetworkManager/IP6Config/IP6Config_proxy.h interfaces/org/freedesktop/NetworkManager/IP6Config/IP6Config.xml
mkdir -p ${PROXY}/NetworkManager/Settings/Connection |true
${XML2CPP} --verbose --proxy=${PROXY}/NetworkManager/Settings/Connection/Connection_proxy.h interfaces/org/freedesktop/NetworkManager/Settings/Connection/Connection.xml
mkdir -p ${PROXY}/NetworkManager/Settings |true
${XML2CPP} --verbose --proxy=${PROXY}/NetworkManager/Settings/Settings_proxy.h interfaces/org/freedesktop/NetworkManager/Settings/Settings.xml

mkdir -p ${PROXY}/PolicyKit1/Authority |true
${XML2CPP} --verbose --proxy=${PROXY}/PolicyKit1/Authority/Authority_proxy.h interfaces/org/freedesktop/PolicyKit1/Authority/Authority.xml

mkdir -p ${PROXY}/systemd1/Manager |true
${XML2CPP} --verbose --proxy=${PROXY}/systemd1/Manager/Manager_proxy.h interfaces/org/freedesktop/systemd1/Manager/Manager.xml
mkdir -p ${PROXY}/systemd1/Service |true
${XML2CPP} --verbose --proxy=${PROXY}/systemd1/Service/Service_proxy.h interfaces/org/freedesktop/systemd1/Service/Service.xml
mkdir -p ${PROXY}/systemd1/Unit |true
${XML2CPP} --verbose --proxy=${PROXY}/systemd1/Unit/Unit_proxy.h interfaces/org/freedesktop/systemd1/Unit/Unit.xml
