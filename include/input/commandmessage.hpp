#ifndef BRIGHT_NETWORK_COMMAND_MESSAGE_H
#define BRIGHT_NETWORK_COMMAND_MESSAGE_H

#include <string>
#include "network/globalstructs.hpp"

namespace bright{
  
    namespace network{

/**
 * @ingroup network
 *
 * @brief command message
 * 
 * @details 
 * Defines a command message
 * Example Usage:
 * @code
 * @endcode
 *
 */ 
class CommandMessage{

public:
  CommandMessage();
  CommandMessage(std::string controlName, std::string commandName, std::string controlType, std::string data);

  const std::string& control_name();
  const std::string& command_name();
  const std::string& control_type();
  const std::string& data();  

  std::string message();


private:

  std::string controlName_;
  std::string commandName_;
  std::string controlType_;
  std::string data_;

  std::string message_;

};

  }
}

#endif