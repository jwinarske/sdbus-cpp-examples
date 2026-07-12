
add_executable(fwupd_client
        fwupd_client.cc
        fwupd_client.h
        main.cc
)
target_link_libraries(fwupd_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)

install(TARGETS fwupd_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)