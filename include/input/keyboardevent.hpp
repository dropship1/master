#ifndef BRIGHT_INPUT_KEYBOARD_EVENT_H
#define BRIGHT_INPUT_KEYBOARD_EVENT_H

#include <string>
#include "input/globalstructs.hpp"

namespace bright{
  
    namespace input{

/**
 * @ingroup input
 *
 * @brief keyboard event
 * 
 * @details 
 * Defines a keyboard event
 * Example Usage:
 * @code
 * @endcode
 *
 */ 
class KeyboardEvent{

public:
  KeyboardEvent(bool down, std::string keyString, Key keyEnum);

  bool is_down();
  std::string key_string();
  Key key_enum();

private:
  bool isDown_;
  std::string keyString_;
  Key keyEnum_;

};

  }
}

#endif