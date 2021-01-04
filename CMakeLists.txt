cmake_minimum_required(VERSION 2.6)

project(cr42ynth)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "-O3")

add_subdirectory(rtosc/)
add_subdirectory(src/)

