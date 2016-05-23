#include "input/commandevent.hpp"
#include <boost/algorithm/string/join.hpp>

using namespace bright::input;

CommandEvent::CommandEvent(std::shared_ptr<std::vector<std::string>> command) :
  command_(command) {
      for(std::vector<std::string>::size_type i = 0; i != command->size(); i++) {
		  if (i > 0) {
			  commandString_ += " ";
		  }
		  commandString_ += command->at(i);
      }
}

std::string CommandEvent::command_string(){
  return commandString_;
}
