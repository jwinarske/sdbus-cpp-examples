
add_executable(geoclue2_client
        main.cc
        geoclue2_manager.cc
        geoclue2_manager.h
        geoclue2_client.h
        geoclue2_location.h
)
target_link_libraries(geoclue2_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET geoclue2_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
