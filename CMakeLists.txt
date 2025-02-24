cmake_minimum_required(VERSION 3.30)
project(sdbus-cpp-examples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(third_party)

add_executable(bluez_client
        src/bluez/main.cc
        src/bluez/bluez_client.cc
        src/bluez/bluez_client.h
)
target_link_libraries(bluez_client
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)


add_executable(upower_client
        src/upower/main.cc
        src/upower/upower_client.cc
        src/upower/upower_display_device.cc
)
target_link_libraries(upower_client
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)


add_executable(login1_manager_client
        src/login1/main.cc
        src/login1/login1_manager_client.cc
        src/login1/login1_manager_client.h
)
target_link_libraries(login1_manager_client
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)

add_executable(hostname1_client
        src/hostname1/main.cc
        src/hostname1/hostname1_client.cc
        src/hostname1/hostname1_client.h
)
target_link_libraries(hostname1_client
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)

add_executable(locale1_client
        src/locale1/main.cc
        src/locale1/locale1_client.cc
        src/locale1/locale1_client.h
)
target_link_libraries(locale1_client
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)

add_executable(timedate1_client
        src/timedate1/main.cc
        src/timedate1/timedate1_client.cc
        src/timedate1/timedate1_client.h
)
target_link_libraries(timedate1_client
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)

add_executable(geoclue2
        src/geoclue2/main.cc
        src/geoclue2/geoclue2_manager.cc
        src/geoclue2/geoclue2_manager.h
        src/geoclue2/geoclue2_client.h
        src/geoclue2/geoclue2_location.h
)
target_link_libraries(geoclue2
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)

add_executable(resolve1_client
        src/resolve1/main.cc
        src/resolve1/resolve1_manager.cc
        src/resolve1/resolve1_manager.h
)
target_link_libraries(resolve1_client
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)

add_executable(avahi_server2_client
        src/avahi/main.cc
        src/avahi/avahi_server2.cc
        src/avahi/avahi_server2.h
)
target_link_libraries(avahi_server2_client
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)

add_executable(udisks2_client
        src/udisks2/main.cc
        src/udisks2/udisks2_manager.cc
        src/udisks2/udisks2_manager.h
)
target_link_libraries(udisks2_client
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)