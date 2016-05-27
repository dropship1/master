#include "ai/aimanager.hpp"

using namespace bright::ai;

AIManager::AIManager() {
}
int AIManager::update_monsters(std::map<std::string, ServerActor> &monsters, std::map<std::string, std::shared_ptr<ServerActor>> &players) {
  int monsterMoveCount = 0;
  float speedMetersPerUpdate = 0.1f;
  //Don't do anything if no players
  if ( players.empty() ) {
    return 0;
  }
  // Have each monster point to nearest player, and move towards it.
  for(auto iterMonster = monsters.begin(); iterMonster != monsters.end(); ++iterMonster) {
    auto &monster = (*iterMonster).second;
    auto monsterPos = monster.pos();

    // Find nearest player
    // Use squared distance instead of distance, because it takes more time to find square root.
    // Wait until we find nearest player to get actual distance.
    double nearestDistanceSquared = 0.0f;
    glm::vec3 nearestPositionDelta;
    bool isFirstPlayer = true;
    std::shared_ptr<ServerActor> pNearestPlayer;
    for(auto iterPlayer = players.begin(); iterPlayer != players.end(); ++iterPlayer) {
      auto pPlayer = (*iterPlayer).second;
      auto playerPos = pPlayer->pos();

      auto positionDelta = playerPos - monsterPos;
      double distanceSquared = dot(positionDelta, positionDelta);

      if ( isFirstPlayer || (distanceSquared < nearestDistanceSquared) ) {
        pNearestPlayer = pPlayer;
        nearestPositionDelta = positionDelta;
        nearestDistanceSquared = distanceSquared;
        isFirstPlayer = false;
      }
    } // end player loop

    double nearestDistance = sqrt(nearestDistanceSquared);
    //std::cout << "nearestDistance: " << nearestDistance << std::endl;

    //Point monster to nearest player, use normalized vector
    auto normalizedMonsterDirection = normalize(nearestPositionDelta);
    monster.point_at( monsterPos, pNearestPlayer->pos() );

    // Move monster towards nearest player
    if (nearestDistance < speedMetersPerUpdate) {
      monster.pos( pNearestPlayer->pos() );
      //std::cout << "MONSTER REACHED PLAYER!!!  :O" << std::endl;
    }
    else {
      //std::cout << "MONSTER LURCHES TOWARD PLAYER..." << std::endl;
      monster.pos( monster.pos() + (normalizedMonsterDirection * speedMetersPerUpdate) );
      monsterMoveCount += 1;
      monster.have_update(true);
    }
    //std::wcout << "monsterMoveCount: " << monsterMoveCount << std::endl;
  } // end monster loop

  return monsterMoveCount;
}

