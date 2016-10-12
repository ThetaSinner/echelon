# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(elm_lib_parser
  tokenizer.cpp
  parser.cpp
  tokenizer-impl/character-query.cpp
  tokenizer-impl/parse-data.cpp
  tokenizer-impl/extractor-for-assignment.cpp
  tokenizer-impl/extractor-for-equality.cpp
  tokenizer-impl/extractor-for-line-comment
  tokenizer-impl/extractor-for-multi-line-comment)

include_directories(tokenizer-impl)

target_compile_features(elm_lib_parser PRIVATE cxx_range_for)
