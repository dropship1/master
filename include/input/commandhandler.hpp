#ifndef BRIGHT_INPUT_COMMANDHANDLER_H
#define BRIGHT_INPUT_COMMANDHANDLER_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "input/commandmessage.hpp"
#include "input/commandevent.hpp"
#include "input/globalstructs.hpp"

namespace bright{

  namespace input{


class CommandHandler{

public:
  CommandHandler();

  void handle_command(std::shared_ptr<bright::input::CommandEvent> pCommandEvent);
  void execute_next_command();
  bool more_commands();
  CommandMessage get_next_command();
  void remove_one_command();
  void update();


private:


  std::vector<CommandMessage> commands_;
  bright::input::ControlState controlState_;
  std::map<std::string, bool> states_;

};



  }

}



#endif



