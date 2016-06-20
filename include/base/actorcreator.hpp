#ifndef BRIGHT_BASE_ACTOR_CREATOR_H
#define BRIGHT_BASE_ACTOR_CREATOR_H

#include <glm/glm.hpp>

#include <map>
#include <memory>

#include "base/actorcontrolcontroller.hpp"
#include "base/controlactor.hpp"
#include "utils/utilities.hpp"
#include "utils/fileworker.hpp"

namespace bright{

  namespace base{

class ActorCreator{

public:
  ActorCreator(std::shared_ptr<bright::utils::FileWorker> pFileWorker);
  ActorCreator();

  void add_bullet(bright::base::ActorControlController& currentActorController);

  std::vector<bright::base::ActorControlController>& ActorCreator::get_bullets();

private:

  std::vector<bright::base::ActorControlController> bullets_;

};


  }
}

#endif