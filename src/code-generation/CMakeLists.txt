# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY

add_library(${code_generator_lib_name}
  generator-lookup.cpp
  generator.cpp
  code-generator-factory.cpp
  )

if (ECHELON_CODE_GENERATOR MATCHES spider-monkey-js)
  include_directories(spider-monkey-js)
  target_sources(${code_generator_lib_name} PRIVATE
    spider-monkey-js/spider-monkey-js-code-generator.cpp
    spider-monkey-js/spider-monkey-js-generators.cpp
    )
  target_compile_definitions(${code_generator_lib_name} PUBLIC CODE_GENERATOR_SPIDER_MONKEY_JS)
elseif (ECHELON_CODE_GENERATOR MATCHES echelon)
  include_directories(echelon)
  target_sources(${code_generator_lib_name} PRIVATE
    echelon/echelon-code-generator.cpp
    )
  target_compile_definitions(${code_generator_lib_name} PUBLIC CODE_GENERATOR_ECHELON)
else ()
  include_directories(spider-monkey-js)
  target_sources(${code_generator_lib_name} PRIVATE
    spider-monkey-js/spider-monkey-js-code-generator.cpp
    spider-monkey-js/spider-monkey-js-generators.cpp
    )
  target_compile_definitions(${code_generator_lib_name} PUBLIC CODE_GENERATOR_SPIDER_MONKEY_JS)

  include_directories(echelon)
  target_sources(${code_generator_lib_name} PRIVATE
    echelon/echelon-code-generator.cpp
    )
  target_compile_definitions(${code_generator_lib_name} PUBLIC CODE_GENERATOR_ECHELON)
endif ()

# Disable prefixing library name with 'lib'
set_target_properties(${code_generator_lib_name} PROPERTIES PREFIX "")

target_compile_features(${code_generator_lib_name} PRIVATE cxx_range_for)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(${code_generator_lib_name} PUBLIC ECHELON_DEBUG)
endif ()
