# Include the necessary files.
include_directories(../include)

# Select all *.cpp files.
file(GLOB cppFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Compile each file.
foreach(cppFile ${cppFiles})
  add_executable("${cppFile}" "${cppFile}")
  target_link_libraries("${cppFile}" gtest_main)
  add_test(NAME "${cppFile}" COMMAND "${cppFile}")
endForeach(cppFile)
