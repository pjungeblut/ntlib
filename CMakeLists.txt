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
# Build NTLib as a static library.
################################################################################

add_subdirectory(modules)

add_library(ntlib STATIC
    $<TARGET_OBJECTS:ntlib_base>
    $<TARGET_OBJECTS:ntlib_diophantine>
)

################################################################################
# Unit Tests.
#
# Workflow from:
# https://github.com/google/googletest/blob/main/googletest/README.md
################################################################################

include(FetchContent)
FetchContent_Declare(
    googletest
    # Specify the commit you depend on and update it regularly.
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG df1544bcee0c7ce35cd5ea0b3eb8cc81855a4140
)
# For Windows: Prevent overriding the parent project's compiler/linker settings.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Build tests.
enable_testing()
add_subdirectory(test)

################################################################################
# Benchmarks.
################################################################################

FetchContent_Declare(
    googlebenchmark
    # Specify the commit you depend on and update it regularly.
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG 12235e24652fc7f809373e7c11a5f73c5763fc4c # version 1.9.0
)
FetchContent_MakeAvailable(googlebenchmark)

# Build benchmarks.
add_subdirectory(benchmarks)
