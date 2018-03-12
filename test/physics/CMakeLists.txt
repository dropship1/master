
cmake_minimum_required (VERSION 3.10.2)


create_test_project(
  "AABB"
  "Physics"
  "aabb.cpp"
  "${CMAKE_SOURCE_DIR}/include"
  "executable" 
  "besdkServer"
)