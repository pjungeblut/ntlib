# Function to create a benchmark.
# The target gets the prefix `bm_`.
#
# Parameters:
# name: The name of the benchmark. Must match the *.cpp file name.
# headers: List of included NTLib header files.
function(add_benchmark name headers)
  add_executable(bm_${name})
  target_sources(bm_${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp)
  target_sources(bm_${name} PUBLIC
    FILE_SET headers TYPE HEADERS
    BASE_DIRS
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/benchmarks/experiments
    FILES ${headers}
  )
  set_target_properties(bm_${name} PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(bm_${name} benchmark::benchmark)
endfunction()

################################################################################

set(is_square_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/benchmarks/experiments/is_square.hpp
)
add_benchmark(is_square "${is_square_headers}")

################################################################################

set(isqrt_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/benchmarks/experiments/isqrt.hpp
)
add_benchmark(isqrt "${isqrt_headers}")

################################################################################

set(odd_part_headers
  ${PROJECT_SOURCE_DIR}/include/base.hpp
  ${PROJECT_SOURCE_DIR}/benchmarks/experiments/odd_part.hpp
)
add_benchmark(odd_part "${odd_part_headers}")

################################################################################

set(prime_decomposition_headers
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_decomposition.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_generation.hpp
  ${PROJECT_SOURCE_DIR}/benchmarks/experiments/prime_decomposition.hpp
)
add_benchmark(prime_decomposition "${prime_decomposition_headers}")

################################################################################

set(prime_generation_headers
  ${PROJECT_SOURCE_DIR}/include/prime_generation.hpp
  ${PROJECT_SOURCE_DIR}/include/sieve.hpp
  ${PROJECT_SOURCE_DIR}/include/sieve_235.hpp
  ${PROJECT_SOURCE_DIR}/benchmarks/experiments/prime_generation.hpp
)
add_benchmark(prime_generation "${prime_generation_headers}")

################################################################################

set(prime_test_headers
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_test.hpp
  ${PROJECT_SOURCE_DIR}/benchmarks/experiments/prime_test.hpp
)
add_benchmark(prime_test "${prime_test_headers}")

################################################################################

set(sieve_headers
  ${PROJECT_SOURCE_DIR}/include/sieve.hpp
  ${PROJECT_SOURCE_DIR}/include/sieve_235.hpp
)
add_benchmark(sieve "${sieve_headers}")
