add_executable(network1_client
        main.cc
        network1_client.cc
        network1_client.h
)

target_link_libraries(network1_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)


install(TARGETS network1_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)