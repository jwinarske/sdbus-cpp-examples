
#
# sdbus-cpp
#
set(SDBUSCPP_BUILD_CODEGEN ON)
add_subdirectory(sdbus-cpp)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET sdbus-c++ PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
target_link_libraries(sdbus-c++-objlib PUBLIC toolchain::toolchain)
target_link_libraries(sdbus-c++ PUBLIC toolchain::toolchain)
install(TARGETS toolchain EXPORT sdbus-c++-targets)

#
# spdlog
#
add_subdirectory(spdlog)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET spdlog PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
target_link_libraries(spdlog PUBLIC toolchain::toolchain)

#
# hid-rp
#
add_subdirectory(hid-rp)
if (ENABLE_LTO AND IPO_SUPPORT_RESULT)
    set_property(TARGET hid-rp PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()