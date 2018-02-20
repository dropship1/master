
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX D)
endif()


cmake_minimum_required (VERSION 3.10.2)
project (brightTests)

message(STATUS "Creating Bright Tests Solution")
message(STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
message(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG})
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG: " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG})


add_subdirectory ("audio")
add_subdirectory ("utils")
add_subdirectory ("converters")
add_subdirectory ("graphics")
add_subdirectory ("base")
add_subdirectory ("context")
add_subdirectory ("input")
add_subdirectory ("physics")
add_subdirectory ("ai")
add_subdirectory ("network")
