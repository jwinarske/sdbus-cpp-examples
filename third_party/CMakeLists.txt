
#
# sdbus-cpp
#
set(SDBUSCPP_BUILD_CODEGEN ON)
set(SDBUSCPP_INSTALL OFF CACHE BOOL "Do not install sdbus-cpp" FORCE)
add_subdirectory(sdbus-cpp)
target_link_libraries(sdbus-c++-objlib PUBLIC toolchain::toolchain)
target_link_libraries(sdbus-c++ PUBLIC toolchain::toolchain)
install(TARGETS toolchain EXPORT sdbus-c++-targets)

#
# spdlog
#
# Exceptions are disabled so spdlog's internal errors (bad sink path, disk
# full, etc.) are routed through its error-handler instead of being thrown.
# A custom spdlog::set_error_handler() is installed in src/utils/logging.h
# before any sink is constructed so these failures are always written to
# stderr rather than silently dropped.
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