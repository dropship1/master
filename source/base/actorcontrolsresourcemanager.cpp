#include "base/actorcontrolsresourcemanager.hpp"  

using namespace bright::base;

ActorControlsResourceManager::ActorControlsResourceManager(std::shared_ptr<bright::utils::FileWorker> pFileWorker) : 
  pFileWorker_(pFileWorker), aabbConverter_(pFileWorker){
}


void ActorControlsResourceManager::initialize(){

  aabbConverter_.batch_read_obj_aabb_binary();

  load_control_actors();
  create_controllers();
  assign_aabbs();

}


std::map<std::string, ActorControlController>& ActorControlsResourceManager::npc_controllers(){
  return NpcControllers_;
}


std::map<std::string, ActorControlController>& ActorControlsResourceManager::player_controllers(){
  return playerControllers_;
}


std::map<std::string, ControlActor>& ActorControlsResourceManager::control_npcs(){
  return controlNpcs_;
}


std::map<std::string, ControlActor>& ActorControlsResourceManager::control_players(){
  return controlPlayers_;
}


std::map<std::string, bright::physics::AABB>& ActorControlsResourceManager::npc_aabbs(){
  return npcAABBs_;
}


std::map<std::string, bright::physics::AABB>& ActorControlsResourceManager::player_aabbs(){
  return playerAABBs_;
}


void ActorControlsResourceManager::create_controllers(){

  auto create_npc_controller = [&] (ControlActor& controlActor) { 
    ActorControlController actorController;
    actorController.pos( controlActor.pos() );
    actorController.rotation( controlActor.rotation() );
    NpcControllers_[controlActor.name()] = actorController;
  };
  std::for_each(controlNpcs_.begin(), controlNpcs_.end(), create_npc_controller);

  auto create_player_controller = [&] (ControlActor& controlActor) { 
    ActorControlController actorController;
    actorController.pos( controlActor.pos() );
    actorController.rotation( controlActor.rotation() );
    playerControllers_[controlActor.name()] = actorController;
  };
  std::for_each(controlPlayers_.begin(), controlPlayers_.end(), create_player_controller);

}


void ActorControlsResourceManager::assign_aabbs(){

  auto assign_npc_aabb = [&] (ControlActor& controlActor) { 
    npcAABBs_[controlActor.name()] = aabbConverter_.aabb_stack_copy( controlActor.aabb() );
  };
  std::for_each(controlNpcs_.begin(), controlNpcs_.end(), assign_npc_aabb);

  auto assign_player_aabb = [&] (ControlActor& controlActor) { 
    playerAABBs_[controlActor.name()] = aabbConverter_.aabb_stack_copy( controlActor.aabb() );
  };
  std::for_each(controlPlayers_.begin(), controlPlayers_.end(), assign_player_aabb);


}


void ActorControlsResourceManager::load_control_actors() {

  std::string controlsWorld = pFileWorker_->get_file_contents("controlling.wrld");


  //std::cout << "DEBUG: controlsWorld: " << controlsWorld << std::endl;

  std::stringstream in(controlsWorld);

  bool isPlayer = false;
  std::string name;
  float posx = 0.0f;
  float posy = 0.0f;
  float posz = 0.0f;
  float rotx = 0.0f;
  float roty = 0.0f;
  float rotz = 0.0f;
  std::string aabbName;

  bool inControlActorNode = false;
  std::string line; 
  while (getline(in, line)){
    if (line.substr(0,1) == "]" ){
      break;
    }
    if (inControlActorNode){
      if(line.substr(0,5) == "NAME="){
        name = line.substr(5);
      }
      if(line.substr(0,5) == "POSX="){
        posx = std::stof(line.substr(5));
      }
      if(line.substr(0,5) == "POSY="){
        posy = std::stof(line.substr(5));
      }
      if(line.substr(0,5) == "POSZ="){
        posz = std::stof(line.substr(5));
      }
      if(line.substr(0,5) == "ROTX="){
        rotx = std::stof(line.substr(5));
      }
      if(line.substr(0,5) == "ROTY="){
        roty = std::stof(line.substr(5));
      }
      if(line.substr(0,5) == "ROTZ="){
        rotz = std::stof(line.substr(5));
      }
      if(line.substr(0,5) == "AABB="){
        aabbName = line.substr(5);
      }
      else if(line.substr(0,7) == "PLAYER="){
        std::istringstream is(line.substr(7));
        is >> std::boolalpha >> isPlayer;
      }
      else if(line.substr(0,15) == "</ControlActor>"){
        ControlActor controlActor;
        controlActor.pos(glm::vec3(posx, posy, posz));
        controlActor.rotation(glm::vec3(rotx, roty, rotz));
        controlActor.aabb(aabbName);
        controlActor.name(name);
        controlActor.is_player(true);
        if (!isPlayer) {
          controlNpcs_[name] = controlActor;
        }
        if (isPlayer) {
          controlPlayers_[name] = controlActor;
        }
      }
    }
    else if (line.substr(0,14) == "<ControlActor>"){ 
      inControlActorNode = true;
      isPlayer = false;
    }
  }
}