cmake_minimum_required (VERSION 4.2.3)

# Enable `import std`.
# CMake support for this is still experimental.
# This MUST come before the `project` command.
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_MODULE_STD 1)

project(NTLib
  VERSION 0.9
  DESCRIPTION "NTLib - Number Theory Library"
  LANGUAGES CXX
)

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
# NTLib comes with extensive unit tests.
# By default, these are not built, except this file is at the root of the source
# tree, i.e., NTLib is not included as a dependency but built by itself.
################################################################################

# This evaluates to false if NTLib is included by another project.
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  # Options.
  option(NTLIB_BUILD_TESTS      "Whether tests should be built."           ON)
  option(NTLIB_COVERAGE_REPORT  "Whether coverage report should be built." ON)
  option(NTLIB_BUILD_PACKAGE    "Whether source package should be built."  ON)

  # Tests.
  if (NTLIB_BUILD_TESTS)
    # Enable testing for CTest.
    # Needs to be in the top level CMakeLists.txt file.
    enable_testing()

    add_subdirectory(test)
  endif()

  # Coverage report.
  if (NTLIB_COVERAGE_REPORT)
    find_program(GCOVR_PATH gcovr REQUIRED)

    add_custom_target(coverage
      COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target test
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
      COMMAND ${GCOVR_PATH}
        --root ${CMAKE_SOURCE_DIR}
        --gcov-executable "llvm-cov gcov"
        --filter ${CMAKE_SOURCE_DIR}/modules/
        --exclude-unreachable-branches
        --exclude-throw-branches
        --html --html-details
        --output ${CMAKE_BINARY_DIR}/coverage/index.html
      COMMAND ${CMAKE_COMMAND} -E echo "Coverage report generated at ${CMAKE_BINARY_DIR}/coverage/index.html."
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Generating coverage report"
    )
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
