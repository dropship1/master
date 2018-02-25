
cmake_minimum_required (VERSION 3.10.2)


create_test_project(
  "AIManager"
  "AI"
  "aimanager.cpp"
  "${CMAKE_SOURCE_DIR}/include"
  "executable" 
  "besdkServer"
)


file (COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)