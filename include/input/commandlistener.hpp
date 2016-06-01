#ifndef BRIGHT_INPUT_COMMAND_LISTENER_H
#define BRIGHT_INPUT_COMMAND_LISTENER_H

#include "input/commandeventlistener.hpp"
#include "input/commandevent.hpp"
#include "input/commandhandler.hpp"
#include "base/serveractor.hpp"
#include "base/clientcontroller.hpp"
#include "base/actorcreator.hpp"

namespace bright{

  namespace input{

class CommandListener : public CommandEventListener{

public:
  CommandListener(std::shared_ptr<bright::base::ServerActor> pClientActor, std::shared_ptr<bright::base::ClientController> pClientController,
                  std::shared_ptr<bright::base::ActorCreator> pActorCreator);

  void update_commands();

  void process_messages();
  void on_command_event(std::shared_ptr<bright::input::CommandEvent> commandEvent);
  void execute_commands();

private:


private:

  bright::input::CommandHandler commandHandler_;

};


  }

}


#endif












