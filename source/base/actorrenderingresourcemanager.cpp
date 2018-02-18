#include "base/actorrenderingresourcemanager.hpp"

using namespace bright::base;


ActorRenderingResourceManager::ActorRenderingResourceManager(std::shared_ptr<bright::utils::FileWorker> pFileWorker): 
  pFileWorker_(pFileWorker), meshConverter_(pFileWorker), graphicsLoadersManager_(pFileWorker){
}


void ActorRenderingResourceManager::initialize(){

  graphicsLoadersManager_.load();
  meshConverter_.batch_read_obj_mesh_binary();
  load_resources_and_create_render_infos();
  create_world_info();

}

void ActorRenderingResourceManager::create_world_info(){

  bright::graphics::Light ambientLight;
  ambientLight.lightDirection_ = glm::vec3(0.0f, 50.0f, 0.0f);
  ambientLight.lightIntensity_ = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
  worldInfo_.ambient_light(ambientLight);

  bright::graphics::Light directionalLight;
  directionalLight.lightDirection_ = glm::vec3(0.0f, 30.0f, 100.0f);
  directionalLight.lightIntensity_ = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  worldInfo_.directional_light(directionalLight);

}


bright::graphics::WorldInfo& ActorRenderingResourceManager::world_info(){
  return worldInfo_;
}


bright::graphics::LoadersManager& ActorRenderingResourceManager::graphics_loader_manager(){
  return graphicsLoadersManager_;
}

std::map<std::string, bright::graphics::ActorGroupRenderInfo>& ActorRenderingResourceManager::actor_group_render_infos(){
  return actorGroupRenderInfos_;
}


std::map<std::string, ActorRenderController>& ActorRenderingResourceManager::actor_render_controllers(){
  return actorRenderControllers_;
}


std::vector<RenderActor>& ActorRenderingResourceManager::render_actors(){
  return renderActors_;
}


void ActorRenderingResourceManager::load_resources_and_create_render_infos(){

  std::string resourcesConfig = pFileWorker_->get_file_contents("rendering.res");
  std::stringstream in(resourcesConfig);

  std::string mesh;
  std::string shader;
  std::string renderName;

  std::string cameraType = "1st";
  bool inRenderActorNode = false;

  std::string line; 
  while (getline(in, line)){
    int start = 0;
    if (line.substr(0,1) == "]" ){
      break;
    }
    if (inRenderActorNode){
      if(line.substr(0,5) == "NAME="){
        renderName = line.substr(5);
      }
      else if(line.substr(0,5) == "MESH="){
        mesh = line.substr(5);
      }
      else if(line.substr(0,7) == "SHADER="){
        shader = line.substr(7);
      }
      else if(line.substr(0,11) == "CAMERATYPE="){
        cameraType = line.substr(11);
      }
      else if(line.substr(0,14) == "</RenderActor>"){
        RenderActor renderActor;
        renderActor.camera_type(cameraType);
        renderActor.render_name(renderName);
        renderActor.mesh(mesh);
        renderActor.shader(shader);
        renderActors_.push_back(renderActor);

        cameraType = "1st";
        inRenderActorNode = false;
      }
    }
    else if (line.substr(0,13) == "<RenderActor>"){ 
      inRenderActorNode = true;
    }

    int stop = 0;
  }

  int end = 0;
  //We only have a controller at the top level/root mesh.. so we cant move any of the children individually yet
  auto create_actor_render_controller = [&] (RenderActor& renderActor) {
    ActorRenderController actortRenderController;
    actortRenderController.name(renderActor.render_name());
    actorRenderControllers_[renderActor.render_name()] = actortRenderController;
  };
  std::for_each(renderActors_.begin(), renderActors_.end(), create_actor_render_controller);

  //The top level mesh is the root, and does not have much data (Currently we don't support children 
  //and animation on each child.. )
  auto get_root_mesh = [&] (RenderActor& renderActor) { 
    //This is the rootMesh, it contains metadata
    auto pRootMesh = meshConverter_.mesh( renderActor.mesh() );
    bright::graphics::ActorGroupRenderInfo rootActorGroupRenderInfo( graphicsLoadersManager_.shaders( renderActor.shader() ) );
    rootActorGroupRenderInfo.hasShader_ = true;
    rootActorGroupRenderInfo.cameraType_ = renderActor.camera_type();

    auto create_render_info = [&] (std::shared_ptr<Mesh> pChildMesh) {
      bright::graphics::ActorRenderInfo newActorRenderInfo(graphicsLoadersManager_.textures(pChildMesh->material()));
      newActorRenderInfo.hasTexture_ = true;
      rootActorGroupRenderInfo.actorRenderInfos_[pChildMesh->part_name()] = newActorRenderInfo;
      load_mesh_to_graphics_card( pChildMesh, rootActorGroupRenderInfo.actorRenderInfos_[pChildMesh->part_name()] );
    };
    std::for_each(pRootMesh->child_meshes().begin(), pRootMesh->child_meshes().end(), create_render_info);
    rootActorGroupRenderInfo.name_ = renderActor.render_name();
    actorGroupRenderInfos_[renderActor.render_name()] = rootActorGroupRenderInfo;
    
  };
  std::for_each(renderActors_.begin(), renderActors_.end(), get_root_mesh);
}


void ActorRenderingResourceManager::load_mesh_to_graphics_card(std::shared_ptr<Mesh> pChildMesh, bright::graphics::ActorRenderInfo& actorRenderInfo){

  glGenBuffers(1, &actorRenderInfo.vbo_);
  glGenBuffers(1, &actorRenderInfo.tbo_);
  glGenBuffers(1, &actorRenderInfo.nbo_);
  glGenVertexArrays(1, &actorRenderInfo.vao_);
  
  actorRenderInfo.vertexCoordsSize_ = pChildMesh->count_vertex_faces();
  
  //MAKE VBOs
  glBindBuffer(GL_ARRAY_BUFFER, actorRenderInfo.vbo_);
  glBufferData(GL_ARRAY_BUFFER, pChildMesh->count_vertex_faces()*sizeof(glm::vec4), &pChildMesh->vertex_faces()[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ARRAY_BUFFER, actorRenderInfo.nbo_);
  glBufferData(GL_ARRAY_BUFFER, pChildMesh->count_normal_faces()*sizeof(glm::vec3), &pChildMesh->normal_faces()[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ARRAY_BUFFER, actorRenderInfo.tbo_);
  glBufferData(GL_ARRAY_BUFFER, pChildMesh->count_texture_faces()*sizeof(glm::vec3), &pChildMesh->texture_faces()[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //Make VAO
  glBindVertexArray(actorRenderInfo.vao_);
  
  glBindBuffer(GL_ARRAY_BUFFER, actorRenderInfo.vbo_);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, actorRenderInfo.nbo_);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, actorRenderInfo.tbo_);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);

}






