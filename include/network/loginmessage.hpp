#ifndef BRIGHT_NETWORK_LOGIN_MESSAGE_H
#define BRIGHT_NETWORK_LOGIN_MESSAGE_H

#include <string>
#include "network/globalstructs.hpp"

namespace bright{
  
    namespace network{

/**
 * @ingroup network
 *
 * @brief login message
 * 
 * @details 
 * Defines a login message
 * Example Usage:
 * @code
 * @endcode
 *
 */ 
class LoginMessage{

public:
  LoginMessage();
  LoginMessage(std::string username, std::string password);

  const std::string& username();
  const std::string& password();
  std::string request_message();

private:
  std::string username_;
  std::string password_;

};

  }
}

#endif