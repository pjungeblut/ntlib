add_library(ntlib_types STATIC)
target_compile_features(ntlib_types PUBLIC cxx_std_23)
target_sources(ntlib_types PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules/types
    FILES
        ${PROJECT_SOURCE_DIR}/modules/types/matrix.cpp
        ${PROJECT_SOURCE_DIR}/modules/types/mod_int.cpp
        ${PROJECT_SOURCE_DIR}/modules/types/rational.cpp
)
target_link_libraries(ntlib_types PRIVATE
    ntlib_base
)