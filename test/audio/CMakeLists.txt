
cmake_minimum_required (VERSION 3.10.2)

create_test_project(
  "SoundManager"
  "Audio"
  "soundmanager.cpp"
  "${CMAKE_SOURCE_DIR}/include;${CMAKE_SOURCE_DIR}/glew-1.11.0/include"
  "executableWIN32" 
  "besdkClient;glew;opengl;openal"
)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(COPY "${CMAKE_SOURCE_DIR}/data/audio" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)