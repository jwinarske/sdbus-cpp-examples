
add_executable(avahi_server_client
        main.cc
        avahi_server.h
)
target_link_libraries(avahi_server_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)

install(TARGETS avahi_server_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)