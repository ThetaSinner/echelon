# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(elm_lib_ast
  AstNode.cpp)

target_compile_features(elm_lib_ast PRIVATE cxx_range_for cxx_strong_enums)
