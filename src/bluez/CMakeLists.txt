
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

install(TARGETS bluez_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)

add_subdirectory(ps5_dual_sense)
add_subdirectory(horipad_steam)
add_subdirectory(xbox_controller)