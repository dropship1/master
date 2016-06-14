#include "base/actorcontrolsresourcemanager.hpp"  

using namespace bright::base;

ActorControlsResourceManager::ActorControlsResourceManager(std::shared_ptr<bright::utils::FileWorker> pFileWorker, std::shared_ptr<bright::converters::AABBConverter> pAABBConverter) : pFileWorker_(pFileWorker), pAABBConverter_(pAABBConverter) {
}

void ActorControlsResourceManager::load_actor_controls(std::map<std::string, ControlActor> &npcs, std::map<std::string, ControlActor> &players) {

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
      else if(line.substr(0,8) == "</ControlActor>"){
        ControlActor controlActor;
        controlActor.pos(glm::vec3(posx, posy, posz));
        controlActor.rotation(glm::vec3(rotx, roty, rotz));
        controlActor.aabb(aabbName);
        controlActor.name(name);
        if (!isPlayer) {
          npcs[name] = controlActor;
        }
        if (isPlayer) {
          players[name] = controlActor;
        }
      }
    }
    else if (line.substr(0,7) == "<ControlActor>"){ 
    }
    inControlActorNode = true;
    isPlayer = false;
  }
}