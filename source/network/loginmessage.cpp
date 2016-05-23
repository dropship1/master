#include "network/loginmessage.hpp"

using namespace bright::network;


LoginMessage::LoginMessage(std::string username, std::string password): 
  username_(username), password_(password){
}

LoginMessage::LoginMessage():
  username_(""), password_(""){
}

const std::string& LoginMessage::username(){
  return username_;
}

const std::string& LoginMessage::password(){
  return password_;
}

std::string LoginMessage::request_message(){
  return "client_login " + username_ + " " + password_ + "\n";
}