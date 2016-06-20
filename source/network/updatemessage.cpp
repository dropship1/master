#include "network/updatemessage.hpp"

using namespace bright::network;


//UpdateMessage::UpdateMessage(std::string name, glm::vec3 position, glm::vec3 right, glm::vec3 up, glm::vec3 look, bool isPlayer): 
//  name_(name), position_(position), isPlayer_(isPlayer), right_(right), up_(up), look_(look){
//}

UpdateMessage::UpdateMessage(std::string name, glm::vec3 position, glm::vec3 rotation, bool isPlayer): 
  name_(name), position_(position), isPlayer_(isPlayer), rotation_(rotation){
}

//UpdateMessage::UpdateMessage(std::string name, glm::vec3 position, bool isPlayer): 
//  name_(name), position_(position), isPlayer_(isPlayer),
//  right_(glm::vec3(1.0f,0.0f,0.0f)), up_(glm::vec3(0.0f,1.0f,0.0f)), look_(glm::vec3(0.0f,0.0f,1.0f)){
//}


UpdateMessage::UpdateMessage(): 
  name_(""), position_(glm::vec3(1.0f,1.0f,1.0f)), rotation_(glm::vec3(0.0f,0.0f,0.0f)), isPlayer_(false){
}


bool UpdateMessage::is_player(){
  return isPlayer_;
}


std::string& UpdateMessage::name(){
  return name_;
}


glm::vec3 UpdateMessage::position(){
  return position_;
}

glm::vec3 UpdateMessage::rotation(){
  return rotation_;
}

//glm::vec3 UpdateMessage::right(){
//  return right_;
//}
//
//glm::vec3 UpdateMessage::up(){
//  return up_;
//}
//
//glm::vec3 UpdateMessage::look(){
//  return look_;
//}

std::string UpdateMessage::full_message(){

  std::string isP = "false";
  if(isPlayer_){ isP = "true"; }

  std::ostringstream myString;
  myString << "svr_update_rsp ";
  myString << name_ << " ";
  myString << isP << " ";
  myString << position_.x << " ";
  myString << position_.y << " ";
  myString << position_.z << " ";
  myString << rotation_.x << " ";
  myString << rotation_.y << " ";
  myString << rotation_.z << "\n";

  return myString.str();
}