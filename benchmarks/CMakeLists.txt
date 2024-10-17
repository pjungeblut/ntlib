# Function to create a benchmark.
# The target gets the prefix `bm_`.
#
# TODO: Remove `headers` when everything is in modules.
#
# Parameters:
# name: The name of the benchmark. Must match the *.cpp file name.
# headers: List of included NTLib headers.
# modules: List of importes NTLib modules.
function(add_benchmark name headers modules)
  add_executable(bm_${name})
  target_sources(bm_${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp)
  target_sources(bm_${name} PUBLIC
    FILE_SET ntlb_headers TYPE HEADERS
    BASE_DIRS
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/benchmarks/experiments
    FILES ${headers}
  )
  target_sources(bm_${name} PUBLIC
    FILE_SET ntlib_modules TYPE CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES ${modules}
  )
  set_target_properties(bm_${name} PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(bm_${name} benchmark::benchmark)
endfunction()

################################################################################

set(is_square_headers
  ${PROJECT_SOURCE_DIR}/benchmarks/experiments/is_square.hpp
)
set(is_square_modules
  ${PROJECT_SOURCE_DIR}/modules/base.cpp
)
add_benchmark(is_square
  "${is_square_headers}"
  "${is_square_modules}"
)

################################################################################

set(isqrt_headers
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/benchmarks/experiments/isqrt.hpp
)
set(isqrt_modules
  ${PROJECT_SOURCE_DIR}/modules/base.cpp
)
add_benchmark(isqrt
  "${isqrt_headers}"
  "${isqrt_modules}"
)

################################################################################

set(odd_part_headers
  ${PROJECT_SOURCE_DIR}/benchmarks/experiments/odd_part.hpp
)
set(odd_part_modules
  ${PROJECT_SOURCE_DIR}/modules/base.cpp
)
add_benchmark(odd_part
  "${odd_part_headers}"
  "${odd_part_modules}"
)

################################################################################

set(prime_decomposition_headers
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_decomposition.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_generation.hpp
  ${PROJECT_SOURCE_DIR}/benchmarks/experiments/prime_decomposition.hpp
)
set(prime_decomposition_modules
  ${PROJECT_SOURCE_DIR}/modules/base.cpp
)
add_benchmark(prime_decomposition
  "${prime_decomposition_headers}"
  "${prime_decomposition_modules}"
)

################################################################################

set(prime_generation_headers
  ${PROJECT_SOURCE_DIR}/include/prime_generation.hpp
  ${PROJECT_SOURCE_DIR}/include/sieve.hpp
  ${PROJECT_SOURCE_DIR}/include/sieve_235.hpp
  ${PROJECT_SOURCE_DIR}/benchmarks/experiments/prime_generation.hpp
)
set(prime_generation_modules
  ${PROJECT_SOURCE_DIR}/modules/base.cpp
)
add_benchmark(prime_generation
  "${prime_generation_headers}"
  "${prime_generation_modules}"
)

################################################################################

set(prime_test_headers
  ${PROJECT_SOURCE_DIR}/include/int128.hpp
  ${PROJECT_SOURCE_DIR}/include/prime_test.hpp
  ${PROJECT_SOURCE_DIR}/benchmarks/experiments/prime_test.hpp
)
set(prime_test_modules
  ${PROJECT_SOURCE_DIR}/modules/base.cpp
)
add_benchmark(prime_test
  "${prime_test_headers}"
  "${prime_test_modules}"
)

################################################################################

set(sieve_headers
  ${PROJECT_SOURCE_DIR}/include/sieve.hpp
  ${PROJECT_SOURCE_DIR}/include/sieve_235.hpp
)
set(sieve_modlues "")
add_benchmark(sieve
  "${sieve_headers}"
  "${sieve_modlues}"
)
