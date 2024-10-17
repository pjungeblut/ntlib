# Probably bad style, but really makes life easier.
file(GLOB all_headers
  "${PROJECT_SOURCE_DIR}/include/*.hpp"
)
file(GLOB all_modules
  "${PROJECT_SOURCE_DIR}/modules/*.cpp"
)

# Function to create a unit test.
# The target gets the prefix `ut_`.
#
# Parameters:
# name: The name of the unit test. Must match the *.cpp file name.
function(add_unittest name)
  add_executable(ut_${name})
  target_sources(ut_${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp)
  target_sources(ut_${name} PUBLIC
    FILE_SET ntlib_headers TYPE HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${all_headers}      
  )
  target_sources(ut_${name} PUBLIC
    FILE_SET ntlib_modules TYPE CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES ${all_modules}
  )
  set_target_properties(ut_${name} PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(ut_${name} GTest::gtest_main)
  add_test(NAME ${name} COMMAND ut_${name})
endfunction()

add_unittest(base)
add_unittest(binomial_coefficient)
add_unittest(chinese_remainder)
add_unittest(ct_mod_int)
add_unittest(diophantine)
add_unittest(divisors)
add_unittest(int128)
add_unittest(lucas_sequence)
add_unittest(matrix)
add_unittest(mod_int)
add_unittest(modulo)
add_unittest(pell_equation)
add_unittest(prime_decomposition)
add_unittest(prime_generation)
add_unittest(prime_test)
add_unittest(pythagorean_triple)
add_unittest(rational)
add_unittest(sieve)
add_unittest(sieve_235)
add_unittest(turan_number)
