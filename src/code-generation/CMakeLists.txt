# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY

add_library(${code_generator_lib_name}
  generator-lookup.cpp
  generator.cpp
)

if (ECHELON_CODE_GENERATOR MATCHES spider-monkey-js)
  include_directories(spider-monkey-js)
  target_sources(${code_generator_lib_name} PUBLIC
    spider-monkey-js/spider-monkey-js-code-generator.cpp
    spider-monkey-js/spider-monkey-js-generators.cpp
  )
elseif(ECHELON_CODE_GENERATOR MATCHES echelon)
  target_sources(${code_generator_lib_name} PRIVATE
    echelon/echelon-code-generator.cpp
  )
else()
  target_sources(${code_generator_lib_name} PRIVATE
    echelon/echelon-code-generator.cpp
  )
endif()

# Disable prefixing library name with 'lib'
set_target_properties(${code_generator_lib_name} PROPERTIES PREFIX "")

target_compile_features(${code_generator_lib_name} PRIVATE cxx_range_for)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(${code_generator_lib_name} PUBLIC ECHELON_DEBUG)
endif()
