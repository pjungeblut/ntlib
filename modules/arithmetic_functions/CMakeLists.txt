add_library(ntlib_arithmetic_functions STATIC)
target_sources(ntlib_arithmetic_functions PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules/arithmetic_functions
    FILES
        ${PROJECT_SOURCE_DIR}/modules/arithmetic_functions/divisors.cpp
        ${PROJECT_SOURCE_DIR}/modules/arithmetic_functions/euler_totient.cpp
)
target_link_libraries(ntlib_arithmetic_functions PRIVATE
    ntlib_base
    ntlib_prime_decomposition
)