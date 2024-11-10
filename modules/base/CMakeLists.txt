add_library(ntlib_base STATIC)
target_sources(ntlib_base PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules/base
    FILES
        ${PROJECT_SOURCE_DIR}/modules/base/base.cpp
        ${PROJECT_SOURCE_DIR}/modules/base/int128.cpp
)