
find_package(PkgConfig)
pkg_check_modules(UDEV REQUIRED IMPORTED_TARGET libudev)

add_executable(horipad_steam_client
        main.cc
        input_reader.cc
        horipad_steam.cc
        ../../upower/upower_client.h
        ../../upower/upower_display_device.cc
        ../../upower/upower_display_device.h
)
target_link_libraries(horipad_steam_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
        PkgConfig::UDEV
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET horipad_steam_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
