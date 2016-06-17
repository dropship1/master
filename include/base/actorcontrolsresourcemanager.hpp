#ifndef BRIGHT_BASE_ACTOR_CONTROLS_RESOURCE_MANAGER_H
#define BRIGHT_BASE_ACTOR_CONTROLS_RESOURCE_MANAGER_H

#include <memory>
#include <map>
#include "base/controlactor.hpp"
#include "base/actorcontrolcontroller.hpp"
#include "physics/aabb.hpp"
#include "converters/aabbconverter.hpp"
#include "utils/fileworker.hpp"


namespace bright{

  namespace base{

class ActorControlsResourceManager {

public:
  ActorControlsResourceManager(std::shared_ptr<bright::utils::FileWorker> pFileWorker);

  // Return number of monsters that are moved but haven't yet reached player.
  void initialize();
  std::map<std::string, ActorControlController>& npc_controllers();
  std::map<std::string, ActorControlController>& player_controllers();
  std::map<std::string, ControlActor>& control_npcs();
  std::map<std::string, ControlActor>& control_players();
  std::map<std::string, bright::physics::AABB>& npc_aabbs();
  std::map<std::string, bright::physics::AABB>& player_aabbs();

private:
  std::shared_ptr<bright::utils::FileWorker> pFileWorker_;
  bright::converters::AABBConverter aabbConverter_;

  std::map<std::string, ControlActor> controlPlayers_;
  std::map<std::string, ControlActor> controlNpcs_;
  std::map<std::string, ActorControlController> playerControllers_;
  std::map<std::string, ActorControlController> NpcControllers_;
  std::map<std::string, bright::physics::AABB> playerAABBs_;
  std::map<std::string, bright::physics::AABB> npcAABBs_;

  void load_control_actors();
  void create_controllers();
  void assign_aabbs();


};

  }
}
#endif
