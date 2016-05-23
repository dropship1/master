#ifndef BRIGHT_CONTEXT_CONTEXT_H
#define BRIGHT_CONTEXT_CONTEXT_H

#include <windows.h>
#include <string>
#include "context/openglcontext.hpp"
#include <memory>

namespace bright{

  namespace context{


/**
 * @ingroup context
 *
 * @brief The context class
 * 
 * @details 
 * This represents a context that opengl may render to (e.g. A window)
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class Context{

public:

/** 
 * @brief Construct a new default context.
 * @details
 */
  Context();

/** 
 * @brief Update the context's window width and window height.
 * @param width The width to use.
 * @param height The height to use.
 */
  void reshape_window(int width, int height);


/** 
 * @brief Initialize the current opengl context
 */
  void initialize();

  void begin_rendering();

  void end_rendering();

/** 
 * @brief Update the context's window title.
 * @param title The windows title.
 */
  void window_title(std::string title);


/** 
 * @brief Get the context's window title.
 * @retval string The title.
 */
  std::string window_title();


/** 
 * @brief Get the context's window width.
 * @retval int The width.
 */
  int window_width();

/** 
 * @brief Get the context's window height.
 * @retval int The height.
 */
  int window_height();

/** 
 * @brief Get the context's rendering context.
 * @retval HGLRC The rendering context.
 */
  HGLRC window_rendering_context();

/** 
 * @brief Get the context's window id.
 * @retval HWND The id.
 */
  HWND window_id();

/** 
 * @brief Get the context's window device context.
 * @retval HDC The device context.
 */
  HDC window_device_context();

/** 
 * @brief Update the context's window values.
 * @param instApplicationInstance The Application Instance
 * @param wndWindowId The Window Id
 * @param dcDeviceContext The Device Context
 * @param glrcRenderingContext The Rendering Context
 * @param sTitle The Title
 * @param iMajorVersion The Major Version
 * @param iMinorVersion The Minor Version
 * @param iWindowWidth The Window's Width
 * @param iWindowHeight The Window's Height
 */
  void set_windows_opengl_context_values( HINSTANCE    applicationInstance, 
                                          HWND         windowId, 
                                          HDC          deviceContext, 
                                          HGLRC        renderingContext,
                                          std::string  title, 
                                          int          majorVersion, 
                                          int          minorVersion, 
                                          int          width,  
                                          int          height 
                                         );

private: 

  std::string title_;

  //(Windows window)With the Win32 API, each Window you see has its own WindowIdentifier  
  HWND windowId_;

  //We also have two different contexts we need to work with
  //(Graphics Card)DeviceContext is basically the graphics card
  //(OpenGL) RenderingContext is basically what the DeviceContext (graphics card) gives us to render to
  HDC deviceContext_; 
  HGLRC renderingContext_; 
  HINSTANCE applicationInstance_;

  int majorVersion_;
  int minorVersion_;

  //Dimensions of the Window
  int width_; 
  int height_;

  std::shared_ptr<OpenglContext> pOpenglContext_;
  
    
};


  }
}

#endif