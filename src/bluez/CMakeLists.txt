
add_executable(bluez_client
        main.cc
        bluez_client.cc
        bluez_client.h
)
target_link_libraries(bluez_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET bluez_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

add_subdirectory(ps5_dual_sense)
add_subdirectory(horipad_steam)
add_subdirectory(xbox_controller)