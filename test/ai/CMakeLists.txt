
cmake_minimum_required (VERSION 3.10.2)
project (brightTestAIAIManager)

message(STATUS "Creating brightTestAIAIManager Solution")

file(GLOB_RECURSE source_ai_aimanager_files "aimanager.cpp")

message("${source_ai_aimanager_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")

add_executable(Test_AI_AIManager ${source_ai_aimanager_files})
target_link_libraries(Test_AI_AIManager debug "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/besdkServerX64.lib")
target_link_libraries(Test_AI_AIManager optimized "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release/besdkServerX64.lib")
set_target_properties(Test_AI_AIManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "bin")
set_target_properties(Test_AI_AIManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin/Debug")
set_target_properties(Test_AI_AIManager PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_ai_aimanager" FILES ${source_ai_aimanager_files})

file (COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)