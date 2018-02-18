#ifndef BRIGHT_BASE_ACTOR_RENDER_CONTROLLER_H
#define BRIGHT_BASE_ACTOR_RENDER_CONTROLLER_H

#include <iostream>
#include <fstream>
#include <vector>

#include "glm/glm.hpp"
#include "glm/mat4x4.hpp" // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_inverse.hpp>


namespace bright{

  namespace base{

class ActorRenderController{

public:

  ActorRenderController();

  void update(glm::vec3 pos, glm::vec3 right, glm::vec3 up, glm::vec3 look);
  void update(glm::vec3 pos);

  glm::vec3 pos();
  glm::vec3 prev_pos();

  glm::mat4 model_to_world_transformation_matrix();
  glm::mat4 world_to_camera_transformation_matrix();

  std::string name();
  void name(std::string name);

private:

  void calculate_model_to_world();
  glm::mat4 modToWorldMat_;
  glm::mat4 worldToCamera_;

  glm::vec3 pos_;
  glm::vec3 prevPos_;
  glm::vec3 rotation_;

  glm::vec3 up_;
  glm::vec3 look_;
  glm::vec3 right_;

  std::string name_;

};

  }
}

#endif