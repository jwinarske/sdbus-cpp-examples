
add_library(utils STATIC
        utils.cc
        utils.h
)
target_link_libraries(utils
        PUBLIC
        sdbus-c++
        glaze::glaze
        spdlog::spdlog
)
