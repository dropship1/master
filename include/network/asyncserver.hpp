#ifndef BRIGHT_NETWORK_ASYNCSERVER_H
#define BRIGHT_NETWORK_ASYNCSERVER_H

#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include <string>
#include <mutex>
#include <vector>

#include "network/clientconnection.hpp"
#include "base/actorcontrolcontroller.hpp"
#include "converters/aabbconverter.hpp"

namespace bright{

  namespace network{


class AsyncServer : public boost::enable_shared_from_this<AsyncServer> {

public:
  AsyncServer(std::map<std::string, bright::base::ActorControlController>& playerControllers);

  void start();
  void process_messages();
  boost::asio::io_service& service();
  void send_login_responses();
  void send_update_responses();
  void send_npc_updates(std::vector<std::string> npcResponses);
  std::mutex commandMessagesMutex_;

private:

  void handle_accept(boost::shared_ptr<ClientConnection> clientConnection, const boost::system::error_code &err);

  boost::asio::io_service service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::vector<boost::shared_ptr<ClientConnection>> clientConnections_;
  std::map<std::string, bright::base::ActorControlController>& playerControllers_;


};



  }

}



#endif



