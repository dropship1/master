
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX D)
endif()


cmake_minimum_required (VERSION 3.10.2)
project (brightTestPhysicsAABB)

message(STATUS "Creating brightTestPhysicsAABB Solution")

file(GLOB_RECURSE source_physics_aabb_files "aabb.cpp")

message("${source_physics_aabb_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")

add_executable(Test_Physics_AABB ${source_physics_aabb_files})
target_link_libraries(Test_Physics_AABB "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkServerD.lib")
set_target_properties(Test_Physics_AABB PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Physics_AABB PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_physics_aabb" FILES ${source_physics_aabb_files})