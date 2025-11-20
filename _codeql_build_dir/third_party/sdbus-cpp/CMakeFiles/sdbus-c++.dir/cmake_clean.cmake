file(REMOVE_RECURSE
  "libsdbus-c++.a"
  "libsdbus-c++.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C CXX)
  include(CMakeFiles/sdbus-c++.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
