cmake_minimum_required(VERSION 3.30)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "MinSizeRel" CACHE STRING "Choose the type of build, options are: Debug, Release, RelWithDebInfo, or MinSizeRel." FORCE)
    message(STATUS "CMAKE_BUILD_TYPE not set, defaulting to MinSizeRel.")
endif ()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

project(sdbus-cpp-examples)

# Set install prefix for packaging
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/" CACHE PATH "Install prefix" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_LTO "Enable Link Time Optimization" ON)

#
# Toolchain target
#
message(STATUS "Compiler ............... ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler Version ....... ${CMAKE_CXX_COMPILER_VERSION}")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    include(compiler_clang)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    include(compiler_gnu)
endif ()

#
# Check for IPO support
#
if (ENABLE_LTO)
    cmake_policy(SET CMP0069 NEW)
    include(CheckIPOSupported)
    check_ipo_supported(
            RESULT IPO_SUPPORT_RESULT
            OUTPUT IPO_SUPPORT_OUTPUT
            LANGUAGES C CXX
    )
    if (IPO_SUPPORT_RESULT)
        message(STATUS "IPO .................... supported")
    else ()
        message(STATUS "IPO .................... not supported: ${IPO_SUPPORT_OUTPUT}")
    endif ()
endif ()

add_subdirectory(src/avahi)
add_subdirectory(src/bluez)
add_subdirectory(src/fwupd)
add_subdirectory(src/geoclue2)
add_subdirectory(src/hostname1)
add_subdirectory(src/locale1)
add_subdirectory(src/login1)
add_subdirectory(src/networkmanager)
add_subdirectory(src/packagekit)
add_subdirectory(src/resolve1)
add_subdirectory(src/timedate1)
add_subdirectory(src/udisks2)
add_subdirectory(src/upower)

add_subdirectory(src/utils)

add_subdirectory(third_party)

include(packaging)
