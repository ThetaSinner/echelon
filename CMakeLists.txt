cmake_minimum_required (VERSION 3.2.0)

# Set the project name and version.
project(echelon VERSION 0.0.1)

# Set names for the output binaries.
set(project_binary_name ${PROJECT_NAME})
set(project_test_binary_name "test-${PROJECT_NAME}")

# Specify root source files.
set(project_source_files "src/main.cpp")
file(GLOB project_test_source_files "${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp")

# Set names for the libs to build.
set(parser_lib_name "${PROJECT_NAME}_lib_parser")
set(model_lib_name "${PROJECT_NAME}_lib_model")
set(transform_lib_name "${PROJECT_NAME}_lib_transform")
set(code_generator_lib_name "${PROJECT_NAME}_lib_code_generator")
set(compiler_lib_name "${PROJECT_NAME}_lib_compiler")
set(linker_lib_name "${PROJECT_NAME}_lib_linker")
set(runner_lib_name "${PROJECT_NAME}_lib_runner")
set(util_lib_name "${PROJECT_NAME}_lib_util")

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add the top level include directory
# (sub directories can be referenced in code, they don't need to be aded here)
include_directories(include)

# -DSPIDERMONKEY_DIR=~/spidermonkey/js/src/build_DBG.OBJ
# g++ --std=c++11 -I/home/thetasinner/spidermonkey/js/src/build_DBG.OBJ/dist/include -L/home/thetasinner/spidermonkey/js/src/build_DBG.OBJ/js/src -g -DDEBUG test.cpp -o test -Wl,--verbose -lmozjs-45 -lm -lz -ldl
# spidermonkey build modified for mfbt: http://stackoverflow.com/questions/40540492/jsapi-segfault-in-js-newcontext
if (SPIDERMONKEY_DIR)
  message("Using spider monkey runner")

  set(spider_monkey_header_directory ${SPIDERMONKEY_DIR}/dist/include)
  set(spider_monkey_lib_directory    ${SPIDERMONKEY_DIR}/js/src)

  include_directories(${spider_monkey_header_directory})
  link_libraries(${spider_monkey_lib_directory})
endif()

# Trigger the lib builds.
add_subdirectory(src/parser)
add_subdirectory(src/model)
add_subdirectory(src/transform)
add_subdirectory(src/code-generation)
add_subdirectory(src/compiler)
add_subdirectory(src/linker)
add_subdirectory(src/runner)
add_subdirectory(src/util)

# Create an executable from the main file.
add_executable(${project_binary_name} ${project_source_files})

# Link the libs to the main executable.
target_link_libraries(${project_binary_name}
  ${parser_lib_name}
  ${model_lib_name}
  ${transform_lib_name}
  ${code_generator_lib_name}
  ${compiler_lib_name}
  ${linker_lib_name}
  ${runner_lib_name}
  ${util_lib_name}
)

# Link to spider monkey if a spider monkey installation forler is specified.
if (SPIDERMONKEY_DIR)
  target_link_libraries(${project_binary_name} mozjs-45 m z dl)
endif()

# Specify required features for compilation.
target_compile_features(${project_binary_name} PRIVATE cxx_range_for)
target_compile_features(${project_binary_name} PRIVATE cxx_nullptr)

# Forward debug definition to the main executable.
if (CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(${project_binary_name} PUBLIC ECHELON_DEBUG)
endif()

# Add cxxtest directory to (path) or (/etc/environment and link to /usr/bin)
find_package(CxxTest)
if (CXXTEST_FOUND)
  message(${CXXTEST_INCLUDE_DIR})
  include_directories(${CXXTEST_INCLUDE_DIR})
  enable_testing()

  CXXTEST_ADD_TEST(${project_test_binary_name} test_runner.cpp ${project_test_source_files})
  target_compile_features(${project_test_binary_name} PRIVATE cxx_range_for)
  target_link_libraries(${project_test_binary_name}
    ${model_lib_name}
    ${parser_lib_name}
    ${transform_lib_name}
    ${code_generator_lib_name}
    ${compiler_lib_name}
    ${linker_lib_name}
    ${util_lib_name})

  if (SPIDERMONKEY_DIR)
    target_link_libraries(${project_test_binary_name} mozjs-45 m z dl)
    target_compile_definitions(${project_test_binary_name} PUBLIC WITH_SPIDER_MONKEY_RUNNER DEBUG)
  endif()

  if (USE_GCC_GCOV)
    set_target_properties(
        ${project_test_binary_name}
        ${model_lib_name}
        ${parser_lib_name}
        ${transform_lib_name}
        ${code_generator_lib_name}
        ${compiler_lib_name}
        ${linker_lib_name}
        ${util_lib_name}
        PROPERTIES
          COMPILE_FLAGS "-O0 -fprofile-arcs -ftest-coverage"
          LINK_FLAGS "-fprofile-arcs -ftest-coverage -lgcov"
    )

    # To create LCOV report
    # lcov --directory cmake-debug-build/src --capture --output-file out.info
    # genhtml out.info
  endif()

  else()
  message("CxxTest not found.")
endif()
