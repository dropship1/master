#include "context/openglcontext.hpp"

using namespace bright::context;


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