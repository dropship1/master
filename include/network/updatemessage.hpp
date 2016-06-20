#ifndef BRIGHT_NETWORK_UPDATE_MESSAGE_H
#define BRIGHT_NETWORK_UPDATE_MESSAGE_H

#include <string>
#include <sstream>
#include "network/globalstructs.hpp"

#include <glm/glm.hpp>

namespace bright{
  
    namespace network{

/**
 * @ingroup network
 *
 * @brief update message
 * 
 * @details 
 * Defines a update message
 * Example Usage:
 * @code
 * @endcode
 *
 */ 
class UpdateMessage{

public:
  UpdateMessage();
  //UpdateMessage(std::string name, glm::vec3 position, bool isPlayer);
  UpdateMessage(std::string name, glm::vec3 position, glm::vec3 rotation, bool isPlayer);
  //UpdateMessage(std::string name, glm::vec3 position, glm::vec3 right, glm::vec3 up, glm::vec3 look, bool isPlayer);

  std::string& name();
  bool is_player();

  glm::vec3 position();
  glm::vec3 rotation();
  //glm::vec3 right();
  //glm::vec3 up();
  //glm::vec3 look();

  std::string full_message();

private:

  std::string name_;
  glm::vec3 position_;
  glm::vec3 rotation_;
  //glm::vec3 right_;
  //glm::vec3 up_;
  //glm::vec3 look_;
  bool isPlayer_;

};

  }
}

#endif