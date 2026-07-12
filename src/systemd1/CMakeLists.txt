add_executable(systemd1_client
        main.cc
        systemd1_manager_client.cc
        systemd1_manager_client.h
)

target_link_libraries(systemd1_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)


install(TARGETS systemd1_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)