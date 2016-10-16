cmake_minimum_required (VERSION 3.2.0)

# Set the project name
project(echelon VERSION 0.0.1)

set(project_binary_name ${PROJECT_NAME})
set(project_test_binary_name "test-${PROJECT_NAME}")

set(project_source_files "src/main.cpp")
file(GLOB project_test_source_files "${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp")

set(parser_lib_name "${PROJECT_NAME}_lib_parser")
set(ast_lib_name "${PROJECT_NAME}_lib_ast")

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add the top level include directory
# (sub directories can be referenced in code, they don't need to be aded here)
include_directories(include)

# Create an executable from the main file.
add_executable(${project_binary_name} ${project_source_files})

# Specify required features for compilation.
target_compile_features(${project_binary_name} PRIVATE cxx_range_for)

# Link the parser to the executable.
add_subdirectory(src/parser)
target_link_libraries(${project_binary_name} ${parser_lib_name})

# Link the ast to the executable.
add_subdirectory(src/ast)
target_link_libraries(${project_binary_name} ${ast_lib_name})

find_package(CxxTest)
if (CXXTEST_FOUND)
  include_directories(${CXXTEST_INCLUDE_DIR})
  enable_testing()
  CXXTEST_ADD_TEST(${project_test_binary_name} test_runner.cpp ${project_test_source_files})
  target_compile_features(${project_test_binary_name} PRIVATE cxx_range_for)
  target_link_libraries(${project_test_binary_name} ${parser_lib_name})
  target_link_libraries(${project_test_binary_name} ${ast_lib_name})
else()
  message("CxxTest not found.")
endif()
