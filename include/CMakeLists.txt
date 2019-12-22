# Select all *.hpp files.
file(GLOB hppFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# Copy each file.
foreach(hppFile ${hppFiles})
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${hppFile}" "${CMAKE_CURRENT_BINARY_DIR}/${hppFile}" COPYONLY)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${hppFile}" DESTINATION include/ntlib)
endForeach(hppFile)
