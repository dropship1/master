
cmake_minimum_required (VERSION 3.10.2)

create_test_project(
  "MeshConverter"
  "Converters"
  "meshconverter.cpp"
  "${CMAKE_SOURCE_DIR}/include"
  "executable" 
  "besdkClient"
)


project (brightTestConvertersAABBConverter)

create_test_project(
  "AABBConverter"
  "Converters"
  "aabbconverter.cpp"
  "${CMAKE_SOURCE_DIR}/include"
  "executable" 
  "besdkServer"
)

file (COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file (COPY "${CMAKE_SOURCE_DIR}/data/aabbs" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)
file (COPY "${CMAKE_SOURCE_DIR}/data/meshes" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)