#ifndef BRIGHT_BASE_ACTOR_CREATOR_H
#define BRIGHT_BASE_ACTOR_CREATOR_H

#include <glm/glm.hpp>

#include <map>
#include <memory>

#include "base/clientcontroller.hpp"
#include "base/serveractor.hpp"
#include "utils/utilities.hpp"
#include "utils/fileworker.hpp"

namespace bright{

  namespace base{

class ActorCreator{

public:
  ActorCreator(std::shared_ptr<bright::utils::FileWorker> pFileWorker);
  ActorCreator();

  void add_bullet(std::shared_ptr<bright::base::ServerActor> pClientActor);
  std::vector<std::shared_ptr<bright::base::ServerActor>>& ActorCreator::get_bullets();

private:

  std::shared_ptr<bright::utils::FileWorker> pFileWorker_;

  std::vector<std::shared_ptr<bright::base::ServerActor>> bullets_;

};


  }
}

#endif