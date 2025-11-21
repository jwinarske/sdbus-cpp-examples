
find_package(PkgConfig)
pkg_check_modules(UDEV REQUIRED IMPORTED_TARGET libudev)

add_executable(dual_sense_client
        main.cc
        dual_sense.cc
        input_reader.cc
        ../../upower/upower_client.h
        ../../upower/upower_display_device.cc
        ../../upower/upower_display_device.h
)
target_link_libraries(dual_sense_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
        PkgConfig::UDEV
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET dual_sense_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

install(TARGETS dual_sense_client
        RUNTIME DESTINATION usr/share/sdbus-cpp-examples)
