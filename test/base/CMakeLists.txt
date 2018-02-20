
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX D)
endif()

project (brightTestBaseActorControlsResourceManager)
message(STATUS "Creating brightTestBaseActorControlsResourceManager Solution")

file(GLOB_RECURSE source_base_actorcontrolsresourcemanager_files "actorcontrolsresourcemanager.cpp")

message("${source_base_actorcontrolsresourcemanager_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")

add_executable(Test_Base_ActorControlsResourceManager ${source_base_actorcontrolsresourcemanager_files})
target_link_libraries(Test_Base_ActorControlsResourceManager "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkServerD.lib")
set_target_properties(Test_Base_ActorControlsResourceManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Base_ActorControlsResourceManager PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_base_actorcontrolsresourcemanager" FILES ${source_base_actorcontrolsresourcemanager_files})




project (brightTestBaseActorRenderingResourceManager)
message(STATUS "Creating brightTestBaseActorRenderingResourceManager Solution")
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE source_base_actorrenderingresourcemanager_files "actorrenderingresourcemanager.cpp")

message("${source_base_actorrenderingresourcemanager_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/glew-1.11.0/include")

add_executable(Test_Base_ActorRenderingResourceManager WIN32 ${source_base_actorrenderingresourcemanager_files})
target_link_libraries(
Test_Base_ActorRenderingResourceManager	 
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib"
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/glew32sd.lib"
${OPENGL_LIBRARIES}
)
set_target_properties(Test_Base_ActorRenderingResourceManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Base_ActorRenderingResourceManager PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_base_actorrenderingresourcemanager" FILES ${source_base_actorrenderingresourcemanager_files})


project (brightTestBaseRenderActor)
message(STATUS "Creating brightTestBaseRenderActor Solution")

file(GLOB_RECURSE source_base_renderactor_files "renderactor.cpp")

message("${source_base_renderactor_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/glew-1.11.0/include")

add_executable(Test_Base_RenderActor ${source_base_renderactor_files})
target_link_libraries(Test_Base_RenderActor "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib")
set_target_properties(Test_Base_RenderActor PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Base_RenderActor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_base_renderactor" FILES ${source_base_renderactor_files})

file (COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file (COPY "${CMAKE_SOURCE_DIR}/data/shaders" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)
file (COPY "${CMAKE_SOURCE_DIR}/data/meshes" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)
file (COPY "${CMAKE_SOURCE_DIR}/data/textures" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)