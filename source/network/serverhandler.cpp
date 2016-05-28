#include "network/serverhandler.hpp"

using namespace bright::network;


ServerHandler::ServerHandler( std::string username, 
                              std::string password, 
                              std::shared_ptr<bright::base::ClientController> playerController, 
                              std::map<std::string, std::shared_ptr<bright::base::ClientController>>& monsterContollers):
  username_(username), password_(password), isLoggedIn_(false), playerController_(playerController), monsterContollers_(monsterContollers),
  loginMessage_(username_, password_) {
}



std::string& ServerHandler::get_login_message(){
  loginReqestMessage_ = loginMessage_.request_message();
  return loginReqestMessage_;
}


void ServerHandler::add_message(std::shared_ptr<NetworkMessage> pNetworkMessage){

  if( pNetworkMessage->type() == MessageType::SERVER_LOGIN_RESPONSE ){
    add_login_response_message(pNetworkMessage);
  }
  else if( pNetworkMessage->type() == MessageType::SERVER_UPDATE_RESPONSE ){
    add_update_message(pNetworkMessage);
  }

}


void ServerHandler::process_messages(){
  
  if (!isLoggedIn_ && !loginResponseMessage_.logged_in()){
    empty_updates();
    return;
  }
  
  
  if(!isLoggedIn_ && loginResponseMessage_.logged_in()){
    handle_login_response();
    return;
  }
  
  //now we're logged into the server.. handle all other types of messages
  handle_update_responses();

}


void ServerHandler::add_login_response_message(std::shared_ptr<NetworkMessage> pNetworkMessage){
  if(isLoggedIn_) { return; }

  std::istringstream in( pNetworkMessage->data() );
  bool isLoggedIn = false;
  std::string logedIn;
  std::string message;
  in >> logedIn;
  in >> message;
  if (logedIn.compare("true") == 0){
    isLoggedIn = true;
  }

  loginResponseMessage_ = LoginResponseMessage(isLoggedIn, message);
}

void ServerHandler::add_update_message(std::shared_ptr<NetworkMessage> pNetworkMessage){
  if(!isLoggedIn_) { return; }
  //create newUpdateMessage by parsing the networkMessage's data
  std::istringstream in( pNetworkMessage->data() );
  std::string name;
  glm::vec3 position;
  glm::vec3 right;
  glm::vec3 up;
  glm::vec3 look;
  std::string isP;
  bool isPlayer = false;
  in >> name;
  in >> isP;
  in >> position.x;
  in >> position.y;
  in >> position.z;
  in >> right.x;
  in >> right.y;
  in >> right.z;
  in >> up.x;
  in >> up.y;
  in >> up.z;
  in >> look.x;
  in >> look.y;
  in >> look.z;
  if (isP.compare("true") == 0){
    isPlayer = true;
  }
  updateMessages_.push_back( std::make_shared<UpdateMessage>(name, position, right, up, look, isPlayer) );
}


void ServerHandler::handle_login_response(){

  if ( !loginResponseMessage_.logged_in() ){
    return;
  }

  isLoggedIn_ = true;
}


void ServerHandler::handle_update_responses(){

  auto handle_updates = [&] (std::shared_ptr<UpdateMessage> pUpdateMessage) { 
    //what to do with the update message?
    if( pUpdateMessage->is_player() && pUpdateMessage->name().compare(username_) == 0 ){
      //playerController_->update( pUpdateMessage->position(), pUpdateMessage->right(), pUpdateMessage->up(), pUpdateMessage->look() );
    }
    else if( pUpdateMessage->is_player() ){
      auto it = monsterContollers_.find( pUpdateMessage->name() );
      if ( it == monsterContollers_.end() ){
        monsterContollers_[pUpdateMessage->name()] = std::make_shared<bright::base::ClientController>();
      }
      monsterContollers_[pUpdateMessage->name()]->update( pUpdateMessage->position(), pUpdateMessage->right(), pUpdateMessage->up(), pUpdateMessage->look() );
    }
    else{
      auto it = monsterContollers_.find( pUpdateMessage->name() );
      if ( it == monsterContollers_.end() ){
        monsterContollers_[pUpdateMessage->name()] = std::make_shared<bright::base::ClientController>();
      }
      monsterContollers_[pUpdateMessage->name()]->update( pUpdateMessage->position(), pUpdateMessage->right(), pUpdateMessage->up(), pUpdateMessage->look() );
    }
  };
  std::for_each(updateMessages_.begin(), updateMessages_.end(), handle_updates);

  updateMessages_.clear();
}



void ServerHandler::empty_updates(){
  updateMessages_.clear();
}
