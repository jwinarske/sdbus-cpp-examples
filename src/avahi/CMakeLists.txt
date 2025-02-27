
add_executable(avahi_server_client
        main.cc
        avahi_server.h
        avahi_server2.h
)
target_link_libraries(avahi_server_client
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET avahi_server_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
