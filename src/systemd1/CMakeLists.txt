add_executable(systemd1_client
        main.cc
        systemd1_manager_client.cc
        systemd1_manager_client.h
)

target_link_libraries(systemd1_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)

if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET systemd1_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

install(TARGETS systemd1_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)