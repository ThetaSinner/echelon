cmake_minimum_required (VERSION 3.2.0)

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

target_compile_features(eager-lilac-mayfly PRIVATE cxx_range_for)

# Link the parser to the executable
add_subdirectory(src/parser)
target_link_libraries(eager-lilac-mayfly elm_lib_parser)
# Link the ast to the executable
add_subdirectory(src/ast)
target_link_libraries(eager-lilac-mayfly elm_lib_ast)

find_package(CxxTest)
if (CXXTEST_FOUND)
  include_directories(${CXXTEST_INCLUDE_DIR})
  enable_testing()
  file(GLOB elm-test-src "${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp")
  CXXTEST_ADD_TEST(elm_test test_runner.cpp ${elm-test-src})
  target_compile_features(elm_test PRIVATE cxx_range_for)
  target_link_libraries(elm_test elm_lib_parser)
  target_link_libraries(elm_test elm_lib_ast)
else()
  message("CxxTest not found.")
endif()
