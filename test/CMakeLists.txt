# Function to create a unit test.
# The target gets the prefix `ut_`.
#
# TODO: Remove parameter `header` as soon as everything is in modules.
#
# Parameters:
# name: The name of the unit test. Must match the *.cpp file name.
# headers: List of included headers.
# modules: List of imported modules.
function(add_unittest name headers modules)
  add_executable(ut_${name})
  target_sources(ut_${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp)
  target_sources(ut_${name} PUBLIC
    FILE_SET ntlib_headers TYPE HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${headers}      
  )
  target_sources(ut_${name} PUBLIC
    FILE_SET ntlib_modules TYPE CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES ${modules}
  )
  set_target_properties(ut_${name} PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(ut_${name} GTest::gtest_main)
  add_test(NAME ${name} COMMAND ut_${name})
endfunction()

################################################################################

set(base_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/include/matrix.hpp
  ${PROJECT_SOURCE_DIR}/include/mod_int.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_generation.hpp
)
set(base_modlues "")
add_unittest(base
  "${base_headers}"
  "${base_modlues}"
)

################################################################################

set(binomial_coefficient_headers
  ${PROJECT_SOURCE_DIR}/include/modulo.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_test.hpp
)
set(binomial_coefficient_modules
  ${PROJECT_SOURCE_DIR}/modules/binomial_coefficient.cpp
)
add_unittest(binomial_coefficient
  "${binomial_coefficient_headers}"
  "${binomial_coefficient_modules}"
)

################################################################################

set(chinese_remainder_headers "")
set(chinese_remainder_modules
  ${PROJECT_SOURCE_DIR}/modules/chinese_remainder.cpp
)
add_unittest(chinese_remainder
  "${chinese_remainder_headers}"
  "${chinese_remainder_modules}")

################################################################################

set(ct_mod_int_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/include/mod_int.hpp
)
set(ct_mod_int_modules "")
add_unittest(ct_mod_int
  "${ct_mod_int_headers}"
  "${ct_mod_int_modules}"
)

################################################################################

set(diophantine_headers
  ${PROJECT_SOURCE_DIR}/include/diophantine.hpp
)
set(diophantine_modules "")
add_unittest(diophantine
  "${diophantine_headers}"
  "${diophantine_modules}"
)

################################################################################

set(divisors_headers
  ${PROJECT_SOURCE_DIR}/include/divisors.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_decomposition.hpp
)
set(divisors_modules "")
add_unittest(divisors
  "${divisors_headers}"
  "${divisors_modules}"
)

################################################################################

set(int128_headers
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
)
set(int128_modules "")
add_unittest(int128
  "${int128_headers}"
  "${int128_modules}"
)

################################################################################

set(lucas_sequence_headers
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/include/lucas_sequence.hpp
  ${PROJECT_SOURCE_DIR}/include/modulo.hpp
)
set(lucas_sequence_modules "")
add_unittest(lucas_sequence
  "${lucas_sequence_headers}"
  "${lucas_sequence_modules}"
)

################################################################################

set(matrix_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/include/matrix.hpp
  ${PROJECT_SOURCE_DIR}/include/mod_int.hpp
)
set(matrix_modules "")
add_unittest(matrix
  "${matrix_headers}"
  "${matrix_modules}"
)

################################################################################

set(mod_int_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/include/mod_int.hpp
)
set(mod_int_modules "")
add_unittest(mod_int
  "${mod_int_headers}"
  "${mod_int_modules}"
)

################################################################################

set(modulo_headers
  ${PROJECT_SOURCE_DIR}/include/modulo.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_decomposition.hpp
)
set(modulo_modules "")
add_unittest(modulo
  "${modulo_headers}"
  "${modulo_modules}"
)

################################################################################

set(pell_equation_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/include/pell_equation.hpp
)
set(pell_equation_modules "")
add_unittest(pell_equation
  "${pell_equation_headers}"
  "${pell_equation_modules}"
)

################################################################################

set(prime_decomposition_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_decomposition.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_generation.hpp
)
set(prime_decomposition_modules "")
add_unittest(prime_decomposition
  "${prime_decomposition_headers}"
  "${prime_decomposition_modules}"
)

################################################################################

set(prime_generation_headers
  ${PROJECT_SOURCE_DIR}/include/prime_generation.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_test.hpp
)
set(prime_generation_modules "")
add_unittest(prime_generation
  "${prime_generation_headers}"
  "${prime_generation_modules}"
)

################################################################################

set(prime_test_headers
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_generation.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_test.hpp
)
set(prime_test_modules "")
add_unittest(prime_test
  "${prime_test_headers}"
  "${prime_test_modules}"
)

################################################################################

set(pythagorean_triple_headers
  ${PROJECT_SOURCE_DIR}/include/pythagorean_triple.hpp
)
set(pythagorean_triple_modules "")
add_unittest(pythagorean_triple
  "${pythagorean_triple_headers}"
  "${pythagorean_triple_modules}"
)

################################################################################

set(rational_headers
  ${PROJECT_SOURCE_DIR}/include/rational.hpp
)
set(rational_modules "")
add_unittest(rational
  "${rational_headers}"
  "${rational_modules}"
)

################################################################################

set(sieve_headers
  ${PROJECT_SOURCE_DIR}/include/sieve.hpp
)
set(sieve_modlues "")
add_unittest(sieve
  "${sieve_headers}"
  "${sieve_modlues}"
)

################################################################################

set(sieve_235_headers
  ${PROJECT_SOURCE_DIR}/include/sieve_235.hpp
)
set(sieve_235_modules "")
add_unittest(sieve_235
  "${sieve_235_headers}"
  "${sieve_235_modules}"
)

################################################################################

set(turan_number_headers
  ${PROJECT_SOURCE_DIR}/include/turan_number.hpp
)
set(turan_number_modules "")
add_unittest(turan_number
  "${turan_number_headers}"
  "${turan_number_modules}"
)
