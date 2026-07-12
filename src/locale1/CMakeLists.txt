
add_executable(locale1_client
        main.cc
        locale1_client.cc
        locale1_client.h
)
target_link_libraries(locale1_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)

install(TARGETS locale1_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)