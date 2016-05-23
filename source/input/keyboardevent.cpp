#include "input/keyboardevent.hpp"

using namespace bright::input;


KeyboardEvent::KeyboardEvent(bool isDown, std::string keyString, Key keyEnum): 
  isDown_(isDown), keyString_(keyString), keyEnum_(keyEnum){
}


bool KeyboardEvent::is_down(){
  return isDown_;
}


std::string KeyboardEvent::key_string(){
  return keyString_;
}

Key KeyboardEvent::key_enum(){
  return keyEnum_;
}