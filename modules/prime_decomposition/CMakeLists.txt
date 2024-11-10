add_library(ntlib_prime_decomposition STATIC)
target_sources(ntlib_prime_decomposition PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES
        ${PROJECT_SOURCE_DIR}/include/prime_list.hpp
)
target_sources(ntlib_prime_decomposition PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules/prime_decomposition
    FILES
        ${PROJECT_SOURCE_DIR}/modules/prime_decomposition/prime_decomposition.cpp
)
target_link_libraries(ntlib_prime_decomposition PRIVATE
    ntlib_base
    ntlib_prime_generation
    ntlib_prime_test
)