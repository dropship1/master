
cmake_minimum_required (VERSION 3.10.2)

create_test_project(
  "ActorControlsResourceManager"
  "Base"
  "actorcontrolsresourcemanager.cpp"
  "${CMAKE_SOURCE_DIR}/include"
  "executable" 
  "besdkServer"
)


create_test_project(
  "ActorRenderingResourceManager"
  "Base"
  "actorrenderingresourcemanager.cpp"
  "${CMAKE_SOURCE_DIR}/include;${CMAKE_SOURCE_DIR}/glew-1.11.0/include"
  "executableWIN32" 
  "besdkClient;glew;opengl"
)


create_test_project(
  "RenderActor"
  "Base"
  "renderactor.cpp"
  "${CMAKE_SOURCE_DIR}/include"
  "executable" 
  "besdkClient"
)


file (COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file (COPY "${CMAKE_SOURCE_DIR}/data/shaders" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)
file (COPY "${CMAKE_SOURCE_DIR}/data/meshes" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)
file (COPY "${CMAKE_SOURCE_DIR}/data/textures" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)