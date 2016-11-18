# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(${util_lib_name}
  string-util.cpp
  tizer.cpp
  iterator-util.cpp)

# Disable prefixing library name with 'lib'
set_target_properties(${util_lib_name} PROPERTIES PREFIX "")

target_compile_features(${util_lib_name} PRIVATE cxx_range_for cxx_strong_enums)
