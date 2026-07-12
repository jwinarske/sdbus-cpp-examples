
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

install(TARGETS upower_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)