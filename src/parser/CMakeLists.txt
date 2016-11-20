# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(${parser_lib_name}
  tokenizer.cpp
  parser.cpp
  token.cpp
  encoding-query.cpp
  token-expr-info.cpp
  token-expr.cpp
  match-data.cpp
  token-expr-matcher.cpp
  token-type-enum.cpp
  tokenizer-impl/character-query.cpp
  tokenizer-impl/parse-data.cpp
  tokenizer-impl/extractor-for-assignment.cpp
  tokenizer-impl/extractor-for-equality.cpp
  tokenizer-impl/extractor-for-line-comment.cpp
  tokenizer-impl/extractor-for-multi-line-comment.cpp
  tokenizer-impl/extractor-for-strings.cpp
  stage2/echelon-lookup.cpp
  stage2/enhanced-token.cpp
  stage2/matcher.cpp
  stage2/matcher-lookup.cpp
  stage2/pattern-match-info.cpp
  stage2/ast-transform.cpp
  stage2/ast-transform-lookup.cpp
  stage2/token-pattern-element.cpp
  stage2/token-pattern-group.cpp
  stage2/token-pattern.cpp
  stage2/pattern-translator.cpp
  stage2/ast-construction-manager.cpp
  stage2/parser-internal-input.cpp
  stage2/parser-internal-output.cpp
  stage2/parser.cpp
  stage2/token-pattern-lookup.cpp)

include_directories(parser-data)
include_directories(tokenizer-impl)

# Disable prefixing library name with 'lib'
set_target_properties(${parser_lib_name} PROPERTIES PREFIX "")

target_compile_features(${parser_lib_name} PRIVATE cxx_range_for cxx_strong_enums)
