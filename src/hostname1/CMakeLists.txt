
add_executable(hostname1_client
        main.cc
        hostname1_client.cc
        hostname1_client.h
)
target_link_libraries(hostname1_client
        PUBLIC
        utils
        sdbus-c++
        simdjson
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET hostname1_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

install(TARGETS hostname1_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)