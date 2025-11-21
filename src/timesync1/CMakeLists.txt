
add_executable(timesync1_manager_client
        main.cc
        timesync1_manager_client.cc
        timesync1_manager_client.h
)
target_link_libraries(timesync1_manager_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET timesync1_manager_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

install(TARGETS timesync1_manager_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)
