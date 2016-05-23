#include "network/loginresponsemessage.hpp"

using namespace bright::network;


LoginResponseMessage::LoginResponseMessage(bool loggedIn, std::string message): 
  loggedIn_(loggedIn), message_(message){
}

LoginResponseMessage::LoginResponseMessage(): 
  loggedIn_(false), message_(""){
}

bool LoginResponseMessage::logged_in(){
  return loggedIn_;
}

std::string& LoginResponseMessage::message(){
  return message_;
}


const std::string& LoginResponseMessage::full_response(){
  std::string loggedIn = "false";
  if(loggedIn_){
    loggedIn = "true";
  }
  fullResponse_ = "svr_login_rsp " + loggedIn + " " + message_ + "\n";
  return fullResponse_;
}
  