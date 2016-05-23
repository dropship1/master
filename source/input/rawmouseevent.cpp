#include "input/rawmouseevent.hpp"

using namespace bright::input;


RawMouseEvent::RawMouseEvent(MouseEventType rawMouseEventType, MouseButton rawMouseButton, bool isDown, int deltaRateX, int deltaRateY, short int deltaRateWheel): 
  rawMouseEventType_(rawMouseEventType), rawMouseButton_(rawMouseButton), isRawMouseButtonDown_(isDown), deltaRateX_(deltaRateX), deltaRateY_(deltaRateY), deltaRateWheel_(deltaRateWheel){
}


 
MouseEventType RawMouseEvent::raw_mouse_event_type(){
  return rawMouseEventType_;
}

 
MouseButton RawMouseEvent::raw_mouse_button(){
  return rawMouseButton_;
}

bool RawMouseEvent::is_raw_mouse_button_down(){
  return isRawMouseButtonDown_;
}
 
int RawMouseEvent::delta_rate_x(){
  return deltaRateX_;
}


int RawMouseEvent::delta_rate_y(){
  return deltaRateY_;
}


short int RawMouseEvent::delta_rate_wheel(){
  return deltaRateWheel_;
}

