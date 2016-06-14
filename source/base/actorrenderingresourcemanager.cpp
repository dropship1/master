#include "base/actorrenderingresourcemanager.hpp"

using namespace bright::base;


ActorRenderingResourceManager::ActorRenderingResourceManager(std::shared_ptr<bright::utils::FileWorker> pFileWorker): 
  pFileWorker_(pFileWorker), meshConverter_(pFileWorker), graphicsLoadersManager_(pFileWorker){
}


void ActorRenderingResourceManager::initialize(){

  pWorldInfo_ = std::make_shared<bright::graphics::WorldInfo>();
  graphicsLoadersManager_.load();
  meshConverter_.batch_read_obj_mesh_binary();
  load_resources_and_create_render_infos();
  create_world_info();

}

void ActorRenderingResourceManager::create_world_info(){

  bright::graphics::Light ambientLight;
  ambientLight.lightDirection_ = glm::vec3(0.0f, 50.0f, 0.0f);
  ambientLight.lightIntensity_ = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
  pWorldInfo_->ambient_light(ambientLight);

  bright::graphics::Light directionalLight;
  directionalLight.lightDirection_ = glm::vec3(0.0f, 30.0f, 100.0f);
  directionalLight.lightIntensity_ = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  pWorldInfo_->directional_light(directionalLight);

}


std::shared_ptr<bright::graphics::WorldInfo> ActorRenderingResourceManager::world_info(){
  return pWorldInfo_;
}


bright::graphics::LoadersManager& ActorRenderingResourceManager::graphics_loader_manager(){
  return graphicsLoadersManager_;
}

std::map<std::string, std::shared_ptr<bright::graphics::ActorGroupRenderInfo>>& ActorRenderingResourceManager::actor_group_render_infos(){
  return actorGroupRenderInfos_;
}


std::map<std::string, std::shared_ptr<ClientController>>& ActorRenderingResourceManager::controllers(){
  return controllers_;
}


std::vector<RenderActor>& ActorRenderingResourceManager::render_actors(){
  return renderActors_;
}


void ActorRenderingResourceManager::load_resources_and_create_render_infos(){

  std::string resourcesConfig = pFileWorker_->get_file_contents("rendering.res");
  std::stringstream in(resourcesConfig);

  std::string mesh;
  std::string shader;
  std::string name;

  std::string cameraType = "1st";
  bool inRenderActorNode = false;

  std::string line; 
  while (getline(in, line)){
    if (line.substr(0,1) == "]" ){
      break;
    }
    if (inRenderActorNode){
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
      else if(line.substr(0,8) == "</RenderActor>"){
        RenderActor renderActor;
        renderActor.camera_type(cameraType);
        renderActor.name(name);
        renderActor.mesh(mesh);
        renderActor.shader(shader);
        renderActors_.push_back(renderActor);

        cameraType = "1st";
        inActorNode = false;
      }
    }
    else if (line.substr(0,7) == "<RenderActor>"){ 
      inRenderActorNode = true;
    }
  }

  //We only have a controller at the top level/root mesh.. so we cant move any of the children
  //individually yet
  auto create_actor_render_controller = [&] (RenderActor& renderActor) {
    auto actortRenderController = std::make_shared<ActorRenderController>();
    actorRenderControllers_[renderActor.name()] = actortRenderController;
  };
  std::for_each(renderActors_.begin(), renderActors_.end(), create_actor_render_controller);

  //The top level mesh is the root, and does not have much data (Currently we dont support children 
  //and animation on each child.. )
  auto get_root_mesh = [&] (RenderActor& renderActor) { 
    //This is the rootMesh, it contains metadata
    auto pRootMesh = meshConverter_.mesh(renderActor.mesh());
    auto pRootActorGroupRenderInfo = std::make_shared<bright::graphics::ActorGroupRenderInfo>();
    pRootActorGroupRenderInfo->cameraType_ = renderActor.camera_type();
    pRootActorGroupRenderInfo->pShader_ = graphicsLoadersManager_.shaders(renderActor.shader());
    pRootActorGroupRenderInfo->hasShader_ = true;

    auto create_render_info = [&] (std::shared_ptr<Mesh> pChildMesh) { 
      auto pActorRenderInfo = std::make_shared<bright::graphics::ActorRenderInfo>();
      load_mesh_to_graphics_card(pChildMesh, pActorRenderInfo);
      pActorRenderInfo->pTexture_ = graphicsLoadersManager_.textures(pChildMesh->material());
      pActorRenderInfo->hasTexture_ = true;
      pRootActorGroupRenderInfo->actorRenderInfos_[pChildMesh->part_name()] = pActorRenderInfo;
    };
    std::for_each(pRootMesh->child_meshes().begin(), pRootMesh->child_meshes().end(), create_render_info);
    actorGroupRenderInfos_[renderActor.name()] = pRootActorGroupRenderInfo;
    
  };
  std::for_each(renderActors_.begin(), renderActors_.end(), get_root_mesh);
}


void ActorRenderingResourceManager::load_mesh_to_graphics_card(std::shared_ptr<Mesh> pChildMesh, std::shared_ptr<bright::graphics::ActorRenderInfo> pActorRenderInfo){

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






