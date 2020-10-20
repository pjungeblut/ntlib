# Include the ntlib headers.
include_directories(../include)

add_executable(bm_base ${CMAKE_CURRENT_SOURCE_DIR}/base.cpp)
target_link_libraries(bm_base benchmark)
