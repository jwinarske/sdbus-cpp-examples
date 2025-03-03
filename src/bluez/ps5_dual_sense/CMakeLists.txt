
find_package(PkgConfig REQUIRED)
pkg_check_modules(HIDAPI IMPORTED_TARGET hidapi-hidraw)

if (HIDAPI_FOUND)
    add_executable(dual_sense_client
            main.cc
            dual_sense.cc
            ../../upower/upower_client.h
            ../../upower/upower_display_device.cc
            ../../upower/upower_display_device.h
    )
    target_link_libraries(dual_sense_client
            PUBLIC
            utils
            sdbus-c++
            spdlog::spdlog
            PkgConfig::HIDAPI
    )
    if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
        set_property(TARGET dual_sense_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif ()
endif ()