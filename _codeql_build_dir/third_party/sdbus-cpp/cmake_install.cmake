# Install script for directory: /home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/_codeql_build_dir/third_party/sdbus-cpp/tools/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/_codeql_build_dir/third_party/sdbus-cpp/docs/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "sdbus-c++-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/_codeql_build_dir/third_party/sdbus-cpp/libsdbus-c++.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "sdbus-c++-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/sdbus-c++" TYPE FILE FILES
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/ConvenienceApiClasses.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/ConvenienceApiClasses.inl"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/VTableItems.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/VTableItems.inl"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/Error.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/IConnection.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/AdaptorInterfaces.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/ProxyInterfaces.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/StandardInterfaces.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/IObject.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/IProxy.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/Message.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/MethodResult.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/Types.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/TypeTraits.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/Flags.h"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/include/sdbus-c++/sdbus-c++.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "sdbus-c++-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/sdbus-c++/sdbus-c++-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/sdbus-c++/sdbus-c++-targets.cmake"
         "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/_codeql_build_dir/third_party/sdbus-cpp/CMakeFiles/Export/aae783ae287047a810449855b451586a/sdbus-c++-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/sdbus-c++/sdbus-c++-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/sdbus-c++/sdbus-c++-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/sdbus-c++" TYPE FILE FILES "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/_codeql_build_dir/third_party/sdbus-cpp/CMakeFiles/Export/aae783ae287047a810449855b451586a/sdbus-c++-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/sdbus-c++" TYPE FILE FILES "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/_codeql_build_dir/third_party/sdbus-cpp/CMakeFiles/Export/aae783ae287047a810449855b451586a/sdbus-c++-targets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "sdbus-c++-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/sdbus-c++" TYPE FILE FILES
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/_codeql_build_dir/third_party/sdbus-cpp/cmake/sdbus-c++-config.cmake"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/_codeql_build_dir/third_party/sdbus-cpp/cmake/sdbus-c++-config-version.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "sdbus-c++-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/_codeql_build_dir/third_party/sdbus-cpp/pkgconfig/sdbus-c++.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "sdbus-c++-doc" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/sdbus-c++" TYPE FILE FILES
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/README"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/README.md"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/NEWS"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/COPYING"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/ChangeLog"
    "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/third_party/sdbus-cpp/AUTHORS"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/runner/work/sdbus-cpp-examples/sdbus-cpp-examples/_codeql_build_dir/third_party/sdbus-cpp/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
