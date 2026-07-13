# GenerateProxies.cmake — regenerate the checked-in D-Bus proxy headers.
#
# The proxy headers under src/proxy/ are generated ahead of time from the XML
# interface definitions in interfaces/ and committed to the repository (there is
# no runtime introspection). This module replaces the old generate.sh with a
# CMake target driven by the sdbus-c++-xml2cpp tool built by the
# third_party/sdbus-cpp submodule:
#
#     cmake --build <build-dir> --target regenerate-proxies
#
# It is deliberately NOT part of the default build: regeneration rewrites tracked
# source files, so it must be an explicit, on-demand step. Include this module
# after add_subdirectory(third_party) so the tool target exists.
#
# The list below is the single source of truth for the interface -> proxy
# mapping (path under interfaces/  ->  path under src/proxy/). Add a new
# interface by appending a pair and rerunning the target.

set(_proxy_map
    "org/bluez/Adapter1/Adapter1.xml" "org/bluez/Adapter1/adapter1_proxy.h"
    "org/bluez/AgentManager1/AgentManager1.xml" "org/bluez/AgentManager/agent_manager1_proxy.h"
    "org/bluez/BatteryProviderManager1/BatteryProviderManager1.xml" "org/bluez/BatteryProviderManager1/battery_provider_manager1_proxy.h"
    "org/bluez/Battery1/Battery1.xml" "org/bluez/Battery1/battery1_proxy.h"
    "org/bluez/Device1/Device1.xml" "org/bluez/Device1/device1_proxy.h"
    "org/bluez/Input1/Input1.xml" "org/bluez/Input1/input_proxy.h"
    "org/bluez/LEAdvertisingManager1/LEAdvertisingManager1.xml" "org/bluez/LEAdvertisingManager1/le_advertising_manager1_proxy.h"
    "org/bluez/Media1/Media1.xml" "org/bluez/Media1/media1_proxy.h"
    "org/bluez/NetworkServer1/NetworkServer1.xml" "org/bluez/NetworkServer1/network_server1_proxy.h"
    "org/bluez/Profile1/Profile1.xml" "org/bluez/Profile1/profile1_proxy.h"
    "org/bluez/GattManager1/GattManager1.xml" "org/bluez/GattManager1/gatt_manager1_proxy.h"
    "org/bluez/GattService1/GattService1.xml" "org/bluez/GattService1/gatt_service1_proxy.h"
    "org/bluez/GattCharacteristic1/GattCharacteristic1.xml" "org/bluez/GattCharacteristic1/gatt_characteristic1_proxy.h"
    "org/bluez/GattDescriptor1/GattDescriptor1.xml" "org/bluez/GattDescriptor1/gatt_descriptor1_proxy.h"
    "org/freedesktop/UPower/UPower.xml" "org/freedesktop/UPower/upower_proxy.h"
    "org/freedesktop/UPower/DisplayDevice/DisplayDevice.xml" "org/freedesktop/UPower/DisplayDevice/display_device_proxy.h"
    "org/freedesktop/login1/Manager/Manager.xml" "org/freedesktop/login1/Manager/manager_proxy.h"
    "org/freedesktop/login1/Seat/Seat.xml" "org/freedesktop/login1/Seat/seat_proxy.h"
    "org/freedesktop/login1/Session/Session.xml" "org/freedesktop/login1/Session/session_proxy.h"
    "org/freedesktop/login1/User/User.xml" "org/freedesktop/login1/User/user_proxy.h"
    "org/freedesktop/hostname1/hostname1.xml" "org/freedesktop/hostname1/hostname1_proxy.h"
    "org/freedesktop/locale1/locale1.xml" "org/freedesktop/locale1/locale1_proxy.h"
    "org/freedesktop/ModemManager1/ModemManager1.xml" "org/freedesktop/ModemManager1/ModemManager1_proxy.h"
    "org/freedesktop/timedate1/timedate1.xml" "org/freedesktop/timedate1/timedate1_proxy.h"
    "org/freedesktop/timesync1/Manager/Manager.xml" "org/freedesktop/timesync1/manager_proxy.h"
    "org/freedesktop/GeoClue2/Manager/Manager.xml" "org/freedesktop/GeoClue2/Manager/manager_proxy.h"
    "org/freedesktop/GeoClue2/Client/Client.xml" "org/freedesktop/GeoClue2/Client/client_proxy.h"
    "org/freedesktop/GeoClue2/Client/Location/Location.xml" "org/freedesktop/GeoClue2/Client/Location/location_proxy.h"
    "org/freedesktop/resolve1/Manager/Manager.xml" "org/freedesktop/resolve1/Manager/manager_proxy.h"
    "org/freedesktop/resolve1/Link/Link.xml" "org/freedesktop/resolve1/Link/link_proxy.h"
    "org/freedesktop/fwupd/fwupd.xml" "org/freedesktop/fwupd/fwupd_proxy.h"
    "org/freedesktop/PackageKit/PackageKit.xml" "org/freedesktop/PackageKit/packagekit_proxy.h"
    "org/freedesktop/PackageKit/Offline/Offline.xml" "org/freedesktop/PackageKit/Offline/offline_proxy.h"
    "org/freedesktop/PackageKit/Transaction/Transaction.xml" "org/freedesktop/PackageKit/Transaction/transaction_proxy.h"
    "org/freedesktop/RealtimeKit1/RealtimeKit1.xml" "org/freedesktop/RealtimeKit1/realtime_kit1_proxy.h"
    "org/freedesktop/Avahi/Server/Server.xml" "org/freedesktop/Avahi/Server/server_proxy.h"
    "org/freedesktop/Avahi/Server2/Server2.xml" "org/freedesktop/Avahi/Server2/server2_proxy.h"
    "org/freedesktop/Flatpak/SystemHelper/SystemHelper.xml" "org/freedesktop/Flatpak/SystemHelper/system_helper_proxy.h"
    "org/freedesktop/UDisks2/Block/Block.xml" "org/freedesktop/UDisks2/Block/block_proxy.h"
    "org/freedesktop/UDisks2/Drive/Drive.xml" "org/freedesktop/UDisks2/Drive/drive_proxy.h"
    "org/freedesktop/UDisks2/Manager/Manager.xml" "org/freedesktop/UDisks2/Manager/manager_proxy.h"
    "org/freedesktop/UDisks2/Manager/NVMe/NVMe.xml" "org/freedesktop/UDisks2/Manager/NVMe/nvme_proxy.h"
    "org/freedesktop/UDisks2/NVMe/Controller/Controller.xml" "org/freedesktop/UDisks2/NVMe/controller_proxy.h"
    "org/freedesktop/UDisks2/NVMe/Namespace/Namespace.xml" "org/freedesktop/UDisks2/NVMe/Namespace/namespace_proxy.h"
    "org/freedesktop/UDisks2/Filesystem/Filesystem.xml" "org/freedesktop/UDisks2/Filesystem/filesystem_proxy.h"
    "org/freedesktop/UDisks2/Partition/Partition.xml" "org/freedesktop/UDisks2/Partition/partition_proxy.h"
    "org/freedesktop/UDisks2/PartitionTable/PartitionTable.xml" "org/freedesktop/UDisks2/PartitionTable/partition_table_proxy.h"
    "org/freedesktop/UDisks2/Swapspace/Swapspace.xml" "org/freedesktop/UDisks2/Swapspace/swapspace_proxy.h"
    "org/freedesktop/NetworkManager/NetworkManager.xml" "org/freedesktop/NetworkManager/network_manager_proxy.h"
    "org/freedesktop/NetworkManager/AgentManager/AgentManager.xml" "org/freedesktop/NetworkManager/AgentManager/agent_manager_proxy.h"
    "org/freedesktop/NetworkManager/Connection/Active/Active.xml" "org/freedesktop/NetworkManager/Connection/Active/active_proxy.h"
    "org/freedesktop/NetworkManager/Device/Loopback/Loopback.xml" "org/freedesktop/NetworkManager/Device/Loopback/loopback_proxy.h"
    "org/freedesktop/NetworkManager/Device/Statistics/Statistics.xml" "org/freedesktop/NetworkManager/Device/Statistics/statistics_proxy.h"
    "org/freedesktop/NetworkManager/Device/Wireless/Wireless.xml" "org/freedesktop/NetworkManager/Device/Wireless/wireless_proxy.h"
    "org/freedesktop/NetworkManager/Device/Device.xml" "org/freedesktop/NetworkManager/Device/device_proxy.h"
    "org/freedesktop/NetworkManager/DHCP4Config/DHCP4Config.xml" "org/freedesktop/NetworkManager/DHCP4Config/dhcp4_config_proxy.h"
    "org/freedesktop/NetworkManager/DnsManager/DnsManager.xml" "org/freedesktop/NetworkManager/DnsManager/dns_manager_proxy.h"
    "org/freedesktop/NetworkManager/IP4Config/IP4Config.xml" "org/freedesktop/NetworkManager/IP4Config/ip4_config_proxy.h"
    "org/freedesktop/NetworkManager/IP6Config/IP6Config.xml" "org/freedesktop/NetworkManager/IP6Config/ip6_config_proxy.h"
    "org/freedesktop/NetworkManager/Settings/Connection/Connection.xml" "org/freedesktop/NetworkManager/Settings/Connection/connection_proxy.h"
    "org/freedesktop/NetworkManager/Settings/Settings.xml" "org/freedesktop/NetworkManager/Settings/settings_proxy.h"
    "org/freedesktop/network1/Link/Link.xml" "org/freedesktop/network1/Link/link_proxy.h"
    "org/freedesktop/network1/Network/Network.xml" "org/freedesktop/network1/Network/network_proxy.h"
    "org/freedesktop/network1/Manager/Manager.xml" "org/freedesktop/network1/Manager/manager_proxy.h"
    "org/freedesktop/PolicyKit1/Authority/Authority.xml" "org/freedesktop/PolicyKit1/Authority/authority_proxy.h"
    "org/freedesktop/systemd1/Manager/Manager.xml" "org/freedesktop/systemd1/Manager/manager_proxy.h"
    "org/freedesktop/systemd1/Service/Service.xml" "org/freedesktop/systemd1/Service/service_proxy.h"
    "org/freedesktop/systemd1/Unit/Unit.xml" "org/freedesktop/systemd1/Unit/unit_proxy.h"
    "fi/w1/wpa_supplicant1/wpa_supplicant1.xml" "fi/w1/wpa_supplicant1/wpa_supplicant1_proxy.h"
    "net/connman/Clock/Clock.xml" "net/connman/Clock/clock_proxy.h"
    "net/connman/Manager/Manager.xml" "net/connman/Manager/manager_proxy.h"
    "net/connman/Service/Service.xml" "net/connman/Service/service_proxy.h"
    "net/connman/Technology/Technology.xml" "net/connman/Technology/technology_proxy.h"
    "org/ofono/HandsfreeAudioManager/HandsfreeAudioManager.xml" "org/ofono/HandsfreeAudioManager/handsfree_audio_manager_proxy.h"
    "org/ofono/Manager/Manager.xml" "org/ofono/Manager/manager_proxy.h"
)

