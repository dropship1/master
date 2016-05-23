#ifndef BRIGHT_BASE_ACTOR_H
#define BRIGHT_BASE_ACTOR_H

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <glm/glm.hpp>

namespace bright{

  namespace base{

/**
 * @ingroup base
 *
 * @brief The actor class
 * 
 * @details 
 * This is a simple representation of an actor in the world.
 * An actor can be anything, from a camera to a flower.
 * This class is more metadata than anything
 * Example Usage:
 * @code
 * @endcode
 *
 */
class Actor{

public:

/** 
 * @brief Construct a new default actor.
 * @details
 */
  Actor();

  glm::vec3 pos();
  void pos(glm::vec3 pos);

  glm::vec3 rotation();
  void rotation(glm::vec3 rotation);

  std::string mesh();
  void mesh(std::string mesh);

  std::string shader();
  void shader(std::string shader);

  std::string name();
  void name(std::string name);

  std::string camera_type();
  void camera_type(std::string cameraType);

  bool is_player();
  void is_player(bool value);

private:

  glm::vec3 pos_;
  glm::vec3 rotation_;
  std::string mesh_;
  std::string shader_;
  std::string name_;
  std::string cameraType_;
  bool isPlayer_;


};

  }
}

#endif