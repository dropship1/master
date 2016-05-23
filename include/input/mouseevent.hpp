#ifndef BRIGHT_INPUT_MOUSE_EVENT_H
#define BRIGHT_INPUT_MOUSE_EVENT_H

#include "input/globalstructs.hpp"

namespace bright{
  
    namespace input{


/**
 * @ingroup input
 *
 * @brief mouse event
 * 
 * @details 
 * Defines a mouse event
 * Example Usage:
 * @code
 * @endcode
 *
 */ 
class MouseEvent{

public:

  MouseEvent(MouseEventType mouseEventType, MouseButton mouseButton, float xPos, float yPos);

  MouseEventType mouse_event_type();

  MouseButton mouse_button();

  float x_pos();
  float y_pos();

private:
  MouseButton mouseButton_;
  MouseEventType mouseEventType_;
  float xPos_;
  float yPos_;

};

  }
}

#endif