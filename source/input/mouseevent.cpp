#include "input/mouseevent.hpp"

using namespace bright::input;


MouseEvent::MouseEvent(MouseEventType mouseEventType, MouseButton mouseButton, float xpos, float ypos): 
  mouseEventType_(mouseEventType), mouseButton_(mouseButton), xPos_(xpos), yPos_(ypos){
}

 
MouseEventType MouseEvent::mouse_event_type(){
  return mouseEventType_;
}


MouseButton MouseEvent::mouse_button(){
  return mouseButton_;
}


float MouseEvent::x_pos(){
  return xPos_;
}


float MouseEvent::y_pos(){
  return yPos_;
}