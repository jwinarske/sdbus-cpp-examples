
add_executable(packagekit_client
        packagekit_client.cc
        packagekit_client.h
        packagekit_transaction.cc
        packagekit_transaction.h
        main.cc
)
target_link_libraries(packagekit_client
        PUBLIC
        sdbus-c++
        spdlog::spdlog
)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET packagekit_client PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
