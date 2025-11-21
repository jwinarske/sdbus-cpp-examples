
add_executable(upower_client
        main.cc
        upower_display_device.cc
)
target_link_libraries(upower_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET upower_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

install(TARGETS upower_client
        RUNTIME DESTINATION usr/share/sdbus-cpp-examples)
