
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX D)
endif()


cmake_minimum_required (VERSION 3.10.2)
project (brightTestContextContext)
find_package(OpenGL REQUIRED)

message(STATUS "Creating brightTestContextContext Solution")

file(GLOB_RECURSE source_context_files "context.cpp")

message("${source_context_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/glew-1.11.0/include")

add_executable(Test_Context_Context WIN32 ${source_context_files})
target_link_libraries(
Test_Context_Context 
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib"
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/glew32sd.lib"
${OPENGL_LIBRARIES}
)
set_target_properties(Test_Context_Context PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Context_Context PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_context" FILES ${source_context_files})





project (brightTestContextContextManagerAndOpenglContext)
find_package(OpenGL REQUIRED)

message(STATUS "Creating brightTestContextContextManagerAndOpenglContext Solution")

file(GLOB_RECURSE source_contextmanager_and_openglcontext_files "contextmanager_and_openglcontext.cpp")

message("${source_contextmanager_and_openglcontext_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/glew-1.11.0/include")

add_executable(Test_Context_ContextManager_And_OpenglContext WIN32 ${source_contextmanager_and_openglcontext_files})
target_link_libraries(
Test_Context_ContextManager_And_OpenglContext 
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib"
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/glew32sd.lib"
${OPENGL_LIBRARIES}
)
set_target_properties(Test_Context_ContextManager_And_OpenglContext PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Context_ContextManager_And_OpenglContext PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_contextmanager_and_openglcontext" FILES ${source_contextmanager_and_openglcontext_files})