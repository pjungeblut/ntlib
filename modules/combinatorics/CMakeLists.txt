add_library(ntlib_combinatorics STATIC)
target_compile_features(ntlib_combinatorics PUBLIC cxx_std_23)
target_sources(ntlib_combinatorics PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules/combinatorics
    FILES
        ${PROJECT_SOURCE_DIR}/modules/combinatorics/binomial_coefficient.cpp
        ${PROJECT_SOURCE_DIR}/modules/combinatorics/figurate_number.cpp
        ${PROJECT_SOURCE_DIR}/modules/combinatorics/pythagorean_triple.cpp
        ${PROJECT_SOURCE_DIR}/modules/combinatorics/turan_number.cpp
)
target_link_libraries(ntlib_combinatorics PRIVATE
    ntlib_base
    ntlib_chinese_remainder
    ntlib_modulo
    ntlib_prime_decomposition
    ntlib_prime_test
)