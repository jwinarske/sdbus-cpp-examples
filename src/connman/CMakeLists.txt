add_executable(connman_client
        main.cc
        connman_client.cc
        connman_client.h
)

target_link_libraries(connman_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)


install(TARGETS connman_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)