function(create_test_project projectName subdirName sourceFileslist includes type client_or_server)
  message("projectName: ${projectName}")
  message("subdirName: ${subdirName}")
  message("sourceFileslist: ${sourceFileslist}")
  message("includes: ${includes}")
  message("type: ${type}")
  message("client_or_server: ${client_or_server}")

  set(projectFullName "brightTest${projectName}")
  project(${projectFullName})
  message(STATUS "Creating ${projectFullName} Solution")
  string(TOLOWER ${projectName} lcProjectName)

  set(source_files_list_name source_${lcProjectName}_files)
  file(GLOB_RECURSE  sourceFiles ${sourceFileslist})
  
  message("${source_files_list_name}")

  include_directories(${includes})

  set(target Test_${subdirName}_${projectName})
  if(${type} MATCHES "executable")
    add_executable(${target} ${sourceFiles})
  endif()

  if(${client_or_server} MATCHES "server")
    target_link_libraries(${target} debug "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/besdkServerX64.lib")
    target_link_libraries(${target} optimized "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release/besdkServerX64.lib")
  elseif(${client_or_server} MATCHES "client")
    target_link_libraries(${target} debug "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/besdkClientX64.lib")
    target_link_libraries(${target} optimized "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release/besdkClientX64.lib")
  endif()

  set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "bin")
  set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin/Debug")
  set_target_properties(${target} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")

  SOURCE_GROUP(${source_files_list_name} FILES ${sourceFiles})

endfunction(create_test_project)


set(includes "${CMAKE_SOURCE_DIR}/include")
create_test_project(
  "BaseActorControlsResourceManager"
  "Base"
  "actorcontrolsresourcemanager.cpp"
  "${includes}"
  "executable" 
  "server"
)

#project (brightTestBaseActorControlsResourceManager)
#message(STATUS "Creating brightTestBaseActorControlsResourceManager Solution")
#
#file(GLOB_RECURSE source_base_actorcontrolsresourcemanager_files "actorcontrolsresourcemanager.cpp")
#
#message("${source_base_actorcontrolsresourcemanager_files}")
#
#include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")
#
#add_executable(Test_Base_ActorControlsResourceManager ${source_base_actorcontrolsresourcemanager_files})
#
#target_link_libraries(Test_Base_ActorControlsResourceManager debug "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/besdkServerX64.lib")
#target_link_libraries(Test_Base_ActorControlsResourceManager optimized "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release/besdkServerX64.lib")
#
#set_target_properties(Test_Base_ActorControlsResourceManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "bin")
#set_target_properties(Test_Base_ActorControlsResourceManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin/Debug")
#set_target_properties(Test_Base_ActorControlsResourceManager PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
#SOURCE_GROUP("source_base_actorcontrolsresourcemanager" FILES ${source_base_actorcontrolsresourcemanager_files})
#
#
#
#project (brightTestBaseActorRenderingResourceManager)
#message(STATUS "Creating brightTestBaseActorRenderingResourceManager Solution")
#find_package(OpenGL REQUIRED)
#
#file(GLOB_RECURSE source_base_actorrenderingresourcemanager_files "actorrenderingresourcemanager.cpp")
#
#message("${source_base_actorrenderingresourcemanager_files}")
#
#include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/glew-1.11.0/include")
#
#add_executable(Test_Base_ActorRenderingResourceManager WIN32 ${source_base_actorrenderingresourcemanager_files})
#
#target_link_libraries(Test_Base_ActorRenderingResourceManager debug "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/besdkClientX64.lib")
#target_link_libraries(Test_Base_ActorRenderingResourceManager optimized "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release/besdkClientX64.lib")
#
#target_link_libraries(Test_Base_ActorRenderingResourceManager debug "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/vendor/glew/Debug/x64/glew32sd.lib")
#target_link_libraries(Test_Base_ActorRenderingResourceManager optimized "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/vendor/glew/Release/x64/glew32s.lib")
#target_link_libraries(Test_Base_ActorRenderingResourceManager ${OPENGL_LIBRARIES})
#
#set_target_properties(Test_Base_ActorRenderingResourceManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "bin")
#set_target_properties(Test_Base_ActorRenderingResourceManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin/Debug")
#set_target_properties(Test_Base_ActorRenderingResourceManager PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
#SOURCE_GROUP("source_base_actorrenderingresourcemanager" FILES ${source_base_actorrenderingresourcemanager_files})
#
#
#project (brightTestBaseRenderActor)
#message(STATUS "Creating brightTestBaseRenderActor Solution")
#
#file(GLOB_RECURSE source_base_renderactor_files "renderactor.cpp")
#
#message("${source_base_renderactor_files}")
#
#include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")
#
#add_executable(Test_Base_RenderActor ${source_base_renderactor_files})
#
#target_link_libraries(Test_Base_RenderActor debug "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/besdkClientX64.lib")
#target_link_libraries(Test_Base_RenderActor optimized "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release/besdkClientX64.lib")
#
#set_target_properties(Test_Base_RenderActor PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "bin")
#set_target_properties(Test_Base_RenderActor PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin/Debug")
#set_target_properties(Test_Base_RenderActor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
#SOURCE_GROUP("source_base_renderactor" FILES ${source_base_renderactor_files})

file (COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file (COPY "${CMAKE_SOURCE_DIR}/data/shaders" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)
file (COPY "${CMAKE_SOURCE_DIR}/data/meshes" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)
file (COPY "${CMAKE_SOURCE_DIR}/data/textures" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)