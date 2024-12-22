add_library(ntlib_base STATIC)
target_compile_features(ntlib_base PUBLIC cxx_std_23)
target_sources(ntlib_base PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules/base
    FILES
        ${PROJECT_SOURCE_DIR}/modules/base/base.cpp
        ${PROJECT_SOURCE_DIR}/modules/base/int128.cpp
        ${PROJECT_SOURCE_DIR}/modules/base/modulo.cpp
)