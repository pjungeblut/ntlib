include_directories("${CMAKE_CURRENT_BINARY_DIR}/../include")

# Select all *.cpp files.
file(GLOB libFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

set(ntlib_SRCS ${libFiles})
add_library(ntlib ${ntlib_SRCS})

install(TARGETS ntlib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
