# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(${runner_lib_name}
  spidermonkey-runner.cpp
  runner-data.cpp)

# Disable prefixing library name with 'lib'
set_target_properties(${runner_lib_name} PROPERTIES PREFIX "")

target_compile_features(${runner_lib_name} PRIVATE cxx_range_for cxx_strong_enums)

if (SPIDERMONKEY_DIR)
  target_compile_definitions(${runner_lib_name} PUBLIC DEBUG WITH_SPIDER_MONKEY_RUNNER)
endif()
