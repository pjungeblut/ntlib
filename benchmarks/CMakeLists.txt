# Include the ntlib headers.
include_directories(..)

function(add_benchmark name)
  add_executable(bm_${name} ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp)
  set_target_properties(bm_${name} PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(bm_${name} benchmark::benchmark)
endfunction()

add_benchmark(isqrt)
