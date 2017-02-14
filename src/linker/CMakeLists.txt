# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(${linker_lib_name}
  linker.cpp
        ../ast/scope/scope-pusher.cpp)

target_link_libraries(${linker_lib_name}
  ${model_lib_name}
)

# Disable prefixing library name with 'lib'
set_target_properties(${linker_lib_name} PROPERTIES PREFIX "")

target_compile_features(${linker_lib_name} PRIVATE cxx_range_for cxx_strong_enums)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(${linker_lib_name} PUBLIC ECHELON_DEBUG)
endif ()
