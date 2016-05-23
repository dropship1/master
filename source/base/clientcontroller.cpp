#include "base/clientcontroller.hpp"

using namespace bright::base;


ClientController::ClientController(){

  prevPos_ = glm::vec3(0.0f, 0.0f, 0.0f);
  pos_ = glm::vec3(0.0f, 0.0f, 0.0f);
  rotation_ = glm::vec3(0.0f, 0.0f, 0.0f);


  up_ = glm::vec3(0.0f,1.0f,0.0f);
  look_ = glm::vec3(0.0f,0.0f,1.0f);
  right_ = glm::vec3(1.0f,0.0f,0.0f);

}


void ClientController::update(glm::vec3 pos, glm::vec3 right, glm::vec3 up, glm::vec3 look){
  prevPos_ = pos_;
  pos_ = pos;
  right_ = right;
  up_ = up;
  look_ = look;
}

void ClientController::update(glm::vec3 pos){
  prevPos_ = pos_;
  pos_ = pos;
}


glm::vec3 ClientController::pos(){
  return pos_;
}


glm::vec3 ClientController::prev_pos(){
  return prevPos_;
}


void ClientController::calculate_model_to_world(){

  glm::mat4 finalMatrix = glm::mat4(1.0f);

  ////Right Axis
  finalMatrix[0].x = right_.x;
  finalMatrix[0].y = right_.y;
  finalMatrix[0].z = right_.z;
  finalMatrix[0].w = 0.0f;

  //Up Axis
  finalMatrix[1].x = up_.x;
  finalMatrix[1].y = up_.y;
  finalMatrix[1].z = up_.z;
  finalMatrix[1].w = 0.0f;

  //Look Axis
  finalMatrix[2].x = look_.x;
  finalMatrix[2].y = look_.y;
  finalMatrix[2].z = look_.z;
  finalMatrix[2].w = 0.0f;

  //Pos
  finalMatrix[3].x = pos_.x;
  finalMatrix[3].y = pos_.y;
  finalMatrix[3].z = pos_.z;
  finalMatrix[3].w = 1.0f;

  modToWorldMat_ = finalMatrix;
  worldToCamera_ = glm::inverse(modToWorldMat_);
}


glm::mat4 ClientController::model_to_world_transformation_matrix(){
  calculate_model_to_world();
  return modToWorldMat_;
}

glm::mat4 ClientController::world_to_camera_transformation_matrix(){
  calculate_model_to_world();
  return worldToCamera_;
}
