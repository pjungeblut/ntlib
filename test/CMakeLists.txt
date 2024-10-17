# Function to create a unit test.
# The target gets the prefix `ut_`.
#
# Parameters:
# name: The name of the unit test. Must match the *.cpp file name.
# headers: List of included header files.
function(add_unittest name headers)
  add_executable(ut_${name})
  target_sources(ut_${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp)
  target_sources(ut_${name} PUBLIC
    FILE_SET ntlib_headers TYPE HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${headers}      
  )
  set_target_properties(ut_${name} PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(ut_${name} GTest::gtest_main)
  add_test(NAME ${name} COMMAND ut_${name})
endfunction()

set(base_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/include/matrix.hpp
  ${PROJECT_SOURCE_DIR}/include/mod_int.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_generation.hpp
)
add_unittest(base ${base_headers})

set(binomial_coefficient_headers
  ${PROJECT_SOURCE_DIR}/include/binomial_coefficient.hpp
  ${PROJECT_SOURCE_DIR}/include/modulo.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_test.hpp
)
add_unittest(binomial_coefficient ${binomial_coefficient_headers})

set(chinese_remainder_headers
  ${PROJECT_SOURCE_DIR}/include/chinese_remainder.hpp
)
add_unittest(chinese_remainder ${chinese_remainder_headers})

set(ct_mod_int_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/include/mod_int.hpp
)
add_unittest(ct_mod_int ${ct_mod_int_headers})

set(diophantine_headers
  ${PROJECT_SOURCE_DIR}/include/diophantine.hpp
)
add_unittest(diophantine ${diophantine_headers})

set(divisors_headers
  ${PROJECT_SOURCE_DIR}/include/divisors.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_decomposition.hpp
)
add_unittest(divisors ${divisors_headers})

set(int128_headers
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
)
add_unittest(int128 ${int128_headers})

set(lucas_sequence_headers
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/include/lucas_sequence.hpp
  ${PROJECT_SOURCE_DIR}/include/modulo.hpp
)
add_unittest(lucas_sequence ${lucas_sequence_headers})

set(matrix_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/include/matrix.hpp
  ${PROJECT_SOURCE_DIR}/include/mod_int.hpp
)
add_unittest(matrix ${matrix_headers})

set(mod_int_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/include/mod_int.hpp
)
add_unittest(mod_int ${mod_int_headers})

set(modulo_headers
  ${PROJECT_SOURCE_DIR}/include/modulo.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_decomposition.hpp
)
add_unittest(modulo ${modulo_headers})

set(pell_equation_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/include/pell_equation.hpp
)
add_unittest(pell_equation ${pell_equation_headers})

set(prime_decomposition_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_decomposition.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_generation.hpp
)
add_unittest(prime_decomposition ${prime_decomposition_headers})

set(prime_generation_headers
  ${PROJECT_SOURCE_DIR}/include/prime_generation.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_test.hpp
)
add_unittest(prime_generation ${prime_generation_headers})

set(prime_test_headers
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_generation.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_test.hpp
)
add_unittest(prime_test ${prime_test_headers})

set(pythagorean_triple_headers
  ${PROJECT_SOURCE_DIR}/include/pythagorean_triple.hpp
)
add_unittest(pythagorean_triple ${pythagorean_triple_headers})

set(rational_headers
  ${PROJECT_SOURCE_DIR}/include/rational.hpp
)
add_unittest(rational ${rational_headers})

set(sieve_headers
  ${PROJECT_SOURCE_DIR}/include/sieve.hpp
)
add_unittest(sieve ${sieve_headers})

set(sieve_235_headers
  ${PROJECT_SOURCE_DIR}/include/sieve_235.hpp
)
add_unittest(sieve_235 ${sieve_235_headers})

set(turan_number_headers
  ${PROJECT_SOURCE_DIR}/include/turan_number.hpp
)
add_unittest(turan_number ${turan_number_headers})
