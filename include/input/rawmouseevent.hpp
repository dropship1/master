#ifndef BRIGHT_INPUT_RAW_MOUSE_EVENT_H
#define BRIGHT_INPUT_RAW_MOUSE_EVENT_H

#include "input/globalstructs.hpp"

namespace bright{
  
    namespace input{

/**
 * @ingroup input
 *
 * @brief raw mouse event
 * 
 * @details 
 * Defines a raw mouse event
 * Example Usage:
 * @code
 * @endcode
 *
 */ 
class RawMouseEvent{

public:
  RawMouseEvent(MouseEventType rawMouseEventType, MouseButton rawMouseButton, bool isDown, int deltaRateX, int deltaRateY, short int deltaWheel);

  MouseEventType raw_mouse_event_type();
  
  MouseButton raw_mouse_button();
  bool is_raw_mouse_button_down();
  
  int delta_rate_x();
  int delta_rate_y();
  short int delta_rate_wheel();

private:
  MouseEventType rawMouseEventType_;
  MouseButton rawMouseButton_;
  bool isRawMouseButtonDown_;
  int deltaRateX_;
  int deltaRateY_;
  short int deltaRateWheel_;

};

  }
}

#endif