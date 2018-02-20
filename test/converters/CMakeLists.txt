
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX D)
endif()


cmake_minimum_required (VERSION 3.10.2)
project (brightTestConvertersMeshConverter)

message(STATUS "Creating brightTestConvertersMeshConverter Solution")

file(GLOB_RECURSE source_converters_meshconverter_files "meshconverter.cpp")

message("${source_converters_meshconverter_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")

add_executable(Test_Converters_MeshConverter ${source_converters_meshconverter_files})
target_link_libraries(Test_Converters_MeshConverter "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib")
set_target_properties(Test_Converters_MeshConverter PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Converters_MeshConverter PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_converters_meshconverter" FILES ${source_converters_meshconverter_files})



project (brightTestConvertersAABBConverter)

file(GLOB_RECURSE source_converters_aabbconverter_files "aabbconverter.cpp")

message("${source_converters_aabbconverter_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")

add_executable(Test_Converters_AABBConverter ${source_converters_aabbconverter_files})
target_link_libraries(Test_Converters_AABBConverter "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkServerD.lib")
set_target_properties(Test_Converters_AABBConverter PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Converters_AABBConverter PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_converters_aabbconverter" FILES ${source_converters_aabbconverter_files})

file (COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file (COPY "${CMAKE_SOURCE_DIR}/data/aabbs" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)
file (COPY "${CMAKE_SOURCE_DIR}/data/meshes" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)