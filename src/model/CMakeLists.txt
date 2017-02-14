# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(${model_lib_name}
  ast/ast-node.cpp

  internal/node-enhancer-lookup.cpp
  internal/enhanced-ast-block-node.cpp
  internal/enhanced-ast-function-prototype-node.cpp

  scope/scope-helper.cpp
)

# Declare depency libs
target_link_libraries(${model_lib_name}
  ${transform_lib_name}
)

# Disable prefixing library name with 'lib'
set_target_properties(${model_lib_name} PROPERTIES PREFIX "")

target_compile_features(${model_lib_name} PRIVATE cxx_range_for cxx_strong_enums)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(${model_lib_name} PUBLIC ECHELON_DEBUG)
endif ()
