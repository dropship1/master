#ifndef BRIGHT_INPUT_RAW_MOUSE_EVENT_LISTENER_H
#define BRIGHT_INPUT_RAW_MOUSE_EVENT_LISTENER_H

#include <memory>
#include "input/rawmouseevent.hpp"

namespace bright{
  
    namespace input{

/**
 * @ingroup input
 *
 * @brief raw mouse event listener
 * 
 * @details
 * Defines a raw mouse event listener interface
 * Example Usage:
 * @code
 * @endcode
 *
 */ 
class RawMouseEventListener{

public:
  virtual void on_raw_mouse_event(std::shared_ptr<RawMouseEvent> rawMouseInputEvent) = 0;

private:

};

  }
}

#endif