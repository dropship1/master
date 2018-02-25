
cmake_minimum_required (VERSION 3.10.2)

create_test_project(
  "InputManager"
  "Input"
  "inputmanager.cpp"
  "${CMAKE_SOURCE_DIR}/include;${CMAKE_SOURCE_DIR}/glew-1.11.0/include"
  "executableWIN32" 
  "besdkClient;glew;opengl"
)



create_test_project(
  "InputContextManager"
  "Input"
  "inputcontextmanager.cpp"
  "${CMAKE_SOURCE_DIR}/include;${CMAKE_SOURCE_DIR}/glew-1.11.0/include"
  "executableWIN32" 
  "besdkClient;glew;opengl"
)


file (COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)