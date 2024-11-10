add_library(ntlib_prime_test STATIC)
target_sources(ntlib_prime_test PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules/prime_test
    FILES
        ${PROJECT_SOURCE_DIR}/modules/prime_test/lucas_sequence.cpp
        ${PROJECT_SOURCE_DIR}/modules/prime_test/prime_test.cpp
)
target_link_libraries(ntlib_prime_test PRIVATE
    ntlib_base
    ntlib_modulo
    ntlib_types
)