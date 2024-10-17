# Probably bad style, but really makes life easier.
file(GLOB all_headers
  "${PROJECT_SOURCE_DIR}/include/*.hpp"
  "${PROJECT_SOURCE_DIR}/benchmarks/experiments/*.hpp"
)
file(GLOB all_modules
  "${PROJECT_SOURCE_DIR}/modules/*.cpp"
)

# Function to create a benchmark.
# The target gets the prefix `bm_`.
#
# Parameters:
# name: The name of the benchmark. Must match the *.cpp file name.
function(add_benchmark name)
  add_executable(bm_${name})
  target_sources(bm_${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp)
  target_sources(bm_${name} PUBLIC
    FILE_SET ntlb_headers TYPE HEADERS
    BASE_DIRS
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/benchmarks/experiments
    FILES ${all_headers}
  )
  target_sources(bm_${name} PUBLIC
    FILE_SET ntlib_modules TYPE CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules
    FILES ${all_modules}
  )
  set_target_properties(bm_${name} PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(bm_${name} benchmark::benchmark)
endfunction()

add_benchmark(is_square)
add_benchmark(isqrt)
add_benchmark(odd_part)
add_benchmark(prime_decomposition)
add_benchmark(prime_generation)
add_benchmark(prime_test)
add_benchmark(sieve)