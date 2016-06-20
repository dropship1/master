#include "physics/physicsmanager.hpp"

using namespace bright::physics;


PhysicsManager::PhysicsManager(std::shared_ptr<bright::base::ActorControlsResourceManager> pActorControlsResourceManager) :
  controlPlayers_(pActorControlsResourceManager->control_players() ),
  controlNpcs_(pActorControlsResourceManager->control_npcs() ),
  playerControllers_(pActorControlsResourceManager->player_controllers() ),
  npcControllers_(pActorControlsResourceManager->npc_controllers() ),
  playerAABBs_(pActorControlsResourceManager->player_aabbs() ),
  npcAABBs_(pActorControlsResourceManager->npc_aabbs() ) {

}


void PhysicsManager::handle_physics(){
  
  auto did_npc_collide_with_player = [&] (std::map<std::string, bright::base::ControlActor>::value_type& pair) {
    std::string npcName = pair.first;
    bright::base::ControlActor& controlNpc = pair.second;
    auto& npcAABB = npcAABBs_[controlNpc.aabb()];
    auto& npcController = npcControllers_[npcName];

    auto check_player = [&] (std::map<std::string, bright::base::ControlActor>::value_type& innerPair) {
      std::string playerName = innerPair.first;
      auto& player = innerPair.second;
      if ( player.is_logged_in() ){
        auto& playerAABB = playerAABBs_[player.aabb()];
        bool intersect = npcAABB.intersect(playerAABB);
        if (intersect){
          npcController.set_to_prev_pos();
          npcController.has_changed_this_frame(true);
        }
      }
    };
    std::for_each(controlPlayers_.begin(), controlPlayers_.end(), check_player);
  };
  std::for_each(controlNpcs_.begin(), controlNpcs_.end(), did_npc_collide_with_player);


  auto did_player_collide_with_npc = [&] (std::map<std::string, bright::base::ControlActor>::value_type& pair) {
     std::string playerName = pair.first;
     auto& player = pair.second;
     if ( player.is_logged_in() ){
      auto& playerAABB = playerAABBs_[player.aabb()];
      auto& playerController = playerControllers_[playerName];

      auto check_npc = [&] (std::map<std::string, bright::base::ControlActor>::value_type& innerPair) {
        std::string npcName = innerPair.first;
        auto& controlNpc = innerPair.second;
        auto& npcAABB = npcAABBs_[controlNpc.aabb()];

        bool intersect = playerAABB.intersect(npcAABB);
        if (intersect){
          playerController.set_to_prev_pos();
          playerController.has_changed_this_frame(true);
        }
      };
      std::for_each(controlNpcs_.begin(), controlNpcs_.end(), check_npc);

    }
  };
  std::for_each(controlPlayers_.begin(), controlPlayers_.end(), did_player_collide_with_npc);


  auto did_npc_collide_with_npc = [&] (std::map<std::string, bright::base::ControlActor>::value_type& pair) {
    std::string npcName = pair.first;
    auto& controlNpc = pair.second;
    auto& npcAABB = npcAABBs_[controlNpc.aabb()];
  
    auto check_npc = [&] (std::map<std::string, bright::base::ControlActor>::value_type& innerPair) -> bool {
      std::string npcName2 = innerPair.first;
      if (npcName2.compare(npcName) != 0){
        auto& controlNpc2 = innerPair.second;
        auto& npc2AABB = npcAABBs_[controlNpc2.aabb()];
        bool intersect = npcAABB.intersect(npc2AABB);
        if (intersect){
          auto& npcController = npcControllers_[npcName];
          npcController.set_to_prev_pos();
          npcController.has_changed_this_frame(true);
          return true;
        }
        else{ return false; }
      }
      else{ return false; }
    };

    for(auto& npc2 : controlNpcs_) {
      if ( check_npc(npc2) == true ){
        break;
      }
    }

  };
  for(auto& npc: controlNpcs_){ did_npc_collide_with_npc(npc); } 
}

