
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
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET login1_manager_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
