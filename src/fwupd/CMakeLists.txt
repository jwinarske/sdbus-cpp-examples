
add_executable(fwupd_client
        fwupd_client.cc
        fwupd_client.h
        main.cc
)
target_link_libraries(fwupd_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET fwupd_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

install(TARGETS fwupd_client
        RUNTIME DESTINATION usr/share/sdbus-cpp-examples)
