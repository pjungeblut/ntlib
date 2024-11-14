add_library(ntlib_diophantine STATIC)
target_compile_features(ntlib_diophantine PUBLIC cxx_std_23)
target_sources(ntlib_diophantine PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS ${PROJECT_SOURCE_DIR}/modules/diophantine
    FILES
        ${PROJECT_SOURCE_DIR}/modules/diophantine/continued_fraction.cpp
        ${PROJECT_SOURCE_DIR}/modules/diophantine/diophantine.cpp
        ${PROJECT_SOURCE_DIR}/modules/diophantine/pell_equation.cpp
)
target_link_libraries(ntlib_diophantine PRIVATE
    ntlib_base
    ntlib_types
)