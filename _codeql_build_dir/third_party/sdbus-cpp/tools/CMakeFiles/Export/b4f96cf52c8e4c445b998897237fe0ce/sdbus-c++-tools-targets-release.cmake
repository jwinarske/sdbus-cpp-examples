#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SDBusCpp::sdbus-c++-xml2cpp" for configuration "Release"
set_property(TARGET SDBusCpp::sdbus-c++-xml2cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SDBusCpp::sdbus-c++-xml2cpp PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/sdbus-c++-xml2cpp"
  )

list(APPEND _cmake_import_check_targets SDBusCpp::sdbus-c++-xml2cpp )
list(APPEND _cmake_import_check_files_for_SDBusCpp::sdbus-c++-xml2cpp "${_IMPORT_PREFIX}/bin/sdbus-c++-xml2cpp" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
