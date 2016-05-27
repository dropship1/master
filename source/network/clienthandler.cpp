#include "network/clienthandler.hpp"

using namespace bright::network;


ClientHandler::ClientHandler(std::map<std::string, std::shared_ptr<bright::base::ServerActor>>& clientActors, std::shared_ptr<bright::converters::AABBConverter> pAABBConverter):
  clientActors_(clientActors), haveLoginMessage_(false), needToSendLoginResponse_(false), needToSendUpdateResponse_(false), pAABBConverter_(pAABBConverter) {
  clientActor_ = std::make_shared<bright::base::ServerActor>();
}

std::string ClientHandler::username(){
  return loginMessage_.username();
}


std::string ClientHandler::password(){
  return loginMessage_.password();
}


bool ClientHandler::is_logged_in(){
  return clientActor_->is_logged_in();
}

void ClientHandler::add_message(std::shared_ptr<NetworkMessage> networkMessage){

  if( !is_logged_in() && networkMessage->type() == MessageType::CLIENT_LOGIN_REQUEST ){
    add_login_message(networkMessage);
  }
  else if( is_logged_in() && networkMessage->type() == MessageType::CLIENT_COMMAND){
    add_command_message(networkMessage);
  }

}


void ClientHandler::process_messages(){
  
  if( !is_logged_in()  && haveLoginMessage_){
    handle_login();
  }
  else if( is_logged_in() ){
    //now the client is logged in.. handle all other types of messages
    handle_commands();
  }

}


void ClientHandler::add_login_message(std::shared_ptr<NetworkMessage> networkMessage){

  std::istringstream in( networkMessage->data() );
  std::string username;
  std::string password;
  in >> username;
  in >> password;
  loginMessage_ = LoginMessage(username, password);
  haveLoginMessage_ = true;

}

void ClientHandler::add_command_message(std::shared_ptr<NetworkMessage> networkMessage){

  //create newCommandMessage by parsing the networkMessage's data
  std::istringstream in( networkMessage->data() );
  std::string controlName;
  std::string commandName;
  std::string controlType;
  std::string data;
  in >> controlName;
  in >> commandName;
  in >> controlType;
  in >> data;
  CommandMessage commandMessage(controlName, commandName, controlType, data);
  commands_.push_back(commandMessage);
}


void ClientHandler::handle_login(){

  clientActor_->aabb( pAABBConverter_->aabb("cube") );
  clientActor_->pos(glm::vec3(50.0f, 30.0f, -50.0f));
  clientActor_->rotate_down(55.0f);
  clientActor_->name(username());
  clientActor_->is_logged_in(true);

  loginResponseMessage_ = LoginResponseMessage(true, "login-ok");
  needToSendLoginResponse_ = true;

  clientActors_[username()] = clientActor_;

}


const std::string& ClientHandler::get_login_response(){
  return loginResponseMessage_.full_response();
}

bool ClientHandler::need_to_send_login_response(){
  return needToSendLoginResponse_;
}


void ClientHandler::need_to_send_login_response(bool value){
  needToSendLoginResponse_ = value;
}


UpdateMessage ClientHandler::get_one_update(){
  if( !updateResponses_.empty() ) { 
    return updateResponses_[0];
  }

  return bright::network::UpdateMessage("EMPTYBADNOUSERNAME", glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), false);
}

bool ClientHandler::any_updates(){
  return !updateResponses_.empty();
}


void ClientHandler::clear_one_update(){
  if( updateResponses_.empty() ) { return; }
  updateResponses_.erase( updateResponses_.begin() );
}


void ClientHandler::handle_commands(){

  auto update_controls = [&] (CommandMessage& cmdMessage) { 

    if( cmdMessage.command_name().compare("STATE") == 0 ){

      if( cmdMessage.control_type().compare("STATE_ON")  == 0 ){
        if( cmdMessage.control_name().compare("MOVE_FORWARD") == 0 ){
          clientActor_->move_fwd(0.2f);
          add_update_response();
        }
        else if( cmdMessage.control_name().compare("MOVE_BACK") == 0 ){
          clientActor_->move_backward(5.2f);
          add_update_response();
        }
        else if( cmdMessage.control_name().compare("MOVE_LEFT") == 0 ){
          clientActor_->move_left(0.2f);  
          add_update_response();
        }
        else if( cmdMessage.control_name().compare("MOVE_RIGHT") == 0 ){
          clientActor_->move_right(0.2f);
          add_update_response();
        }

      }
      else if( cmdMessage.control_type().compare("STATE_OFF") == 0 ){

        if( cmdMessage.control_name().compare("MOVE_FORWARD") == 0 ){
        }
        else if( cmdMessage.control_name().compare("MOVE_BACK") == 0 ){
        }
        else if( cmdMessage.control_name().compare("MOVE_LEFT") == 0 ){
        }
        else if( cmdMessage.control_name().compare("MOVE_RIGHT") == 0 ){
        }

      }

    }
    else if( cmdMessage.command_name().compare("AXIS") == 0 ){
      std::string::size_type sz;
      int amount = std::stoi (cmdMessage.control_type(),&sz);

      if( amount > 0 ){
        if( cmdMessage.control_name().compare("CAMERA_X_AXIS") == 0 ){
          clientActor_->rotate_right(0.01f);
          add_update_response();
        }
        else if( cmdMessage.control_name().compare("CAMERA_Y_AXIS") == 0 ){
          clientActor_->rotate_down(0.01f);
          add_update_response();
        }
      }
      else if(  amount < 0  ){
        if( cmdMessage.control_name().compare("CAMERA_X_AXIS") == 0 ){
          clientActor_->rotate_left(0.01f);
          add_update_response();
        }
        else if( cmdMessage.control_name().compare("CAMERA_Y_AXIS") == 0 ){
          clientActor_->rotate_up(0.01f);
          add_update_response();
        }
      }

    }

  };
  std::for_each(commands_.begin(), commands_.end(), update_controls);

  //done with commands, empty them so we can have fresh ones next time
  empty_commands();
}


void ClientHandler::empty_commands(){
  commands_.clear();
}


void ClientHandler::add_update_response(){
  updateResponses_.push_back( UpdateMessage(username(), clientActor_->pos(), clientActor_->right(), clientActor_->up(), clientActor_->look(), true) );
}



