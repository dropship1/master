#include "input/commandListener.hpp"

using namespace bright::input;

CommandListener::CommandListener(std::shared_ptr<bright::base::ServerActor> pClientActor, std::shared_ptr<bright::base::ClientController> pClientController): commandHandler_(pClientActor,pClientController){
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




























