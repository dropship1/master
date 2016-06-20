#include "base/actorcontrolcontroller.hpp"

using namespace bright::base;


ActorControlController::ActorControlController(){

  prevPos_ = glm::vec3(0.0f, 0.0f, 0.0f);
  pos_ = glm::vec3(0.0f, 0.0f, 0.0f);
  rotation_ = glm::vec3(0.0f, 0.0f, 0.0f);


  up_ = glm::vec3(0.0f,1.0f,0.0f);
  look_ = glm::vec3(0.0f,0.0f,1.0f);
  right_ = glm::vec3(1.0f,0.0f,0.0f);

  hasChangedThisFrame_ = false;

}


void ActorControlController::calculate_new_direction_vectors(){

  //http://www.toymaker.info/Games/html/camera.html
  //www.3dgep.com/understanding-the-view-matrix/
  up_ = glm::vec3(0.0f,1.0f,0.0f);
  look_ = glm::vec3(0.0f,0.0f,1.0f);
  right_ = glm::vec3(1.0f,0.0f,0.0f);

  //Yaw
  glm::mat4 yawRotationMatrix4 = glm::mat4(1.0f);
  yawRotationMatrix4 = glm::rotate(yawRotationMatrix4, rotation_.y, up_); 
  glm::mat3x3 yawMatrix3 = glm::mat3x3(yawRotationMatrix4);
  look_ = yawMatrix3 * look_;
  right_ = yawMatrix3 * right_;

  //Pitch
  glm::mat4 pitchRotationMatrix4 = glm::mat4(1.0f);
  pitchRotationMatrix4 = glm::rotate(pitchRotationMatrix4, rotation_.x, right_); 
  glm::mat3x3 pitchMatrix3 = glm::mat3x3(pitchRotationMatrix4);
  look_ = pitchMatrix3 * look_;
  up_ = pitchMatrix3 * up_;

  //Roll
  glm::mat4 rollRotationMatrix4 = glm::mat4(1.0f);
  rollRotationMatrix4 = glm::rotate(rollRotationMatrix4, rotation_.z, look_); 
  glm::mat3x3 rollMatrix3 = glm::mat3x3(rollRotationMatrix4);
  right_ = rollMatrix3 * right_;
  up_ = rollMatrix3 * up_;

}


void ActorControlController::update(glm::vec3 pos, glm::vec3 rotation){
  prevPos_ = pos_;
  pos_ = pos;
  rotation_ = rotation;
}


void ActorControlController::set_to_prev_pos(){
  pos_ = prevPos_;
}

void ActorControlController::pos(glm::vec3 pos){
  prevPos_ = pos_;
  pos_ = pos;
}

void ActorControlController::rotation(glm::vec3 rot){
  rotation_ = rot;
  calculate_new_direction_vectors();
}


void ActorControlController::update_axes(glm::vec3 eye, glm::vec3 center){
  glm::mat4 viewMatrix = glm::lookAt(eye, center, glm::vec3(0.0f,1.0f,0.0f));
  viewMatrix = glm::transpose(viewMatrix);
  right_ = glm::vec3(viewMatrix[0].x, viewMatrix[0].y, viewMatrix[0].z);
  up_ = glm::vec3(viewMatrix[1].x, viewMatrix[1].y, viewMatrix[1].z);
  look_ = glm::vec3(viewMatrix[2].x, viewMatrix[2].y, viewMatrix[2].z);
}


glm::vec3 ActorControlController::pos(){
  return pos_;
}


glm::vec3 ActorControlController::prev_pos(){
  return prevPos_;
}


glm::vec3 ActorControlController::rotation(){
  return rotation_;
}

glm::vec3 ActorControlController::right(){
  return right_;
}

glm::vec3 ActorControlController::up(){
  return up_;
}

glm::vec3 ActorControlController::look(){
  return look_;
}


bool ActorControlController::has_changed_this_frame(){
  return hasChangedThisFrame_;
}


void ActorControlController::has_changed_this_frame(bool value){
  hasChangedThisFrame_ = value;
}


void ActorControlController::rotate_left(float amount){
  rotate_y(amount);
  calculate_new_direction_vectors();
}

void ActorControlController::rotate_right(float amount){
  rotate_y(-amount);
  calculate_new_direction_vectors();
}

void ActorControlController::rotate_up(float amount){
  rotate_x(amount);
  calculate_new_direction_vectors();
}

void ActorControlController::rotate_down(float amount){
  rotate_x(-amount);
  calculate_new_direction_vectors();
}

void ActorControlController::rotate_roll_left(float amount){
  rotate_z(amount);
  calculate_new_direction_vectors();
}

void ActorControlController::rotate_roll_right(float amount){
  rotate_z(-amount);
  calculate_new_direction_vectors();
}


void ActorControlController::rotate_x(float amount){

  float rotateX = amount;
  float testAmount = rotation_.x + rotateX;
  if (testAmount > 360.0f){
    float amountPastZero = testAmount - 360.0f;
    rotation_.x = 0.0f;
    rotation_.x += amountPastZero;
  }
  else if (testAmount < -360.0f){
    float amountBelowZero = testAmount + 360.0f;
    rotation_.x = 0.0f;
    rotation_.x += amountBelowZero;
  }
  else{
    rotation_.x += rotateX;
  }
}

  

void ActorControlController::rotate_y(float amount){

  float rotateY = amount;
  float testAmount = rotation_.y + rotateY;
  if (testAmount > 360.0f){
    float amountPastZero = testAmount - 360.0f;
    rotation_.y = 0.0f;
    rotation_.y += amountPastZero;
  }
  else if (testAmount < -360.0f){
    float amountBelowZero = testAmount + 360.0f;
    rotation_.y = 0.0f;
    rotation_.y += amountBelowZero;
  }
  else{
    rotation_.y += rotateY;
  }
}



void ActorControlController::rotate_z(float amount){

  float rotateZ = amount;
  float testAmount = rotation_.z + rotateZ;
  if (testAmount > 360.0f){
    float amountPastZero = testAmount - 360.0f;
    rotation_.z = 0.0f;
    rotation_.z += amountPastZero;
  }
  else if (testAmount < -360.0f){
    float amountBelowZero = testAmount + 360.0f;
    rotation_.z = 0.0f;
    rotation_.z += amountBelowZero;
  }
  else{
    rotation_.z += rotateZ;
  }
}


void ActorControlController::move_fwd(float amount){
  prevPos_ = pos_;

  pos_ += look_ * -amount;
}


void ActorControlController::move_backward(float amount){
  prevPos_ = pos_;

  pos_ += look_ * amount;
}


void ActorControlController::move_right(float amount){
  prevPos_ = pos_;

  pos_ += right_ * amount;
}


void ActorControlController::move_left(float amount){
  prevPos_ = pos_;

  pos_ += right_ * -amount;
}



void ActorControlController::move_up(float amount){
  prevPos_ = pos_;

  pos_ += up_ * amount;
}


void ActorControlController::move_down(float amount){
  prevPos_ = pos_;

  pos_ += up_ * -amount;
}

