# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(${compiler_lib_name}
  integrity-check.cpp
  echelon-compiler.cpp
)

# Disable prefixing library name with 'lib'
set_target_properties(${compiler_lib_name} PROPERTIES PREFIX "")

target_compile_features(${compiler_lib_name} PRIVATE cxx_range_for)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(${compiler_lib_name} PUBLIC ECHELON_DEBUG)
endif()
