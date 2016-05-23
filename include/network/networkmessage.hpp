#ifndef BRIGHT_NETWORK_NETWORK_MESSAGE_H
#define BRIGHT_NETWORK_NETWORK_MESSAGE_H

#include <string>
#include "network/globalstructs.hpp"

namespace bright{
  
    namespace network{

/**
 * @ingroup network
 *
 * @brief network message
 * 
 * @details 
 * Defines a network message
 * Example Usage:
 * @code
 * @endcode
 *
 */ 
class NetworkMessage{

public:
  NetworkMessage(std::string data, MessageType type);

  std::string data();
  MessageType type();

private:
  std::string data_;
  MessageType type_;

};

  }
}

#endif