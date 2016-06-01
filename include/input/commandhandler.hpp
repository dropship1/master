#ifndef BRIGHT_INPUT_COMMAND_HANDLER_H
#define BRIGHT_INPUT_COMMAND_HANDLER_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "input/commandmessage.hpp"
#include "input/commandevent.hpp"
#include "input/globalstructs.hpp"
#include "base/serveractor.hpp"
#include "base/clientcontroller.hpp"
#include "base/actorcreator.hpp"

namespace bright{

  namespace input{


class CommandHandler{

public:
  CommandHandler(std::shared_ptr<bright::base::ServerActor> pClientActor, std::shared_ptr<bright::base::ClientController> pClientController,
                 std::shared_ptr<bright::base::ActorCreator> pActorCreator);
  CommandHandler();

  void handle_command(std::shared_ptr<bright::input::CommandEvent> pCommandEvent);
  void execute_next_command();
  bool more_commands();
  CommandMessage get_next_command();
  void remove_one_command();
  void update();


private:


  std::shared_ptr<bright::base::ServerActor> pClientActor_;
  std::shared_ptr<bright::base::ClientController> pClientController_;

  std::shared_ptr<bright::base::ActorCreator> pActorCreator_;


  std::vector<CommandMessage> commands_;
  bright::input::ControlState controlState_;
  std::map<std::string, bool> states_;

  void udpate_player_controller();

};



  }

}



#endif



