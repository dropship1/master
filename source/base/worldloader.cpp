#include "base/worldloader.hpp"  

using namespace bright::base;

WorldLoader::WorldLoader(std::shared_ptr<bright::utils::FileWorker> pFileWorker) :
  pFileWorker_(pFileWorker) {
  pAABBConverter_ = std::make_shared<bright::converters::AABBConverter>(pFileWorker);
}

void WorldLoader::load_world(std::map<std::string, ServerActor> &npcs) {

  std::string worldConfig = pFileWorker_->get_file_contents("world.cfg");
  pAABBConverter_->batch_read_obj_aabb_binary();

  //std::cout << "DEBUG: worldConfig: " << worldConfig << std::endl;

  std::stringstream in(worldConfig);

  bool isPlayer = false;
  std::string name;
  float posx = 0.0f;
  float posy = 0.0f;
  float posz = 0.0f;
  float rotx = 0.0f;
  float roty = 0.0f;
  float rotz = 0.0f;
  std::string aabbName;

  bool inServerActorNode = false;
  std::string line; 
  while (getline(in, line)){
    if (line.substr(0,1) == "]" ){
      break;
    }
    if (inServerActorNode){
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
      else if(line.substr(0,8) == "</Actor>"){
        if (!isPlayer) {
          ServerActor serverActor;
          serverActor.pos(glm::vec3(posx, posy, posz));
          serverActor.rotation(glm::vec3(rotx, roty, rotz));
          serverActor.aabb( pAABBConverter_->aabb(aabbName) );
          serverActor.aabb()->move(glm::vec3(posx, posy, posz));
          serverActor.name(name);
          npcs[name] = serverActor;
        }
      }
    }
    else if (line.substr(0,7) == "<Actor>"){ 
    }
    inServerActorNode = true;
    isPlayer = false;
  }
}