# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(${transform_lib_name}
  type-deducer.cpp
  ast-enhancer.cpp
  ast-node-enhancer-input-input-data.cpp
  ast-node-enhancer-input-output-data.cpp
  type-rule-lookup.cpp
  name-resolver.cpp
  custom-type-handler.cpp

  transform-data/ast-enhancer-data.cpp
  transform-data/ast-enhancer-helper.cpp
  transform-data/operator-precedence-tree-restructurer.cpp
  transform-data/type-rule-data-load.cpp
  type-events.cpp)

# Declare dependency libs.
target_link_libraries(${transform_lib_name}
  ${model_lib_name}
)

# Disable prefixing library name with 'lib'
set_target_properties(${transform_lib_name} PROPERTIES PREFIX "")

# C++11 features.
target_compile_features(${transform_lib_name} PRIVATE cxx_range_for)

# Forward debug definition to this lib.
if (CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(${transform_lib_name} PUBLIC ECHELON_DEBUG)
endif ()