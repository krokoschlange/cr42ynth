cmake_minimum_required(VERSION 3.10)

project(cr42ynth)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "-O3")

add_subdirectory(rtosc/)
add_subdirectory(src/)

