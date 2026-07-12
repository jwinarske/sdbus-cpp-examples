
add_executable(packagekit_client
        packagekit_client.cc
        packagekit_client.h
        packagekit_transaction.cc
        packagekit_transaction.h
        main.cc
)
target_link_libraries(packagekit_client
        PUBLIC
        utils
        sdbus-c++
        spdlog::spdlog
)

install(TARGETS packagekit_client
        RUNTIME DESTINATION share/sdbus-cpp-examples)