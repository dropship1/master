#include "base/actorcreator.hpp"

using namespace bright::base;


ActorCreator::ActorCreator(){
}


void ActorCreator::add_bullet(bright::base::ActorControlController& currentActorController){
  bullets_.push_back(currentActorController);
}


std::vector<bright::base::ActorControlController>& ActorCreator::get_bullets(){
  return bullets_;
}





