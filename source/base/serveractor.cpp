#include "base/serveractor.hpp"

using namespace bright::base;

ServerActor::ServerActor(): serverController_(), isLoggedIn_(false), isPlayer_(false), name_(""), haveUpdate_(false){
}


std::shared_ptr<bright::physics::AABB> ServerActor::aabb(){
  return aabb_;
}


void ServerActor::aabb(std::shared_ptr<bright::physics::AABB> aabb){
  aabb_ = aabb;
}


bool ServerActor::is_player(){
  return isPlayer_;
}


void ServerActor::is_player(bool value){
  isPlayer_ = value;
}

bool ServerActor::is_logged_in(){
  return isLoggedIn_;
}


void ServerActor::is_logged_in(bool value){
  isLoggedIn_ = value;
}


void ServerActor::name(std::string name){
  name_ = name;
}


std::string ServerActor::name(){
  return name_;
}


void ServerActor::point_at(glm::vec3 pointA, glm::vec3 pointB){
  serverController_.update_axes(pointA, pointB);
}

glm::vec3 ServerActor::pos(){
  return serverController_.pos();
}

void ServerActor::pos(glm::vec3 newPos){
  serverController_.update_pos(newPos);
  if(aabb_){ aabb_->move(newPos); }
}

void ServerActor::set_to_prev_pos(){
  serverController_.set_to_prev_pos();
  if(aabb_){ aabb_->move( serverController_.pos() ); }
}


void ServerActor::move_fwd(float amount){
  serverController_.move_fwd(amount);
  if(aabb_){ aabb_->move( serverController_.pos() ); }
} 


void ServerActor::move_backward(float amount){
  serverController_.move_backward(amount);
  if(aabb_){ aabb_->move( serverController_.pos() ); }
} 


void ServerActor::move_left(float amount){
  serverController_.move_left(amount);
  if(aabb_){ aabb_->move( serverController_.pos() ); }
} 


void ServerActor::move_right(float amount){
  serverController_.move_right(amount);
  if(aabb_){ aabb_->move( serverController_.pos() ); }
} 


void ServerActor::rotate_right(float amount){
  serverController_.rotate_right(amount);
} 


void ServerActor::rotate_left(float amount){
  serverController_.rotate_left(amount);
} 


void ServerActor::rotate_up(float amount){
  serverController_.rotate_up(amount);
} 


void ServerActor::rotate_down(float amount){
  serverController_.rotate_down(amount);
} 


glm::vec3 ServerActor::right(){
  return serverController_.right();
}


glm::vec3 ServerActor::up(){
  return serverController_.up();
}


glm::vec3 ServerActor::look(){
  return serverController_.look();
}

glm::vec3 ServerActor::rotation(){
  return serverController_.rotation();
}


 void ServerActor::rotation(glm::vec3 rot){
  serverController_.update_rot(rot);
}


bool ServerActor::have_update(){
  return haveUpdate_;
}


void ServerActor::have_update(bool value){
  haveUpdate_ = value;
}

ServerActor ServerActor::clone(){

  ServerActor cloned;
  cloned.pos( pos() );
  cloned.rotation( rotation() );
  auto newAabb = std::make_shared<bright::physics::AABB>();
  newAabb->copy_this( aabb() );
  cloned.aabb(newAabb);
  return cloned;
}