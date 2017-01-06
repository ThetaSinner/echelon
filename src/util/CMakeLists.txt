# This will be compiled into the executable if BUILD_SHARED_LIBS is OFF
# otherwise it is output to CMAKE_LIBRARY_OUTPUT_DIRECTORY
add_library(${util_lib_name}
  iterator-util.cpp
  stream-dump.cpp
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

target_compile_features(${util_lib_name} PRIVATE cxx_range_for cxx_strong_enums)

if (LOG_FILE)
  target_compile_definitions(${util_lib_name} PUBLIC LOG_FILE="${LOG_FILE}")
endif ()

if (LOG_LEVEL)
  target_compile_definitions(${util_lib_name} PUBLIC LOG_LEVEL=${LOG_LEVEL})
endif ()