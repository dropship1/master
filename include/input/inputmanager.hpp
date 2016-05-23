#ifndef BRIGHT_INPUT_INPUT_MANAGER_H
#define BRIGHT_INPUT_INPUT_MANAGER_H

#include <iostream>

#include "globalstructs.hpp"

#include "input/keyboardevent.hpp"
#include "input/mouseevent.hpp"
#include "input/rawmouseevent.hpp"

#include "input/keyboardeventlistener.hpp"
#include "input/mouseeventlistener.hpp"
#include "input/rawmouseeventlistener.hpp"

#include <algorithm>
#include <vector>

namespace bright{
  
    namespace input{

/**
 * @ingroup input
 *
 * @brief The input manager class
 * 
 * @details 
 * This manages all input using the appropriate handlers
 * Currently supports keyboard/mouse/rawmouse
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class InputManager{

public:
  InputManager();

  void notify();

  void add_keyboard_event_listener(std::shared_ptr<KeyboardEventListener> pKeyboardEventListener);
  void add_mouse_event_listener(std::shared_ptr<MouseEventListener> pMouseEventListener);
  void add_raw_mouse_event_listener(std::shared_ptr<RawMouseEventListener> pRawMouseEventListener);

  void update_mouse_listeners_position(float newXPos, float newYPos);
       
  void update_raw_mouse_listeners_button(bool isDown, MouseButton button, short int data);
  void update_raw_mouse_listeners_deltas(int xPos, int yPos);
       
  void update_keyboard_listeners(bool isDown, std::string asciiValue);


private:

  void initialize_keys_map();

  void notify_mouse_listeners();
  void notify_raw_mouse_listeners();
  void notify_keyboard_listeners();

  std::vector<std::shared_ptr<KeyboardEventListener>> pKeyboardEventListeners_;
  std::vector<std::shared_ptr<RawMouseEventListener>> pRawMouseEventListeners_;
  std::vector<std::shared_ptr<MouseEventListener>> pMouseEventListeners_;

  std::vector<std::shared_ptr<MouseEvent>> mouseEvents_;
  std::vector<std::shared_ptr<KeyboardEvent>> keyboardEvents_;
  std::vector<std::shared_ptr<RawMouseEvent>> rawMouseEvents_;

  std::map<Key, bool> keyboard_;
  std::map<MouseButton, bool> rawMouseButtons_;

  std::map<std::string,Key> KEYS_MAP_;

};

  }
}

#endif