
add_executable(timedate1_client
        main.cc
        timedate1_client.cc
        timedate1_client.h
)
target_link_libraries(timedate1_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)

install(TARGETS timedate1_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)