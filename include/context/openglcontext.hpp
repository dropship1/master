#ifndef BRIGHT_CONTEXT_OPENGLCONTEXT_H
#define BRIGHT_CONTEXT_OPENGLCONTEXT_H

#include <windows.h>
#include <iostream>

//External libraries
#include <gl/glew.h>
#include <memory>

#include <cassert>

namespace bright{

  namespace context{

/**
 * @ingroup context
 *
 * @brief The opengl context class
 * 
 * @details 
 * OpenGLContext is a class designed to store all of your OpenGL functions and keep them out of 
 * the way of your application logic. 
 * Here we have the ability to create an OpenGL context on a given window and then render 
 * to that window
 * So basically a wrapper to the opengl Rendering context we get from the graphics card that our
 * windows window is connected to.
 * This is basically a singleton so no virtual functions here.
 * This class will never be inherited
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class OpenglContext {

public:

/** 
 * @brief Construct a new default opengl context.
 * @details
 */
  OpenglContext();
  

/** 
 * @brief Setup can happen here before rendering.
 * @details
 * Contains anything we need to setup before we render.
 */
  void initialize();
  

/** 
 * @brief Display method from previous OpenGL tutorials.
 *
 * @details
 * Contains all our rendering code/calls;
 * First thing is set our viewport size to fill the entire window.
 * Next clear our COLOR, DEPTH and STENCIL buffers to avoid overlapping of renders.
 * Any other rendering code will go here.
 * Last, swap buffers. 
 * (Make buffer with new image the current buffer, and get rid of old buffer/image)
 */
  void begin_rendering();


/** 
 * @brief Display method from previous OpenGL tutorials.
 * @details
 */
  void swap_buffers(HDC deviceContext);
  

/** 
 * @brief Method to set our window width and height on resize.
 *
 * @details
 * Called every time our window is resized, and it resets our windowWidth and windowHeight so 
 * that we can set our viewport size.
 *
 * @todo
 * no aspect ratio handling yet
 *
 * @param iWidth The window width.
 * @param iHeight The window height.
 */
  void reshape_window(int width, int height); 


private:  


  static void opengl_debug_message_callback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);
    
};


  }
}

#endif