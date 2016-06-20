#ifndef BRIGHT_BASE_ACTOR_RENDERING_RESOURCE_MANAGER_H
#define BRIGHT_BASE_ACTOR_RENDERING_RESOURCE_MANAGER_H

#include "graphics/worldinfo.hpp"
#include "graphics/loadersmanager.hpp"
#include "graphics/globalstructs.hpp"

#include "base/renderactor.hpp"
#include "base/mesh.hpp"
#include "base/actorrendercontroller.hpp"

#include "audio/soundmanager.hpp"

#include "converters/meshconverter.hpp"

#include "utils/utilities.hpp"

#include <glm/glm.hpp>

#include <map>
#include <memory>

namespace bright{

  namespace base{

class ActorRenderingResourceManager{

public:
  ActorRenderingResourceManager(std::shared_ptr<bright::utils::FileWorker> pFileWorker);

  void initialize();
  void load_resources_and_create_render_infos();
  void create_world_info();

  bright::graphics::WorldInfo& world_info();
  std::map<std::string, bright::graphics::ActorGroupRenderInfo>& actor_group_render_infos();
  std::map<std::string, ActorRenderController>& actor_render_controllers();
  std::vector<RenderActor>& render_actors();
  bright::graphics::LoadersManager& graphics_loader_manager();

private:
  void load_mesh_to_graphics_card(std::shared_ptr<Mesh> pChildMesh, bright::graphics::ActorRenderInfo& actorRenderInfo);

  bright::audio::SoundManager soundManager_;
  bright::converters::MeshConverter meshConverter_;
  bright::graphics::LoadersManager graphicsLoadersManager_;

  std::vector<RenderActor> renderActors_;
  std::map<std::string, bright::graphics::ActorGroupRenderInfo> actorGroupRenderInfos_;
  std::map<std::string, ActorRenderController> actorRenderControllers_;
  bright::graphics::WorldInfo worldInfo_;

  std::shared_ptr<bright::utils::FileWorker> pFileWorker_;

};


  }
}

#endif