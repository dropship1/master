#ifndef BRIGHT_AI_AIMANAGER_H
#define BRIGHT_AI_AIMANAGER_H

#include <memory>
#include <map>
#include <iostream>
#include "base/actorcontrolcontroller.hpp"

namespace bright{

  namespace ai{

    class AIManager {
    public:
      AIManager();

      // Return number of monsters that are moved but haven't yet reached player.
      int AIManager::update_monsters(std::map<std::string, bright::base::ActorControlController> &monsters, std::map<std::string, std::shared_ptr<bright::base::ActorControlController>> &players);
    };

  }
}
#endif
