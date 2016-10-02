cmake_minimum_required (VERSION 3.0)

# Set the project name
project(eager-lilac-mayfly)

# Set project version variables
set (ELM_MAJOR_VERSION 1)
set (ELM_MINOR_VERSION 0)
set (ELM_PATCH_VERSION 0)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add the top level include directory
# (sub directories can be referenced in code, they don't need to be aded here)
include_directories(include)

# Create an executable from the main file.
set(elm-src "src/main.cpp")
add_executable(eager-lilac-mayfly ${elm-src})

# Link the parser to the executable
add_subdirectory(src/parser)
target_link_libraries(eager-lilac-mayfly elm_lib_parser)
