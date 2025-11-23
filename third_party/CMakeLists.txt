
#
# sdbus-cpp
#
set(SDBUSCPP_BUILD_CODEGEN ON)
set(SDBUSCPP_INSTALL OFF CACHE BOOL "Do not install sdbus-cpp" FORCE)
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
set(SPDLOG_NO_EXCEPTIONS ON)
set(SPDLOG_NO_THREAD_ID ON)
set(SPDLOG_BUILD_PIC ON)
set(SPDLOG_SANITIZE_ADDRESS ${SANITIZE_ADDRESS})
add_library(spdlog INTERFACE)
add_library(spdlog::spdlog ALIAS spdlog)
target_compile_options(spdlog INTERFACE -isystem${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)

#
# glaze
#
if(EXISTS "${CMAKE_SOURCE_DIR}/third_party/glaze/CMakeLists.txt")
    add_subdirectory(glaze EXCLUDE_FROM_ALL)
endif()