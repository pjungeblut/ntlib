add_library(ntlib_chinese_remainder STATIC)
target_compile_features(ntlib_chinese_remainder PUBLIC cxx_std_23)
target_sources(ntlib_chinese_remainder PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules/chinese_remainder
    FILES
        ${PROJECT_SOURCE_DIR}/modules/chinese_remainder/chinese_remainder.cpp
)
target_link_libraries(ntlib_chinese_remainder PRIVATE
    ntlib_base
    ntlib_modulo
    ntlib_prime_decomposition
    ntlib_prime_generation
)