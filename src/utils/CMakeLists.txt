
add_library(utils STATIC
        event_loop.cc
        event_loop.h
        sys.h
        utils.cc
        utils.h
)
target_link_libraries(utils
        PUBLIC
        sdbus-c++
        glaze::glaze
        spdlog::spdlog
)

if (SDBUS_CPP_EXAMPLES_BUILD_TESTS)
    add_executable(event_loop_test event_loop_test.cc)
    target_link_libraries(event_loop_test PRIVATE utils sdbus-c++ spdlog::spdlog)
    add_test(NAME event_loop_test COMMAND event_loop_test)

    add_executable(sys_test sys_test.cc)
    target_link_libraries(sys_test PRIVATE utils spdlog::spdlog)
    add_test(NAME sys_test COMMAND sys_test)
endif ()
