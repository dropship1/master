#ifndef BRIGHT_INPUT_COMMAND_LISTENER_H
#define BRIGHT_INPUT_COMMAND_LISTENER_H

#include "input/commandeventlistener.hpp"
#include "input/commandevent.hpp"
#include "input/globalstructs.hpp"
#include "input/commandHandler.hpp"


namespace bright{

  namespace input{

class CommandListener : public CommandEventListener{

public:
  CommandListener();

  void update_commands();

  void process_messages();
  void on_command_event(std::shared_ptr<bright::input::CommandEvent> commandEvent);

private:

  void execute_commands();

  void process();

  void add_command(std::shared_ptr<bright::input::CommandEvent> commandEvent);


private:

  bright::input::CommandHandler commandHandler_;

};


  }

}


#endif












