#ifndef BRIGHT_WORLD_WORLDLOADER_H
#define BRIGHT_WORLD_WORLDLOADER_H

#include <memory>
#include <map>
#include "base/serveractor.hpp"
#include "utils/fileworker.hpp"
#include "physics/aabb.hpp"
#include "converters/aabbconverter.hpp"


namespace bright{

  namespace base{

class WorldLoader {

public:
  WorldLoader(std::shared_ptr<bright::utils::FileWorker> pFileWorker);

  // Return number of monsters that are moved but haven't yet reached player.
  void load_world(std::map<std::string, ServerActor> &npcs);

private:
  std::shared_ptr<bright::utils::FileWorker> pFileWorker_;
  std::shared_ptr<bright::converters::AABBConverter> pAABBConverter_;

};

  }
}
#endif
