#include "network/asyncserver.hpp"

using namespace bright::network;

void AsyncServer::handle_accept(boost::shared_ptr<ClientConnection> clientConnection, const boost::system::error_code & err) {
  clientConnection->start();
  auto newClientConnection = boost::make_shared<ClientConnection>(service_, clientConnections_, clientActors_, commandMessagesMutex_, pAABBConverter_);
  std::cout << "Server Handle Accept: Waiting to accept new client" << std::endl << std::flush;
  acceptor_.async_accept( newClientConnection->sock(), boost::bind(&AsyncServer::handle_accept, shared_from_this(), newClientConnection,_1) );
}

AsyncServer::AsyncServer(std::map<std::string, std::shared_ptr<bright::base::ServerActor>>& clientActors, std::shared_ptr<bright::converters::AABBConverter> pAABBConverter): 
  pAABBConverter_(pAABBConverter), service_(), acceptor_(service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8001)), clientActors_(clientActors){
}


void AsyncServer::start(){
  std::cout << "Server Start: Waiting to accept new client" << std::endl << std::flush;
  auto newClientConnection = boost::make_shared<ClientConnection>(service_, clientConnections_, clientActors_, commandMessagesMutex_, pAABBConverter_);
  acceptor_.async_accept( newClientConnection->sock(), boost::bind(&AsyncServer::handle_accept, shared_from_this(), newClientConnection, _1) );
  //std::cout << "Server Start: After waiting to accept new client" << std::endl << std::flush;

  //std::cout << "Server Start: After service run" << std::endl << std::cout;
}


boost::asio::io_service& AsyncServer::service(){
  return service_;
}

void AsyncServer::process_messages(){
  std::lock_guard<std::mutex> lock(commandMessagesMutex_);
  auto client_process_messages = [&] (boost::shared_ptr<ClientConnection> pClientConnection) { 
    pClientConnection->process_messages();
  };
  std::for_each(clientConnections_.begin(), clientConnections_.end(), client_process_messages);
}


void AsyncServer::send_login_responses(){
  std::lock_guard<std::mutex> lock(commandMessagesMutex_);
  auto client_send_responses = [&] (boost::shared_ptr<ClientConnection> pClientConnection) { 
    pClientConnection->send_login_response();
  };
  std::for_each(clientConnections_.begin(), clientConnections_.end(), client_send_responses);

}


void AsyncServer::send_update_responses(){
  std::lock_guard<std::mutex> lock(commandMessagesMutex_);
  auto client_send_responses = [&] (boost::shared_ptr<ClientConnection> pClientConnection) { 
    pClientConnection->send_update_responses();
  };
  std::for_each(clientConnections_.begin(), clientConnections_.end(), client_send_responses);

}


void AsyncServer::send_monster_responses(std::vector<std::string> monsterResponses){
  std::lock_guard<std::mutex> lock(commandMessagesMutex_);
  auto client_send_monster_response = [&] (boost::shared_ptr<ClientConnection> pClientConnection) { 
    pClientConnection->send_monster_updates(monsterResponses);
  };
  std::for_each(clientConnections_.begin(), clientConnections_.end(), client_send_monster_response);
}

