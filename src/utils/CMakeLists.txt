
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
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET utils PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
