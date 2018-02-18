
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX D)
endif()


cmake_minimum_required (VERSION 3.10.2)
project (brightTestUtilsFileWorker)

message(STATUS "Creating brightTestUtilsFileWorker Solution")

file(GLOB_RECURSE source_utils_files "fileworker.cpp")

message("${source_utils_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")

add_executable(Test_Utils_FileWorker ${source_utils_files})
target_link_libraries(Test_Utils_FileWorker "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib")
set_target_properties(Test_Utils_FileWorker PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
SOURCE_GROUP("source_utils" FILES ${source_utils_files})
