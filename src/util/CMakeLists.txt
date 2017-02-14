# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(${util_lib_name}
  iterator-util.cpp
  to-string.cpp
  ast-to-graphviz.cpp
  echelon-lookup.cpp

  logging/stream-logger.cpp
  logging/stream-logger-level.cpp
  logging/noop-stream-logger.cpp
  logging/logger.cpp
  logging/logger-shared-instance.cpp
)

# Disable prefixing library name with 'lib'
set_target_properties(${util_lib_name} PROPERTIES PREFIX "")

# C++11 features.
target_compile_features(${util_lib_name} PRIVATE cxx_range_for cxx_strong_enums)

# If log file specified then forward definition to lib.
if (LOG_FILE)
  target_compile_definitions(${util_lib_name} PUBLIC LOG_FILE="${LOG_FILE}")
endif ()

# If log level is specified then forward definition to lib.
if (LOG_LEVEL)
  target_compile_definitions(${util_lib_name} PUBLIC LOG_LEVEL=${LOG_LEVEL})
endif ()

# If a spider monkey installation directory is specified then try to compile with spider monkey enabled.
if (SPIDERMONKEY_DIR)
  target_compile_definitions(${util_lib_name} PUBLIC DEBUG WITH_SPIDER_MONKEY_RUNNER)
endif()
