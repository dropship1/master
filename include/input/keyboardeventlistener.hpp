#ifndef BRIGHT_INPUT_KEYBOARD_EVENT_LISTENER_H
#define BRIGHT_INPUT_KEYBOARD_EVENT_LISTENER_H

#include <memory>
#include "input/keyboardevent.hpp"

namespace bright{
  
    namespace input{

/**
 * @ingroup input
 *
 * @brief keyboard event listener
 * 
 * @details 
 * Defines a keyboard event listener interface
 * Example Usage:
 * @code
 * @endcode
 *
 */ 
class KeyboardEventListener{

public:

  virtual void on_keyboard_event(std::shared_ptr<KeyboardEvent> keyboardInputEvent) = 0;

private:

};

  }
}

#endif