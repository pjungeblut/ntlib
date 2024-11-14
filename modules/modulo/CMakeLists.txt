add_library(ntlib_modulo STATIC)
target_compile_features(ntlib_modulo PUBLIC cxx_std_23)
target_sources(ntlib_modulo PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules/modulo/
    FILES
        ${PROJECT_SOURCE_DIR}/modules/modulo/mod_int.cpp
        ${PROJECT_SOURCE_DIR}/modules/modulo/modulo.cpp
)
target_link_libraries(ntlib_modulo PRIVATE
    ntlib_base
)