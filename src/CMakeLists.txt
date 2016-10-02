cmake_minimum_required (VERSION 3.0)
project(eager-lilac-mayfly)

set (eager-lilac-mayfly-version-)

include_directories(include include/parser)

add_subdirectory(parser)

file(GLOB elm-src "*.cpp")

add_executable(eager-lilac-mayfly ${elm-src})

target_link_libraries(eager-lilac-mayfly parser)
