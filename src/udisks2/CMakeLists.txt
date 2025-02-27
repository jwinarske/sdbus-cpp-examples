
add_executable(udisks2_client
        main.cc
        udisks2_manager.cc
        udisks2_manager.h
)
target_link_libraries(udisks2_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET udisks2_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
