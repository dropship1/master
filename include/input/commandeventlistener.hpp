#ifndef BRIGHT_INPUT_COMMAND_EVENT_LISTENER_H
#define BRIGHT_INPUT_COMMAND_EVENT_LISTENER_H

#include <memory>
#include "input/commandevent.hpp"

namespace bright{
  
    namespace input{

/**
 * @ingroup input
 *
 * @brief command event listener
 * 
 * @details 
 * Defines a command event listener interface
 * Example Usage:
 * @code
 * @endcode
 *
 */ 
class CommandEventListener{

public:

  virtual void on_command_event(std::shared_ptr<CommandEvent> commandInputEvent) = 0;

private:

};

  }
}

#endif