#ifndef BRIGHT_NETWORK_SERVERHANDLER_H
#define BRIGHT_NETWORK_SERVERHANDLER_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <memory>
#include <algorithm>

#include "base/actorcontrolcontroller.hpp"
#include "network/networkmessage.hpp"
#include "network/loginmessage.hpp"
#include "network/loginresponsemessage.hpp"
#include "network/updatemessage.hpp"


#include <mutex>

namespace bright{

  namespace network{


class ServerHandler{

public:
  ServerHandler( std::string username, 
                 std::string password, 
                 std::map<std::string, bright::base::ActorControlController>& playerContollers, 
                 std::map<std::string, bright::base::ActorControlController>& npcContollers);

  std::string username(){ return username_; }
  std::string password(){ return password_; }

  void process_messages();

  bool is_logged_in(){ return isLoggedIn_; }

  void add_message(std::shared_ptr<NetworkMessage> pNetMessage);

  std::string& get_login_message();

private:

  void empty_updates();

  void add_login_response_message(std::shared_ptr<NetworkMessage> pNetworkMessage);
  void add_update_message(std::shared_ptr<NetworkMessage> pNetworkMessage);

  void handle_login_response();
  void handle_update_responses();

  bool isLoggedIn_;
  std::string username_;
  std::string password_;

  std::vector<UpdateMessage> updateMessages_;

  LoginMessage loginMessage_;
  std::string loginReqestMessage_;

  LoginResponseMessage loginResponseMessage_;

  std::mutex updateMessagesMutex_;

  std::map<std::string, bright::base::ActorControlController>& playerControllers_;
  std::map<std::string, bright::base::ActorControlController>& npcContollers_;

};



  }

}



#endif



