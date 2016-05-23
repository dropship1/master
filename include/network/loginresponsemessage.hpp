#ifndef BRIGHT_NETWORK_LOGIN_RESPONSE_MESSAGE_H
#define BRIGHT_NETWORK_LOGIN_RESPONSE_MESSAGE_H

#include <string>
#include "network/globalstructs.hpp"

namespace bright{
  
    namespace network{

/**
 * @ingroup network
 *
 * @brief login response message
 * 
 * @details 
 * Defines a login response message
 * Example Usage:
 * @code
 * @endcode
 *
 */ 
class LoginResponseMessage{

public:
  LoginResponseMessage();
  LoginResponseMessage(bool loggedIn, std::string message);

  bool logged_in();
  std::string& message();
  const std::string& full_response();

private:
  bool loggedIn_;
  std::string message_;
  std::string fullResponse_;

};

  }
}

#endif