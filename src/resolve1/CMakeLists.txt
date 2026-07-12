
add_executable(resolve1_client
        main.cc
        resolve1_manager.cc
        resolve1_manager.h
)
target_link_libraries(resolve1_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)

install(TARGETS resolve1_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)