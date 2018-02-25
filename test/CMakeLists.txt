
cmake_minimum_required (VERSION 3.10.2)
project (brightTests)

message(STATUS "")
message(STATUS "")
message(STATUS "::::::::  Creating Bright Tests Solution  ::::::::")
message(STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
message(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY: " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})


add_subdirectory ("audio")
#add_subdirectory ("utils")
#add_subdirectory ("converters")
add_subdirectory ("graphics")
add_subdirectory ("base")
#add_subdirectory ("context")
add_subdirectory ("input")
#add_subdirectory ("physics")
add_subdirectory ("ai")
#add_subdirectory ("network")
