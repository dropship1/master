#include "input/commandmessage.hpp"

using namespace bright::input;


CommandMessage::CommandMessage():
  controlName_("null_control"), commandName_("null_name"), controlType_("null_type"), data_("null_data"){
}

CommandMessage::CommandMessage(std::string controlName, std::string commandName, std::string controlType, std::string data):
  controlName_(controlName), commandName_(commandName), controlType_(controlType), data_(data){
}


const std::string& CommandMessage::control_name(){
  return controlName_;
}


const std::string& CommandMessage::command_name(){
  return commandName_;
}


const std::string& CommandMessage::control_type(){
  return controlType_;
}


const std::string& CommandMessage::data(){
  return data_;
}

std::string CommandMessage::message(){
  return "client_command " + controlName_ + " " + commandName_ + " " + controlType_ + " " + data_ + "\n";
}