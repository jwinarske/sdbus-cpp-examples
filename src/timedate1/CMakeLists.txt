
add_executable(timedate1_client
        main.cc
        timedate1_client.cc
        timedate1_client.h
)
target_link_libraries(timedate1_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET timedate1_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
