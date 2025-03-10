# Use GoogleTest for unit tests.
#
# Workflow from:
# https://github.com/google/googletest/blob/main/googletest/README.md

# Configure GoogleTest.
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

# Include GoogleTest.
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

# Helper function to add unittests.
function(add_unittest name libraries)
  add_executable(ut_${name})
  target_sources(ut_${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp)
  target_link_libraries(ut_${name} PRIVATE
      ${libraries}
      GTest::gtest_main
  )
  set_target_properties(ut_${name} PROPERTIES OUTPUT_NAME ${name})
  add_test(NAME ${name} COMMAND ut_${name})
endfunction()

# Base functionality.
set(base_libs
    ntlib_base
    ntlib_prime_generation
    ntlib_types
)
add_unittest(base "${base_libs}")

# Binomial coefficient.
set(binomial_coefficient_libs
    ntlib_base
    ntlib_combinatorics
    ntlib_prime_test
)
add_unittest(binomial_coefficient "${binomial_coefficient_libs}")

# Chinese remainder.
set(chinese_remainder_libs
    ntlib_chinese_remainder
)
add_unittest(chinese_remainder "${chinese_remainder_libs}")

# Diophantine equations.
set(diophantine_libs
    ntlib_diophantine
)
add_unittest(diophantine "${diophantine_libs}")

# Divisors.
set(divisors_libs
    ntlib_arithmetic_functions
    ntlib_prime_decomposition
)
add_unittest(divisors "${divisors_libs}")

# Euler's totient function.
set(euler_totient_libs
    ntlib_arithmetic_functions
)
add_unittest(euler_totient "${euler_totient_libs}")

# Figurate numbers.
set(figurate_number_libs
    ntlib_combinatorics
)
add_unittest(figurate_number "${figurate_number_libs}")

# 128 bit integers.
set(int128_libs
    ntlib_base
)
add_unittest(int128 "${int128_libs}")

# Lucas sequences.
set(lucas_sequence_libs
    ntlib_base
    ntlib_prime_test
)
add_unittest(lucas_sequence "${lucas_sequence_libs}")

# Matrices.
set(matrix_libs
    ntlib_base
    ntlib_types
)
add_unittest(matrix "${matrix_libs}")

# Modular arithmetic.
set(mod_int_libs
    ntlib_types
)
add_unittest(rt_mod_int "${mod_int_libs}")
add_unittest(ct_mod_int "${mod_int_libs}")

# Modulo.
set(modulo_libs
    ntlib_base
    ntlib_prime_decomposition
)
add_unittest(modulo "${modulo_libs}")

# Pell equation.
set(pell_equation_libs
    ntlib_base
    ntlib_diophantine
)
add_unittest(pell_equation "${pell_equation_libs}")

# Prime decomposition.
set(prime_decomposition_libs
    ntlib_base
    ntlib_prime_decomposition
    ntlib_prime_generation
    ntlib_prime_test
)
add_unittest(prime_decomposition "${prime_decomposition_libs}")

# Prime generation.
set(prime_generation_libs
    ntlib_prime_generation
    ntlib_prime_test
)
add_unittest(prime_generation "${prime_generation_libs}")

# Prime test.
set(prime_test_libs
    ntlib_base
    ntlib_prime_generation
    ntlib_prime_test
)
add_unittest(prime_test "${prime_test_libs}")

# Pythagorean triples.
set(pythagorean_triple_libs
    ntlib_base
    ntlib_combinatorics
)
add_unittest(pythagorean_triple "${pythagorean_triple_libs}")

# Rational numbers.
set(rational_libs
    ntlib_types
)
add_unittest(rational "${rational_libs}")

# 235-wheel sieve.
set(sieve_235_libs
    ntlib_prime_generation
)
add_unittest(sieve_235 "${sieve_235_libs}")

# Turan number.
set(turan_number_libs
    ntlib_combinatorics
)
add_unittest(turan_number "${turan_number_libs}")
