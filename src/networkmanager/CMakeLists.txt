
add_executable(networkmanager_client
        networkmanager_client.cc
        networkmanager_client.h
        main.cc
)
target_link_libraries(networkmanager_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)
