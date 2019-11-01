# CMake version.
cmake_minimum_required (VERSION 2.8)

# Compiler flags.
# We need -gnu++2a for concepts and for __int128_t to be integral.
include_directories(.)
set (CMAKE_CXX_FLAGS "-std=gnu++2a -fconcepts -Wall -Wextra -O3")

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

# Project Euler tests.
add_executable(pe_005 projecteuler/005.cpp)
add_executable(pe_007 projecteuler/007.cpp)
add_executable(pe_009 projecteuler/009.cpp)
add_executable(pe_010 projecteuler/010.cpp)
add_executable(pe_012 projecteuler/012.cpp)
add_executable(pe_015 projecteuler/015.cpp)
add_executable(pe_021 projecteuler/021.cpp)
add_executable(pe_023 projecteuler/023.cpp)
add_executable(pe_027 projecteuler/027.cpp)
add_executable(pe_039 projecteuler/039.cpp)
add_executable(pe_041 projecteuler/041.cpp)
add_executable(pe_045 projecteuler/045.cpp)
add_executable(pe_046 projecteuler/046.cpp)
add_executable(pe_047 projecteuler/047.cpp)
add_executable(pe_064 projecteuler/064.cpp)
add_executable(pe_066 projecteuler/066.cpp)
add_executable(pe_069 projecteuler/069.cpp)
add_executable(pe_072 projecteuler/072.cpp)
add_executable(pe_094 projecteuler/094.cpp)
add_executable(pe_108 projecteuler/108.cpp)
add_executable(pe_216 projecteuler/216.cpp)
