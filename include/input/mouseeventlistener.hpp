#ifndef BRIGHT_INPUT_MOUSE_EVENT_LISTENER_H
#define BRIGHT_INPUT_MOUSE_EVENT_LISTENER_H

#include <memory>
#include "input/mouseevent.hpp"

namespace bright{
  
    namespace input{

/**
 * @ingroup input
 *
 * @brief mouse event listener
 * 
 * @details 
 * Defines a mouse event listener interface
 * Example Usage:
 * @code
 * @endcode
 *
 */
class MouseEventListener{

public:
  virtual void on_mouse_event(std::shared_ptr<MouseEvent> mouseInputEvent) = 0;

private:

};

  }
}

#endif