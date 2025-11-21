
add_executable(realtimekit1_manager_client
        main.cc
        realtimekit1_manager_client.cc
        realtimekit1_manager_client.h
)
target_link_libraries(realtimekit1_manager_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET realtimekit1_manager_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

install(TARGETS realtimekit1_manager_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)
