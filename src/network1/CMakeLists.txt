add_executable(network1_client
        main.cc
        network1_client.cc
        network1_client.h
)

target_link_libraries(network1_client
        PUBLIC
        utils
        sdbus-c++
        simdjson
        spdlog::spdlog
)

if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET network1_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

install(TARGETS network1_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)