add_custom_target(regenerate-proxies
        COMMENT "Regenerating D-Bus proxy headers from interfaces/*.xml")
add_dependencies(regenerate-proxies sdbus-c++-xml2cpp)

list(LENGTH _proxy_map _proxy_map_len)
math(EXPR _proxy_map_last "${_proxy_map_len} - 1")
foreach (_i RANGE 0 ${_proxy_map_last} 2)
    list(GET _proxy_map ${_i} _xml)
    math(EXPR _j "${_i} + 1")
    list(GET _proxy_map ${_j} _out)
    cmake_path(GET _out PARENT_PATH _out_dir)
    # Run from the source root and pass RELATIVE paths: xml2cpp derives the
    # include-guard macro from the --proxy path, so a relative path keeps the
    # guards identical to what generate.sh produced (an absolute path bakes the
    # checkout location into every guard).
    add_custom_command(TARGET regenerate-proxies POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "src/proxy/${_out_dir}"
            COMMAND "$<TARGET_FILE:sdbus-c++-xml2cpp>"
                    "--proxy=src/proxy/${_out}" "interfaces/${_xml}"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            VERBATIM)
endforeach ()

# Format the generated headers the same way the committed ones are stored.
add_custom_command(TARGET regenerate-proxies POST_BUILD
        COMMAND bash "${CMAKE_SOURCE_DIR}/scripts/clang-format.sh" src/proxy
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Formatting generated proxy headers"
        VERBATIM)
