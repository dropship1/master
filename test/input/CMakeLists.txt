
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
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/glew32d.lib"
${OPENGL_LIBRARIES}
)
set_target_properties(Test_Input_InputManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
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
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/glew32d.lib"
${OPENGL_LIBRARIES}
)
set_target_properties(Test_Input_InputContextManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
SOURCE_GROUP("source_input" FILES ${source_input_files})

#create_test("WindowedApp", "Input_InputManager", {"lib"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/input/inputmanager.cpp"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/input");
#create_test("WindowedApp", "Input_InputContextManager", {"lib"}, {besdkdir, "glew-1.11.0/include", "include", "boost-1.58.0"}, {"test/input/inputcontextmanager.cpp", "test/input/data/controls_config.cfg", "test/input/data/controls_contexts.cfg", "test/input/data/filelist"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/input");
