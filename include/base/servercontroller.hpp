#ifndef BRIGHT_BASE_SERVER_CONTROLLER_H
#define BRIGHT_BASE_SERVER_CONTROLLER_H

#include <iostream>
#include <fstream>
#include <vector>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


namespace bright{

  namespace base{

class ServerController{

public:

  ServerController();

  void update(glm::vec3 pos, glm::vec3 rotation);
  void update_pos(glm::vec3 pos);
  void update_rot(glm::vec3 rot);
  void update_axes(glm::vec3 eye, glm::vec3 center);

  void set_to_prev_pos();

  void move_fwd(float amount);
  void move_backward(float amount);
  void move_left(float amount);
  void move_right(float amount);
  void move_up(float amount);
  void move_down(float amount);

  void rotate_left(float amount);
  void rotate_right(float amount);
  void rotate_up(float amount);
  void rotate_down(float amount);
  void rotate_roll_left(float amount);
  void rotate_roll_right(float amount);

  glm::vec3 pos();
  glm::vec3 prev_pos();
  glm::vec3 rotation();
  glm::vec3 right();
  glm::vec3 up();
  glm::vec3 look();

private:

  void rotate_x(float amount);
  void rotate_y(float amount);
  void rotate_z(float amount);

  void calculate_new_direction_vectors();

  glm::vec3 pos_;
  glm::vec3 prevPos_;
  glm::vec3 rotation_;

  glm::vec3 up_;
  glm::vec3 look_;
  glm::vec3 right_;

};

  }
}

#endif