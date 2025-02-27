
add_executable(resolve1_client
        main.cc
        resolve1_manager.cc
        resolve1_manager.h
)
target_link_libraries(resolve1_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET resolve1_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
