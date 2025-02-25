
add_executable(bluez_client
        main.cc
        bluez_client.cc
        bluez_client.h
)
target_link_libraries(bluez_client
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET bluez_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
