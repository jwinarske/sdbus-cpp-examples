
add_executable(upower_client
        main.cc
        upower_client.cc
        upower_display_device.cc
)
target_link_libraries(upower_client
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET upower_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
