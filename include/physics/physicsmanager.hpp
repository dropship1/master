#ifndef BRIGHT_PHYSICS_PHYSICSMANAGER_H
#define BRIGHT_PHYSICS_PHYSICSMANAGER_H

#include <memory>
#include <map>
#include <iostream>
#include "base/actorcontrolsresourcemanager.hpp"
#include "base/actorcontrolcontroller.hpp"
#include "base/controlactor.hpp"

namespace bright{

  namespace physics{

class PhysicsManager {
public:
  PhysicsManager(std::shared_ptr<bright::base::ActorControlsResourceManager> pActorControlsResourceManager);

  void handle_physics();

private:

  std::map<std::string, bright::base::ControlActor>& controlPlayers_;
  std::map<std::string, bright::base::ControlActor>& controlNpcs_;
  std::map<std::string, bright::base::ActorControlController>& playerControllers_;
  std::map<std::string, bright::base::ActorControlController>& npcControllers_;
  std::map<std::string, bright::physics::AABB>& playerAABBs_;
  std::map<std::string, bright::physics::AABB>& npcAABBs_;



};

  }
}
#endif
