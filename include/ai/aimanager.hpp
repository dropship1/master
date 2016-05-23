#ifndef BRIGHT_AI_AIMANAGER_H
#define BRIGHT_AI_AIMANAGER_H

#include <memory>
#include <map>
#include <iostream>
#include "base/serveractor.hpp"

using namespace bright::base;

namespace bright{

  namespace ai{

    class AIManager {
    public:
      AIManager();

      // Return number of monsters that are moved but haven't yet reached player.
      int AIManager::update_monsters(std::map<std::string, ServerActor> &monsters, std::map<std::string, std::shared_ptr<ServerActor>> &players);
    };

  }
}
#endif
