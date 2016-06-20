#include "input/commandlistener.hpp"

using namespace bright::input;

CommandListener::CommandListener(bright::base::ActorControlController& actorControlController, bright::base::ActorRenderController& actorRenderController,
                                 std::shared_ptr<bright::base::ActorCreator> pActorCreator): commandHandler_(actorControlController,actorRenderController,pActorCreator){
}

void CommandListener::on_command_event(std::shared_ptr<bright::input::CommandEvent> pCommandEvent){
  commandHandler_.handle_command(pCommandEvent);
}

void CommandListener::update_commands() {
  commandHandler_.update();
}

void CommandListener::execute_commands() {
  //std::cout << "ServerConnection Do Write:" << std::endl << std::flush;
  if( !commandHandler_.more_commands() ){ return; }
  while ( commandHandler_.more_commands() ) {
    commandHandler_.execute_next_command();
  }
}




























