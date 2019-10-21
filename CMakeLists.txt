# CMake version.
cmake_minimum_required (VERSION 2.8)

# Compiler flags.
include_directories(.)
set (CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -O3")

# Base functionality.
add_executable(base src/base.cpp)

# Sieve of Eratosthenes.
add_executable(sieve_eratosthenes src/sieve_eratosthenes.cpp)

# Prime test.
add_executable(prime_test src/prime_test.cpp)

# Prime decomposition.
add_executable(prime_decomposition src/prime_decomposition.cpp)

# Pythagorean triples.
add_executable(pythagorean_triple src/pythagorean_triple.cpp)
