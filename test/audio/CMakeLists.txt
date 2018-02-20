
cmake_minimum_required (VERSION 3.10.2)
project (brightTestAudioSoundManager)
find_package(OpenGL REQUIRED)

message(STATUS "Creating brightTestAudioSoundManager Solution")

file(GLOB_RECURSE source_audio_files "soundmanager.cpp")

message("${source_audio_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/glew-1.11.0/include")

add_executable(Test_Audio_SoundManager WIN32 ${source_audio_files})
target_link_libraries(Test_Audio_SoundManager debug "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/besdkClientX64.lib")
target_link_libraries(Test_Audio_SoundManager optimized "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release/besdkClientX64.lib")
target_link_libraries(
Test_Audio_SoundManager
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/vendor/OpenAL/Win64/OpenAL32.lib"
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/vendor/OpenAL/Win64/EFX-Util_MT/EFX-Util.lib"
)
target_link_libraries(Test_Audio_SoundManager debug "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/vendor/glew/Debug/x64/glew32sd.lib")
target_link_libraries(Test_Audio_SoundManager optimized "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/vendor/glew/Release/x64/glew32s.lib")
target_link_libraries(Test_Audio_SoundManager ${OPENGL_LIBRARIES}
)
set_target_properties(Test_Audio_SoundManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "bin")
set_target_properties(Test_Audio_SoundManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin/Debug")
set_target_properties(Test_Audio_SoundManager PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")


SOURCE_GROUP("source_audio" FILES ${source_audio_files})

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(COPY "${CMAKE_SOURCE_DIR}/data/audio" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)