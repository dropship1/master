#ifndef BRIGHT_NETWORK_GLOBAL_STRUCTS_H
#define BRIGHT_NETWORK_GLOBAL_STRUCTS_H

#include <map>
#include <string>

namespace bright{
  
    namespace network{ 


/**
 * @ingroup network
 *
 * @brief network helper structs
 * 
 * @details 
 * Structs to help with the network handling
 * Example Usage:
 * @code
 * @endcode
 *
 */  

enum class MessageType {
  EMTPY,
  CLIENT_LOGIN_REQUEST,
  CLIENT_COMMAND,
  SERVER_LOGIN_RESPONSE,
  SERVER_UPDATE_RESPONSE,
};


static std::map<std::string, MessageType> create_message_type_decoder_map(){
  std::map<std::string, MessageType> m;
  m["empty"] = MessageType::EMTPY;
  m["client_login"] = MessageType::CLIENT_LOGIN_REQUEST;
  m["client_command"] = MessageType::CLIENT_COMMAND;
  m["svr_login_rsp"] = MessageType::SERVER_LOGIN_RESPONSE;
  m["svr_update_rsp"] = MessageType::SERVER_UPDATE_RESPONSE;
  return m;
}

static std::map<std::string, MessageType> MessageTypeDecoder = create_message_type_decoder_map();

  }
}

#endif
