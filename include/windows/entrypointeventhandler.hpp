#ifndef BRIGHT_WINDOWS_ENTRYPOINTEVENTHANDLER_H
#define BRIGHT_WINDOWS_ENTRYPOINTEVENTHANDLER_H

#include <windows.h>
#include <string>
#include <memory>

#include "input/inputmanager.hpp"
#include "context/contextmanager.hpp"

LRESULT CALLBACK  WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);

enum WinKeys {
  RIGHT_CONTROL = VK_RCONTROL,
  LEFT_CONTROL = VK_LCONTROL,
  RIGHT_ALT = VK_RMENU,
  LEFT_ALT = VK_LMENU,
  NUMPAD_ENTER = VK_SEPARATOR,
  NUMPAD_DECIMAL = VK_DECIMAL,
  NUMPAD_0 = VK_NUMPAD0,
  NUMPAD_1 = VK_NUMPAD1,
  NUMPAD_2 = VK_NUMPAD2,
  NUMPAD_3 = VK_NUMPAD3,
  NUMPAD_4 = VK_NUMPAD4,
  NUMPAD_5 = VK_NUMPAD5,
  NUMPAD_6 = VK_NUMPAD6,
  NUMPAD_7 = VK_NUMPAD7,
  NUMPAD_8 = VK_NUMPAD8,
  NUMPAD_9 = VK_NUMPAD9
};

namespace bright{

  namespace windows{


/**
 * @ingroup windows
 *
 * @brief The entry point event handler class
 * 
 * @details 
 * This represents a entry point for input and windows events for windows OS
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class EntryPointEventHandler{

public:

/** 
 * @brief Construct a new default entry point event handler.
 * @details
 */
  EntryPointEventHandler();

/** 
 * @brief Update the context's window width and window height.
 * @param width The width to use.
 * @param height The height to use.
 */
  LRESULT handle_event(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);

  
  /**
  * @brief Return the singleton instance of the entry point event handler
  */
  static std::shared_ptr<EntryPointEventHandler> get_entry_point_event_handler();

  std::shared_ptr<bright::context::ContextManager> get_context_manager();
  std::shared_ptr<bright::input::InputManager> get_input_manager();



private: 

  std::shared_ptr<bright::context::ContextManager> pContextManager_;
  std::shared_ptr<bright::input::InputManager> pInputManager_;

  float lastX_ = FLT_MAX;
  float lastY_ = FLT_MAX;
  float lastZ_ = FLT_MAX;

  static std::shared_ptr<EntryPointEventHandler> pEntryPointEventHandler_;
    
};


  }
}

#endif