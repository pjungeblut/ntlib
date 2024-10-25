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
# Build sub-libraries.
################################################################################

# Basic functionality.
add_library(ntlib_base STATIC)
target_sources(ntlib_base PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/base.cpp
)

# 128 bit integers.
add_library(ntlib_int128 STATIC)
target_sources(ntlib_int128 PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/int128.cpp
)

# Modular arithmetic.
add_library(ntlib_modulo STATIC)
target_sources(ntlib_modulo PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/modulo.cpp
)
target_link_libraries(ntlib_modulo PRIVATE
    ntlib_base
)

# Matrices.
add_library(ntlib_matrix STATIC)
target_sources(ntlib_matrix PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/matrix.cpp
)
target_link_libraries(ntlib_matrix PRIVATE
    ntlib_base
)

# Integers in residue classes.
add_library(ntlib_mod_int STATIC)
target_sources(ntlib_mod_int PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/mod_int.cpp
)
target_link_libraries(ntlib_mod_int PRIVATE
    ntlib_modulo
)

# Prime test.
add_library(ntlib_prime_test STATIC)
target_sources(ntlib_prime_test PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/lucas_sequence.cpp
        ${PROJECT_SOURCE_DIR}/modules/prime_test.cpp
)
target_link_libraries(ntlib_prime_test PRIVATE
    ntlib_base
    ntlib_int128
    ntlib_matrix
    ntlib_mod_int
    ntlib_modulo
)

# Prime generation.
add_library(ntlib_prime_generation STATIC)
target_sources(ntlib_prime_generation PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/sieve.cpp
        ${PROJECT_SOURCE_DIR}/modules/sieve_235.cpp
        ${PROJECT_SOURCE_DIR}/modules/prime_generation.cpp
)
target_link_libraries(ntlib_prime_generation PRIVATE
    ntlib_base
    ntlib_prime_test
)

# Prime decomposition.
add_library(ntlib_prime_decomposition STATIC)
target_sources(ntlib_prime_decomposition PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES
        ${PROJECT_SOURCE_DIR}/include/prime_list.hpp
)
target_sources(ntlib_prime_decomposition PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/prime_decomposition.cpp
)
target_link_libraries(ntlib_prime_decomposition PRIVATE
    ntlib_base
    ntlib_int128
    ntlib_prime_generation
    ntlib_prime_test
)

# Euler's totient function.
add_library(ntlib_euler_totient STATIC)
target_sources(ntlib_euler_totient PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/euler_totient.cpp
)
target_link_libraries(ntlib_euler_totient PRIVATE
    ntlib_base
    ntlib_prime_decomposition
)

# Chinese remainder.
add_library(ntlib_chinese_remainder STATIC)
target_sources(ntlib_chinese_remainder PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/chinese_remainder.cpp
)
target_link_libraries(ntlib_chinese_remainder PRIVATE
    ntlib_base
    ntlib_modulo
    ntlib_prime_decomposition
    ntlib_prime_generation
)

# Binomial coefficients.
add_library(ntlib_binomial_coefficient STATIC)
target_sources(ntlib_binomial_coefficient PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/binomial_coefficient.cpp
)
target_link_libraries(ntlib_binomial_coefficient PRIVATE
    ntlib_base
    ntlib_chinese_remainder
    ntlib_modulo
    ntlib_prime_decomposition
    ntlib_prime_test
)

# Rational numbers.
add_library(ntlib_rational STATIC)
target_sources(ntlib_rational PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/rational.cpp
)
target_link_libraries(ntlib_rational PRIVATE
    ntlib_base
)

# Diophantine equations.
add_library(ntlib_diophantine STATIC)
target_sources(ntlib_diophantine PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/continued_fraction.cpp
        ${PROJECT_SOURCE_DIR}/modules/diophantine.cpp
        ${PROJECT_SOURCE_DIR}/modules/pell_equation.cpp
)
target_link_libraries(ntlib_diophantine PRIVATE
    ntlib_base
    ntlib_rational
)

# Divisors.
add_library(ntlib_divisors STATIC)
target_sources(ntlib_divisors PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/divisors.cpp
)
target_link_libraries(ntlib_divisors PRIVATE
    ntlib_base
    ntlib_prime_decomposition
)

# Figurate numbers.
add_library(ntlib_figurate_number STATIC)
target_sources(ntlib_figurate_number PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/figurate_number.cpp
)

# Pythagorean triples.
add_library(ntlib_pythagorean_triple STATIC)
target_sources(ntlib_pythagorean_triple PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/pythagorean_triple.cpp
)
target_link_libraries(ntlib_pythagorean_triple PRIVATE
    ntlib_base
)

# Turan numbers.
add_library(ntlib_turan_number STATIC)
target_sources(ntlib_turan_number PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES
        ${PROJECT_SOURCE_DIR}/modules/turan_number.cpp
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
