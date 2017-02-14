# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(${parser_lib_name}
  parser-data/parser-stage-1-data-load.cpp
  parser-data/parser-stage-2-data-load.cpp
  parser-data/parser-stage-2-helper.cpp

  stage1/token.cpp
  stage1/character-matcher-lookup.cpp
  stage1/character-pattern-element.cpp
  stage1/character-pattern-group.cpp
  stage1/character-pattern.cpp
  stage1/character-pattern-parser.cpp
  stage1/tokenizer.cpp
  stage1/character-pattern-lookup.cpp

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
  stage2/token-pattern-lookup.cpp
  stage2/nested-pattern-lookup.cpp
  stage2/ast-transform-data.cpp
)

# Declare dependency libs.
target_link_libraries(${parser_lib_name}
  ${util_lib_name}
)

# Disable prefixing library name with 'lib'
set_target_properties(${parser_lib_name} PROPERTIES PREFIX "")

# C++11 features.
target_compile_features(${parser_lib_name} PRIVATE cxx_range_for cxx_strong_enums)

# Forward debug definition to this lib.
if (CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(${parser_lib_name} PUBLIC ECHELON_DEBUG)
endif ()
