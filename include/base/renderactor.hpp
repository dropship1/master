#ifndef BRIGHT_BASE_RENDER_ACTOR_H
#define BRIGHT_BASE_RENDER_ACTOR_H

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
 * @brief The render actor class
 * 
 * @details 
 * This is a simple representation of an render actor in the world.
 * An render actor can be anything, from a camera to a flower.
 * This class is more metadata than anything
 * Example Usage:
 * @code
 * @endcode
 *
 */
class RenderActor{

public:

/** 
 * @brief Construct a new default actor.
 * @details
 */
  RenderActor();

  std::string mesh();
  void mesh(std::string mesh);

  std::string shader();
  void shader(std::string shader);

  std::string render_name();
  void render_name(std::string name);

  std::string camera_type();
  void camera_type(std::string cameraType);


private:

  std::string mesh_;
  std::string shader_;
  std::string renderName_;
  std::string cameraType_;


};

  }
}

#endif