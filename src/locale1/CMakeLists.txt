
add_executable(locale1_client
        main.cc
        locale1_client.cc
        locale1_client.h
)
target_link_libraries(locale1_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET locale1_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

install(TARGETS locale1_client
        RUNTIME DESTINATION usr/share/sdbus-cpp-examples)
