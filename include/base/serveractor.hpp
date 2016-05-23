#ifndef BRIGHT_BASE_SERVER_ACTOR_H
#define BRIGHT_BASE_SERVER_ACTOR_H

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <glm/glm.hpp>
#include "physics/aabb.hpp"
#include "base/servercontroller.hpp"

namespace bright{

  namespace base{

/**
 * @ingroup base
 *
 * @brief The server actor class
 * 
 * @details 
 * This is a simple representation of server actor in the simulated world.
 * Example Usage:
 * @code
 * @endcode
 *
 */
class ServerActor{

public:

/** 
 * @brief Construct a new default server actor.
 * @details
 */
  ServerActor();

  std::string name();
  void name(std::string name);

  std::shared_ptr<bright::physics::AABB> aabb();
  void aabb(std::shared_ptr<bright::physics::AABB> aabb);

  bool is_player();
  void is_player(bool value);

  bool is_logged_in();
  void is_logged_in(bool value);

  void point_at(glm::vec3 pointA, glm::vec3 pointB);
  glm::vec3 pos();
  void pos(glm::vec3 newPos);

  void set_to_prev_pos();

  glm::vec3 rotation();
  void rotation(glm::vec3 rot);

  void move_fwd(float amount);
  void move_backward(float amount);
  void move_left(float amount);
  void move_right(float amount);

  void rotate_right(float amount);
  void rotate_left(float amount);
  void rotate_up(float amount);
  void rotate_down(float amount);

  glm::vec3 right();
  glm::vec3 up();
  glm::vec3 look();

  bool have_update();
  void have_update(bool value);

private:

  bright::base::ServerController serverController_;

  std::string name_;
  bool isPlayer_;
  bool isLoggedIn_;
  std::shared_ptr<bright::physics::AABB> aabb_;

  glm::vec3 up_;
  glm::vec3 look_;
  glm::vec3 right_;

  bool haveUpdate_;


};

  }
}

#endif