
add_executable(flatpak_system_helper_client
        main.cc
        flatpak_system_helper_client.cc
        flatpak_system_helper_client.h
)
target_link_libraries(flatpak_system_helper_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET flatpak_system_helper_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

install(TARGETS flatpak_system_helper_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)
