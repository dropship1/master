#ifndef BRIGHT_BASE_CONTROL_ACTOR_H
#define BRIGHT_BASE_CONTROL_ACTOR_H

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
 * @brief The control actor class
 * 
 * @details 
 * This is a simple representation of a control actor in the simulated world.
 * Example Usage:
 * @code
 * @endcode
 *
 */
class ControlActor{

public:

/** 
 * @brief Construct a new default control actor.
 * @details
 */
  ControlActor();

  std::string control_name();
  void control_name(std::string name);

  std::string render_name();
  void render_name(std::string name);

  std::string aabb();
  void aabb(std::string name);

  bool is_player();
  void is_player(bool value);

  bool is_logged_in();
  void is_logged_in(bool value);

  bool have_update();
  void have_update(bool value);

  glm::vec3 pos();
  void pos(glm::vec3 pos);

  glm::vec3 rotation();
  void rotation(glm::vec3 rotation);

  ControlActor clone();

private:

  std::string controlName_;
  std::string renderName_;
  std::string aabb_;

  bool isPlayer_;
  bool isLoggedIn_;
  bool haveUpdate_;

  glm::vec3 pos_;
  glm::vec3 rotation_;

};

  }
}

#endif