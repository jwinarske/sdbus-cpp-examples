
add_executable(login1_manager_client
        main.cc
        login1_manager_client.cc
        login1_manager_client.h
)
target_link_libraries(login1_manager_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)

install(TARGETS login1_manager_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)