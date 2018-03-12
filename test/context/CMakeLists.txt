
cmake_minimum_required (VERSION 3.10.2)

create_test_project(
  "Context"
  "Context"
  "context.cpp"
  "${CMAKE_SOURCE_DIR}/include;${CMAKE_SOURCE_DIR}/glew-1.11.0/include"
  "executableWIN32" 
  "besdkClient;glew;opengl"
)



create_test_project(
  "ContextManagerAndOpenglContext"
  "Context"
  "contextmanager_and_openglcontext.cpp"
  "${CMAKE_SOURCE_DIR}/include;${CMAKE_SOURCE_DIR}/glew-1.11.0/include"
  "executableWIN32" 
  "besdkClient;glew;opengl"
)