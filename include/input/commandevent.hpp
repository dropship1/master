#ifndef BRIGHT_INPUT_COMMAND_EVENT_H
#define BRIGHT_INPUT_COMMAND_EVENT_H

#include <string>
#include <vector>
#include <memory>
#include "input/globalstructs.hpp"

namespace bright{
  
    namespace input{

/**
 * @ingroup input
 *
 * @brief command event
 * 
 * @details 
 * Defines a command event
 * Example Usage:
 * @code
 * @endcode
 *
 */ 
class CommandEvent{

public:
  CommandEvent(std::shared_ptr<std::vector<std::string>> command);
  std::string command_string();

private:
  std::shared_ptr<std::vector<std::string>> command_;
  std::string commandString_;

};

  }
}

#endif