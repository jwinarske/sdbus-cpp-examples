
add_executable(hostname1_client
        main.cc
        hostname1_client.cc
        hostname1_client.h
)
target_link_libraries(hostname1_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)

install(TARGETS hostname1_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)