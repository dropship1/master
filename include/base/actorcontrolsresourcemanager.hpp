#ifndef BRIGHT_BASE_ACTOR_CONTROLS_RESOURCE_MANAGER_H
#define BRIGHT_BASE_ACTOR_CONTROLS_RESOURCE_MANAGER_H

#include <memory>
#include <map>
#include "base/controlactor.hpp"
#include "utils/fileworker.hpp"
#include "physics/aabb.hpp"
#include "converters/aabbconverter.hpp"


namespace bright{

  namespace base{

class ActorControlsResourceManager {

public:
  ActorControlsResourceManager(std::shared_ptr<bright::utils::FileWorker> pFileWorker, std::shared_ptr<bright::converters::AABBConverter> pAABBConverter);

  // Return number of monsters that are moved but haven't yet reached player.
  void load_actor_controls(std::map<std::string, ControlActor> &npcs, std::map<std::string, ControlActor> &players);

private:
  std::shared_ptr<bright::utils::FileWorker> pFileWorker_;
  std::shared_ptr<bright::converters::AABBConverter> pAABBConverter_;

};

  }
}
#endif
