#include "base/actor.hpp"

using namespace bright::base;

Actor::Actor(){
}


glm::vec3 Actor::pos(){
  return pos_;
}


void Actor::pos(glm::vec3 pos){
  pos_ = pos;
}


glm::vec3 Actor::rotation(){
  return rotation_;
}


void Actor::rotation(glm::vec3 rotation){
  rotation_ = rotation;
}

std::string Actor::mesh(){
  return mesh_;
}


void Actor::mesh(std::string mesh){
  mesh_ = mesh;
}

std::string Actor::shader(){
  return shader_;
}


void Actor::shader(std::string shader){
  shader_ = shader;
}


std::string Actor::name(){
  return name_;
}


void Actor::name(std::string name){
  name_ = name;
}


bool Actor::is_player(){
  return isPlayer_;
}


void Actor::is_player(bool value){
  isPlayer_ = value;
}


std::string Actor::camera_type(){
  return cameraType_;
}


void Actor::camera_type(std::string cameraType){
  cameraType_ = cameraType;
}