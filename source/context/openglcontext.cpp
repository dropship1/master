#include "context/openglcontext.hpp"

using namespace bright::context;


void OpenglContext::opengl_debug_message_callback(GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* message,
  const void* userParam)
{
  fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
    (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
    type, severity, message);
  assert(0);
}

OpenglContext::OpenglContext(){}


void OpenglContext::initialize(){
  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);

  std::cout << "DEBUG_OPENGL defined? "  << std::endl;

#ifdef DEBUG_OPENGL
  std::cout << "YES " << std::endl;
  // During init, enable debug output
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback((GLDEBUGPROC)OpenglContext::opengl_debug_message_callback, 0);
#endif
#ifndef DEBUG_OPENGL
  std::cout << "NO " << std::endl;
#endif

}

void OpenglContext::reshape_window(int width, int height) {

  glViewport(0, 0, width, height);
    
}


void OpenglContext::begin_rendering() {

  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  

}


void OpenglContext::swap_buffers(HDC deviceContext) {
  SwapBuffers( deviceContext ); 
}