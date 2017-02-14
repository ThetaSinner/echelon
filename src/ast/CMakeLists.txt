# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(${ast_lib_name}
  ast-node.cpp
  transform-stage/node-enhancer-lookup.cpp transform-stage/enhanced-ast-block-node.cpp transform-stage/enhanced-ast-function-prototype-node.cpp)

target_link_libraries(${ast_lib_name}
  ${transform_lib_name}
)

# Disable prefixing library name with 'lib'
set_target_properties(${ast_lib_name} PROPERTIES PREFIX "")

target_compile_features(${ast_lib_name} PRIVATE cxx_range_for cxx_strong_enums)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(${ast_lib_name} PUBLIC ECHELON_DEBUG)
endif ()
