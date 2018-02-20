
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX D)
endif()


cmake_minimum_required (VERSION 3.10.2)
project (brightInputInputManager)
find_package(OpenGL REQUIRED)

message(STATUS "Creating brightInputInputManager Solution")

file(GLOB_RECURSE source_input_files "inputmanager.cpp")

message("${source_input_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/glew-1.11.0/include")

add_executable(Test_Input_InputManager WIN32 ${source_input_files})
target_link_libraries(
Test_Input_InputManager 
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib"
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/glew32sd.lib"
${OPENGL_LIBRARIES}
)
set_target_properties(Test_Input_InputManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Input_InputManager PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_input" FILES ${source_input_files})



project (brightInputInputContextManager)
find_package(OpenGL REQUIRED)

message(STATUS "Creating brightInputInputContextManager Solution")

file(GLOB_RECURSE source_input_files "inputcontextmanager.cpp")

message("${source_input_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/glew-1.11.0/include")

add_executable(Test_Input_InputContextManager WIN32 ${source_input_files})
target_link_libraries(
Test_Input_InputContextManager 
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib"
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/glew32sd.lib"
${OPENGL_LIBRARIES}
)
set_target_properties(Test_Input_InputContextManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Input_InputContextManager PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_input" FILES ${source_input_files})


file (COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)