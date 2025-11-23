add_executable(wpa_supplicant1_client
        main.cc
        wpa_supplicant1_client.cc
        wpa_supplicant1_client.h
)

target_link_libraries(wpa_supplicant1_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)

if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET wpa_supplicant1_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

install(TARGETS wpa_supplicant1_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)