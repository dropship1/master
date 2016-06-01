#include "base/actorcreator.hpp"

using namespace bright::base;


ActorCreator::ActorCreator(std::shared_ptr<bright::utils::FileWorker> pFileWorker): 
  pFileWorker_(pFileWorker){
}

ActorCreator::ActorCreator(){
}




void ActorCreator::add_bullet(std::shared_ptr<bright::base::ServerActor> pClientActor){

  auto pBullet = std::make_shared<bright::base::ServerActor>( pClientActor->clone() );
  bullets_.push_back(pBullet);

}



std::vector<std::shared_ptr<bright::base::ServerActor>>& ActorCreator::get_bullets(){

  return bullets_;

}





