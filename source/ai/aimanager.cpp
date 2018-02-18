#include "ai/aimanager.hpp"

using namespace bright::ai;


AIManager::AIManager(std::shared_ptr<bright::base::ActorControlsResourceManager> pActorControlsResourceManager) :
  controlPlayers_(pActorControlsResourceManager->control_players() ),
  controlNpcs_(pActorControlsResourceManager->control_npcs() ),
  playerControllers_(pActorControlsResourceManager->player_controllers() ),
  npcControllers_(pActorControlsResourceManager->npc_controllers() ),
  playerAABBs_(pActorControlsResourceManager->player_aabbs() ),
  npcAABBs_(pActorControlsResourceManager->npc_aabbs() ) {

}


int AIManager::handle_ai() {
  int npcMoveCount = 0;
  float speedMetersPerUpdate = 0.1f;

  // Have each npc point to nearest player, and move towards it.
  for(auto iterNpc = controlNpcs_.begin(); iterNpc != controlNpcs_.end(); ++iterNpc) {
    auto &controlNpc = (*iterNpc).second;
    auto& npcController = npcControllers_[controlNpc.control_name()];
    auto npcPos = npcController.pos();

    //Find nearest player
    //Use squared distance instead of distance, because it takes more time to find square root.
    //Wait until we find nearest player to get actual distance.
    double nearestDistanceSquared = 0.0f;
    glm::vec3 nearestPositionDelta;
    bool isFirstPlayer = true;
    glm::vec3 nearestPlayerPos;
    for(auto iterPlayer = controlPlayers_.begin(); iterPlayer != controlPlayers_.end(); ++iterPlayer) {
      auto &controlPlayer = (*iterPlayer).second;
      auto& playerController = playerControllers_[controlPlayer.control_name()];
      auto playerPos = playerController.pos();

      auto positionDelta = playerPos - npcPos;
      double distanceSquared = dot(positionDelta, positionDelta);

      if ( isFirstPlayer || (distanceSquared < nearestDistanceSquared) ) {
        nearestPlayerPos = playerPos;
        nearestPositionDelta = positionDelta;
        nearestDistanceSquared = distanceSquared;
        isFirstPlayer = false;
      }
    } // end player loop

    double nearestDistance = sqrt(nearestDistanceSquared);
    //std::cout << "nearestDistance: " << nearestDistance << std::endl;

    //Point npc to nearest player, use normalized vector
    auto normalizedNpcDirection = normalize(nearestPositionDelta);
    npcController.update_axes( npcPos, nearestPlayerPos );

    // Move npc towards nearest player
    if (nearestDistance < speedMetersPerUpdate) {
      npcController.pos( nearestPlayerPos );
      //std::cout << "NPC REACHED PLAYER!!!  :O" << std::endl;
    }
    else {
      //std::cout << "NPC LURCHES TOWARD PLAYER..." << std::endl;
      //npcController.pos( npcController.pos() + (normalizedNpcDirection * speedMetersPerUpdate) );
      npcController.move_fwd( speedMetersPerUpdate );
      npcMoveCount += 1;
    }
    //std::wcout << "npcMoveCount: " << npcMoveCount << std::endl;
  } // end npc loop

  return npcMoveCount;
}

