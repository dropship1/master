#include "base/resourcemanager.hpp"

using namespace bright::base;


ResourceManager::ResourceManager(std::shared_ptr<bright::utils::FileWorker> pFileWorker): 
  pFileWorker_(pFileWorker), meshConverter_(pFileWorker), graphicsLoadersManager_(pFileWorker){
}


void ResourceManager::initialize(){

  pWorldInfo_ = std::make_shared<bright::graphics::WorldInfo>();
  graphicsLoadersManager_.load();
  meshConverter_.batch_read_obj_mesh_binary();
  load_resources_and_create_render_infos();
  create_world_info();

}

void ResourceManager::create_world_info(){

  bright::graphics::Light ambientLight;
  ambientLight.lightDirection_ = glm::vec3(0.0f, 50.0f, 0.0f);
  ambientLight.lightIntensity_ = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
  pWorldInfo_->ambient_light(ambientLight);

  bright::graphics::Light directionalLight;
  directionalLight.lightDirection_ = glm::vec3(0.0f, 30.0f, 100.0f);
  directionalLight.lightIntensity_ = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  pWorldInfo_->directional_light(directionalLight);

}


std::shared_ptr<bright::graphics::WorldInfo> ResourceManager::world_info(){
  return pWorldInfo_;
}


bright::graphics::LoadersManager& ResourceManager::graphics_loader_manager(){
  return graphicsLoadersManager_;
}

std::map<std::string, std::shared_ptr<bright::graphics::ActorGroupRenderInfo>>& ResourceManager::actor_group_render_infos(){
  return actorGroupRenderInfos_;
}


std::map<std::string, std::shared_ptr<ClientController>>& ResourceManager::controllers(){
  return controllers_;
}


std::vector<Actor>& ResourceManager::actors(){
  return actors_;
}


void ResourceManager::load_resources_and_create_render_infos(){

  std::string resourcesConfig = pFileWorker_->get_file_contents("resources.cfg");
  std::stringstream in(resourcesConfig);

  std::string mesh;
  std::string shader;
  bool isPlayer = false;
  std::string name;
  std::string cameraType = "1st";

  bool inActorNode = false;
  std::string line; 
  while (getline(in, line)){
    if (line.substr(0,1) == "]" ){
      break;
    }
    if (inActorNode){
      if(line.substr(0,5) == "name="){
        name = line.substr(5);
      }
      else if(line.substr(0,5) == "mesh="){
        mesh = line.substr(5);
      }
      else if(line.substr(0,7) == "shader="){
        shader = line.substr(7);
      }
      else if(line.substr(0,11) == "cameratype="){
        cameraType = line.substr(11);
      }
      else if(line.substr(0,7) == "player="){
        std::istringstream is(line.substr(7));
        is >> std::boolalpha >> isPlayer;
      }
      else if(line.substr(0,8) == "</Actor>"){
        Actor actor;
        actor.camera_type(cameraType);
        actor.is_player(isPlayer);
        actor.name(name);
        actor.mesh(mesh);
        actor.shader(shader);
        actors_.push_back(actor);
        isPlayer = false;
        cameraType = "1st";
        inActorNode = false;
      }
    }
    else if (line.substr(0,7) == "<Actor>"){ 
      inActorNode = true;
    }
  }

  //We only have a controller at the top level/root mesh.. so we cant move any of the children
  //individually yet
  auto create_controller = [&] (Actor& actor) {
    auto controller = std::make_shared<ClientController>();
    controllers_[actor.name()] = controller;
  };
  std::for_each(actors_.begin(), actors_.end(), create_controller);

  //The top level mesh is the root, and does not have much data (Currently we dont support children 
  //and animation on each child.. )
  auto get_root_mesh = [&] (Actor& actor) { 
    //This is the rootMesh, it contains metadata
    auto pRootMesh = meshConverter_.mesh(actor.mesh());
    auto pRootActorGroupRenderInfo = std::make_shared<bright::graphics::ActorGroupRenderInfo>();
    pRootActorGroupRenderInfo->cameraType_ = actor.camera_type();
    pRootActorGroupRenderInfo->pShader_ = graphicsLoadersManager_.shaders(actor.shader());
    pRootActorGroupRenderInfo->hasShader_ = true;

    auto create_render_info = [&] (std::shared_ptr<Mesh> pChildMesh) { 
      auto pActorRenderInfo = std::make_shared<bright::graphics::ActorRenderInfo>();
      load_mesh_to_graphics_card(pChildMesh, pActorRenderInfo);
      pActorRenderInfo->pTexture_ = graphicsLoadersManager_.textures(pChildMesh->material());
      pActorRenderInfo->hasTexture_ = true;
      pRootActorGroupRenderInfo->actorRenderInfos_[pChildMesh->part_name()] = pActorRenderInfo;
    };
    std::for_each(pRootMesh->child_meshes().begin(), pRootMesh->child_meshes().end(), create_render_info);
    actorGroupRenderInfos_[actor.name()] = pRootActorGroupRenderInfo;
    
  };
  std::for_each(actors_.begin(), actors_.end(), get_root_mesh);
}


void ResourceManager::load_mesh_to_graphics_card(std::shared_ptr<Mesh> pChildMesh, std::shared_ptr<bright::graphics::ActorRenderInfo> pActorRenderInfo){

  glGenBuffers(1, &pActorRenderInfo->vbo_);
  glGenBuffers(1, &pActorRenderInfo->tbo_);
  glGenBuffers(1, &pActorRenderInfo->nbo_);
  glGenVertexArrays(1, &pActorRenderInfo->vao_);
  
  pActorRenderInfo->vertexCoordsSize_ = pChildMesh->count_vertex_faces();
  
  //MAKE VBOs
  glBindBuffer(GL_ARRAY_BUFFER, pActorRenderInfo->vbo_);
  glBufferData(GL_ARRAY_BUFFER, pChildMesh->count_vertex_faces()*sizeof(glm::vec4), &pChildMesh->vertex_faces()[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ARRAY_BUFFER, pActorRenderInfo->nbo_);
  glBufferData(GL_ARRAY_BUFFER, pChildMesh->count_normal_faces()*sizeof(glm::vec3), &pChildMesh->normal_faces()[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ARRAY_BUFFER, pActorRenderInfo->tbo_);
  glBufferData(GL_ARRAY_BUFFER, pChildMesh->count_texture_faces()*sizeof(glm::vec3), &pChildMesh->texture_faces()[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //Make VAO
  glBindVertexArray(pActorRenderInfo->vao_);
  
  glBindBuffer(GL_ARRAY_BUFFER, pActorRenderInfo->vbo_);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, pActorRenderInfo->nbo_);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, pActorRenderInfo->tbo_);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);

}






