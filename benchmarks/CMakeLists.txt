# Use GoogleBenchmark for microbenchmarking.
FetchContent_Declare(
    googlebenchmark
    # Specify the commit you depend on and update it regularly.
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG 12235e24652fc7f809373e7c11a5f73c5763fc4c # version 1.9.0
)
FetchContent_MakeAvailable(googlebenchmark)

# Probably bad style, but really makes life easier.
file(GLOB all_headers CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/include/*.hpp"
  "${PROJECT_SOURCE_DIR}/benchmarks/experiments/*.hpp"
)
file(GLOB all_modules CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/modules/*.cpp"
  "${PROJECT_SOURCE_DIR}/modules/**/*.cpp"
)

# Function to create a benchmark.
# The target gets the prefix `bm_`.
#
# Parameters:
# name: The name of the benchmark. Must match the *.cpp file name.
function(add_benchmark name)
  add_executable(bm_${name})
  target_compile_features(bm_${name} PUBLIC cxx_std_23)
  target_sources(bm_${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp)
  target_sources(bm_${name} PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/benchmarks/experiments
    FILES ${all_headers}
  )
  target_sources(bm_${name} PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS
      ${PROJECT_SOURCE_DIR}/modules
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