cmake_minimum_required (VERSION 3.28)

project(NTLib
    VERSION 0.9
    DESCRIPTION "NTLib - Number Theory Library"
    LANGUAGES CXX
  )

################################################################################
# General settings.
################################################################################

# Colored compiler output.
option (NTLIB_COLORED_COMPILET_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if (${NTLIB_COLORED_COMPILET_OUTPUT})
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
  endif()
endif()

################################################################################
# Build NTLib as a static library.
################################################################################

add_subdirectory(modules)

add_library(ntlib STATIC
    $<TARGET_OBJECTS:ntlib_arithmetic_functions>
    $<TARGET_OBJECTS:ntlib_base>
    $<TARGET_OBJECTS:ntlib_chinese_remainder>
    $<TARGET_OBJECTS:ntlib_combinatorics>
    $<TARGET_OBJECTS:ntlib_diophantine>
    $<TARGET_OBJECTS:ntlib_prime_decomposition>
    $<TARGET_OBJECTS:ntlib_prime_generation>
    $<TARGET_OBJECTS:ntlib_prime_test>
    $<TARGET_OBJECTS:ntlib_types>
)
target_compile_features(ntlib PUBLIC cxx_std_23)

################################################################################
# NTLib comes with tests and benchmarks.
# By default, these are not bulid, except this file is at the root of the source
# tree, i.e., NTLib is not included as a dependency but bulid by itself.
################################################################################

# This evaluates to false if NTLib is included by another project.
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  # Options.
  option(NTLIB_BUILD_TESTS      "whether tests should be built"          ON)
  option(NTLIB_BUILD_BENCHMARKS "whether benchmarks should be built"     ON)
  option(NTLIB_BUILD_PACKAGE    "whether source package should be built" ON)

  # Tests.
  if (NTLIB_BUILD_TESTS)
    # Enable testing for CTest.
    # Needs to be in the top level CMakeLists.txt file.
    enable_testing()

    add_subdirectory(test)
  endif()

  # Benchmarks.
  if (NTLIB_BUILD_BENCHMARKS)
    # Don't build tests for Google Benchmark.
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF
        CACHE INTERNAL "don't build tests for Google Benchmark" FORCE)

    add_subdirectory(benchmarks)
  endif()

  # Source package.
  # This allows fast consumption via CMake's `FetchContent` command.
  #
  # See: https://www.foonathan.net/2022/06/cmake-fetchcontent/
  if (NTLIB_BUILD_PACKAGE)
    set(ntlib_package_files modules/ includes/ CMakeLists.txt LICENSE)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINART_DIR}/package/${PROJECT_NAME}-src.zip
        COMMAND ${CMAKE_COMMAND} -E tar c ${CMAKE_CURRENT_BINART_DIR}/package/${PROJECT_NAME}-src.zip --format=zip -- ${ntlib_package_files}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${ntlib_package_files}
    )
    add_custom_target(${PROJECT_NAME}_package DEPENDS ${CMAKE_CURRENT_BINART_DIR}/package/${PROJECT_NAME}-src.zip)
  endif()
endif()
