#ifndef BRIGHT_CONTEXT_CONTEXTMANAGER_H
#define BRIGHT_CONTEXT_CONTEXTMANAGER_H

//Raw input stuff
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

//Include glew before gl.h
#include "Gl/glew.h"
#include "Gl/wglew.h"

//opengl
#include <gl/GL.h>

#include <windows.h>
#include <string>
#include <map>
#include <algorithm>
#include <memory>
#include <array>

#include "context/context.hpp"

namespace bright{

  namespace context{


/**
 * @ingroup context
 *
 * @brief The context manager class
 * 
 * @details 
 * This represents a context manager that manages all the current context that we may be 
 * rendering to with opengl.
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class ContextManager {

public:

/** 
 * @brief Construct a new default context manager.
 * @details
 */
  ContextManager();

/** 
 * @brief Destroy the context manager.
 * @details
 */ 
  ~ContextManager();


/** 
 * @brief Creation of our window with renderable context
 *
 * @details
 *
 * @param wndpProc The windows procedure to call/feed info when a window event occurs.
 * @param sTitle The title of the window.
 * @param iWidth The windows width.
 * @param iHeight The windows height.
 *
 */
  std::shared_ptr<Context> create_windows_opengl_context(WNDPROC proc, std::string title, int width, int height);


/** 
 * @brief Get the current context.
 * @retval std::shared_ptr<Context> The current context.
 */
  std::shared_ptr<Context> windows_current_context();


  void remove_system_menu();


  void exit();

/** 
 * @brief Show the window that has been created.
 * @param windowTitle The window that should be shown based on the title.
 */
  void show_window(bool hidePrevious);

  void set_current_context(std::string windowId);

  void constrain_cursor();

  void reshape_context(bool isFullScreen, int width, int height);
  void reshape_rendering_context_viewport(int width, int height);

  void begin_rendering();
  void end_rendering();

  void initialize();

  int show_cursor(bool hide);

private:
  std::shared_ptr<Context> pCurrentContext_;
  std::shared_ptr<Context> pPreviousContext_;
  std::map<std::string, std::shared_ptr<Context>> contexts_;

  std::array<int, 2> pGlVersion_;

  DWORD update_window_size_current_context(bool isFullscreen, int width, int height);

  //Opengl context to render in
  HINSTANCE create_windows_application_instance();

  //Window, sets all the basic information for a window, such as the icon, cursor, type of redraw,
  //the instance of the owner application, etc
  //WindowIdentifier,holds the unique identifier for the window which we create
  //dwExStyle, just sets the type of window and that it has an edge.
  //Also quick error check to make sure the WindowClass variable can be registered:
  HWND create_windows_window(WNDPROC proc, std::string title, int width, int height, bool fullscreen, HINSTANCE applicationInstance);
  HDC create_windows_device_context(HWND windowId);

  //Connect our Window represented by theWindowIdentifier to a device (graphics card) and then 
  //get a rendering context from the device to render to
  //Currently creates an OpenGL modern context by default
  //Will default to OpenGL 2.1 capable context if the OpenGL modern context cannot be created. 
  HGLRC create_windows_opengl_rendering_context(HDC deviceContext);
  std::array<int, 2> opengl_version_info();
 
    
};


  }
}


#endif