cmake_minimum_required (VERSION 3.28)

project("NTLib - Number Theory Library" VERSION 0.9 LANGUAGES CXX)

################################################################################
# General settings.
################################################################################

# Set the C++ version.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-std=c++23 -Wall -Wextra -O2")

# Treat warnings as errors.
if (MSVC)
  add_compile_options(/W3 /WX)
else()
  add_compile_options(-W -Wall -Werror)
endif()

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

# Build benchmarks.
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "no benchmark test" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "no gtest tests" FORCE)
add_subdirectory(benchmarks)
