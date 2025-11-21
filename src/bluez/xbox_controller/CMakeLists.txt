
find_package(PkgConfig)
pkg_check_modules(UDEV REQUIRED IMPORTED_TARGET libudev)

add_executable(xbox_controller_client
        main.cc
        input_reader.cc
        xbox_controller.cc
        ../../upower/upower_client.h
        ../../upower/upower_display_device.cc
        ../../upower/upower_display_device.h
)
target_link_libraries(xbox_controller_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
        PkgConfig::UDEV
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET xbox_controller_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

install(TARGETS xbox_controller_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)