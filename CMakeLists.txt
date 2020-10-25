cmake_minimum_required (VERSION 3.13)

project("ntlib - Number Theory Library" LANGUAGES CXX)

################################################################################
# General settings.
################################################################################

# Build in Release mode to disable asserts.
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-std=c++2a -Wall -Wextra -O3")
include_directories(./include)

################################################################################
# Unit Tests.
################################################################################

# Download and unpack GoogleTest at configure time.
configure_file(CMakeListsGoogleTest.txt googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Add googletest directly to the build.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Build tests.
enable_testing()
add_subdirectory(test)

################################################################################
# Benchmarks.
################################################################################

# Download and unpack GoogleBenchmark at configure time.
configure_file(CMakeListsGoogleBenchmark.txt
    googlebenchmark-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googlebenchmark-download)
if (result)
  message(FATAL_ERROR "CMake step for googlebenchmark failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googlebenchmark-download)
if (result)
  message(FATAL_ERROR "Build step for googlebenchmark failed: ${result}")
endif()

# Add googlebenchmark directly to the build.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googlebenchmark-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googlebenchmark-build
                 EXCLUDE_FROM_ALL)

# Don't run tests from Google Benchmark.
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)

# Build benchmarks.
add_subdirectory(benchmarks)
