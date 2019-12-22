# CMake version.
cmake_minimum_required (VERSION 2.8)

# Project information.
project(ntlib CXX C)
set(PROJECT_VENDOR "Paul Jungeblut")
set(PROJECT_URL "https://github.com/pjungeblut/ntlib")
set(PROJECT_DESCRIPTION "NTLIB: Number Theory Library")
set(CMAKE_BUILD_TYPE "Release")

set (CMAKE_CXX_FLAGS "-std=gnu++2a -fconcepts -Wall -Wextra -O3")
add_subdirectory(include)
add_subdirectory(lib)
