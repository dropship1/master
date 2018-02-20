
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX D)
endif()


cmake_minimum_required (VERSION 3.10.2)
project (brightTestGraphicsRenderer)
find_package(OpenGL REQUIRED)

message(STATUS "Creating brightTestGraphicsRenderer Solution")

file(GLOB_RECURSE source_graphics_files "renderer.cpp")

message("${source_graphics_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/glew-1.11.0/include")

add_executable(Test_Graphics_Renderer WIN32 ${source_graphics_files})
target_link_libraries(
Test_Graphics_Renderer 
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib"
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/glew32sd.lib"
${OPENGL_LIBRARIES}
)
set_target_properties(Test_Graphics_Renderer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Graphics_Renderer PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_graphics" FILES ${source_graphics_files})




project (brightTestGraphicsRendererAndTextures)
find_package(OpenGL REQUIRED)

message(STATUS "Creating brightTestGraphicsRendererAndTextures Solution")

file(GLOB_RECURSE source_graphics_files "rendererandtextures.cpp")

message("${source_graphics_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/glew-1.11.0/include")

add_executable(Test_Graphics_RendererAndTextures WIN32 ${source_graphics_files})
target_link_libraries(
Test_Graphics_RendererAndTextures 
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib"
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/glew32sd.lib"
${OPENGL_LIBRARIES}
)
set_target_properties(Test_Graphics_RendererAndTextures PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Graphics_RendererAndTextures PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_graphics" FILES ${source_graphics_files})




project (brightTestGraphicsShaderLoader)
find_package(OpenGL REQUIRED)

message(STATUS "Creating brightTestGraphicsShaderLoader Solution")

file(GLOB_RECURSE source_graphics_files "shaderloader.cpp")

message("${source_graphics_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/glew-1.11.0/include")

add_executable(Test_Graphics_ShaderLoader WIN32 ${source_graphics_files})
target_link_libraries(
Test_Graphics_ShaderLoader	 
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib"
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/glew32sd.lib"
${OPENGL_LIBRARIES}
)
set_target_properties(Test_Graphics_ShaderLoader PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Graphics_ShaderLoader PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_graphics" FILES ${source_graphics_files})






project (brightTestGraphicsTextureLoader)
find_package(OpenGL REQUIRED)

message(STATUS "Creating brightTestGraphicsTextureLoader Solution")

file(GLOB_RECURSE source_graphics_files "textureloader.cpp")

message("${source_graphics_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/glew-1.11.0/include")

add_executable(Test_Graphics_TextureLoader WIN32 ${source_graphics_files})
target_link_libraries(
Test_Graphics_TextureLoader	 
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib"
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/glew32sd.lib"
${OPENGL_LIBRARIES}
)
set_target_properties(Test_Graphics_TextureLoader PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Graphics_TextureLoader PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_graphics" FILES ${source_graphics_files})




project (brightTestGraphicsLoaderManager)
find_package(OpenGL REQUIRED)

message(STATUS "Creating brightTestGraphicsLoaderManager Solution")

file(GLOB_RECURSE source_graphics_files "loadersmanager.cpp")

message("${source_graphics_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/glew-1.11.0/include")

add_executable(Test_Graphics_LoadersManager WIN32 ${source_graphics_files})
target_link_libraries(
Test_Graphics_LoadersManager	 
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib"
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/glew32sd.lib"
${OPENGL_LIBRARIES}
)
set_target_properties(Test_Graphics_LoadersManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Graphics_LoadersManager PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_graphics" FILES ${source_graphics_files})



project (brightTestGraphicsImageLoader)
message(STATUS "Creating brightTestGraphicsImageLoader Solution")

file(GLOB_RECURSE source_graphics_imageloader_files "imageloader.cpp")

message("${source_graphics_imageloader_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")

add_executable(Test_Graphics_ImageLoader ${source_graphics_imageloader_files})
target_link_libraries(Test_Graphics_ImageLoader "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib")
set_target_properties(Test_Graphics_ImageLoader PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
set_target_properties(Test_Graphics_ImageLoader PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
SOURCE_GROUP("source_graphics_imageloader" FILES ${source_graphics_imageloader_files})




file (COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file (COPY "${CMAKE_SOURCE_DIR}/data/shaders" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)
file (COPY "${CMAKE_SOURCE_DIR}/data/meshes" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)
file (COPY "${CMAKE_SOURCE_DIR}/data/textures" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)