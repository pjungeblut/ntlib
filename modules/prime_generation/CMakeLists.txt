add_library(ntlib_prime_generation STATIC)
target_compile_features(ntlib_prime_generation PUBLIC cxx_std_23)
target_sources(ntlib_prime_generation PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules/prime_generation
    FILES
        ${PROJECT_SOURCE_DIR}/modules/prime_generation/sieve_235.cpp
        ${PROJECT_SOURCE_DIR}/modules/prime_generation/prime_generation.cpp
)
target_link_libraries(ntlib_prime_generation PRIVATE
    ntlib_base
    ntlib_prime_test
)