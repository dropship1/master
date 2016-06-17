#include "network/clientconnection.hpp"

using namespace bright::network;


ClientConnection::ClientConnection( boost::asio::io_service& ioService, 
                                    std::vector<boost::shared_ptr<ClientConnection>>& clientConnections,
                                    std::map<std::string, std::shared_ptr<bright::base::ActorControlController>>& clientActors,
                                    std::mutex& commandMessagesMutex,
                                    std::shared_ptr<bright::converters::AABBConverter> pAABBConverter) : 
  sock_(ioService), started_(false), clientHandler_(clientActors),
  clientConnections_(clientConnections), commandMessagesMutex_(commandMessagesMutex), ioService_(ioService){

}

void ClientConnection::start(){
  //std::cout << "Server Client Connection Start:" << std::endl << std::flush;
  boost::asio::ip::tcp::no_delay option(true);
  sock_.set_option(option);  
  started_ = true;
  std::lock_guard<std::mutex> lock(commandMessagesMutex_);
  clientConnections_.push_back( shared_from_this() );
  do_read();
}


void ClientConnection::stop(){
  //std::cout << "Server Client Connection Stop:" << std::endl << std::flush;
  if (!started_) return;
  started_ = false;
  sock_.close();
  std::lock_guard<std::mutex> lock(commandMessagesMutex_);
  auto it = std::find(clientConnections_.begin(), clientConnections_.end(), shared_from_this());
  if ( it != clientConnections_.end() ){
    clientConnections_.erase(it);
  }
}


bool ClientConnection::started() const{ 
  return started_; 
}

void ClientConnection::handler_send_login_response(){
  if( clientHandler_.need_to_send_login_response() ){
    do_write(clientHandler_.get_login_response());
    clientHandler_.need_to_send_login_response(false);
  }
}

void ClientConnection::handler_send_update_responses(){
  if ( !clientHandler_.is_logged_in() ) { return; }
  if ( !clientHandler_.any_updates() ){ return; }
  send_updates();
}

void ClientConnection::send_other_client_update(std::string msg){
  strcpy(writeOtherBuffer_, msg.c_str());
  async_write(sock_, boost::asio::buffer(writeOtherBuffer_, msg.size()), MEM_FN2(on_write_other_update,_1,_2));
}


void ClientConnection::on_write_other_update(const boost::system::error_code &err, size_t bytes){
  writeOtherBuffer_[0] = 0;
}


void ClientConnection::handle_send_monster_updates(std::vector<std::string> msgs){
  if ( !clientHandler_.is_logged_in() ) { return; }
  monsterMsgs_ = msgs;
  send_monster_updates();
}

void ClientConnection::send_monster_updates(){
  //std::cout << "Server Client Connection Do Write:" << std::endl << std::flush;
  std::string msg = monsterMsgs_[0];
  monsterMsgs_.erase( monsterMsgs_.begin() );
  strcpy(writeMonsterBuffer_, msg.c_str());
  async_write(sock_, boost::asio::buffer(writeMonsterBuffer_, msg.size()), MEM_FN2(on_write_monster_update,_1,_2));
}


void ClientConnection::on_write_monster_update(const boost::system::error_code &err, size_t bytes){
  writeMonsterBuffer_[0] = 0;
  if ( monsterMsgs_.empty() ){ return; }
  send_monster_updates();
}


std::string ClientConnection::username(){
  return clientHandler_.username();
}


void ClientConnection::send_updates(){
  //std::cout << "Server Client Connection Do Write:" << std::endl << std::flush;

  UpdateMessage updateResponse = clientHandler_.get_one_update();
  clientHandler_.clear_one_update();
  std::string msg = updateResponse.full_message();
  auto client_send_responses = [&] (boost::shared_ptr<ClientConnection> pClientConnection) { 
    if ( pClientConnection->username().compare(username()) == 0 ){}
    else { pClientConnection->send_other_client_update(msg); }
  };
  std::for_each(clientConnections_.begin(), clientConnections_.end(), client_send_responses);  
  strcpy(writeUpdateBuffer_, msg.c_str());
  async_write(sock_, boost::asio::buffer(writeUpdateBuffer_, msg.size()), MEM_FN2(on_write_update,_1,_2));
}


void ClientConnection::on_write_update(const boost::system::error_code &err, size_t bytes){
  //std::cout << "Server Client Connection On Write:" << std::endl << std::flush;
  writeUpdateBuffer_[0] = 0;

  if ( !clientHandler_.any_updates() ){ return; }
  send_updates();

}


boost::asio::ip::tcp::socket& ClientConnection::sock(){
  //std::cout << "Server Client Connection Sock:" << std::endl << std::flush;
  return sock_;
}


void ClientConnection::do_write(const std::string & msg){
  //std::cout << "Server Client Connection Do Write:" << std::endl << std::flush;
  strcpy(writeBuffer_, msg.c_str());
  async_write(sock_, boost::asio::buffer(writeBuffer_, msg.size()), MEM_FN2(on_write,_1,_2));
}


void ClientConnection::on_write(const boost::system::error_code &err, size_t bytes){
  //std::cout << "Server Client Connection On Write:" << std::endl << std::flush;
  writeBuffer_[0] = 0;
}


void ClientConnection::do_read(){
  //std::cout << "Server Client Connection Do Read:" << std::endl << std::flush;
  boost::asio::async_read(sock_, boost::asio::buffer(readBuffer_), MEM_FN2(read_complete,_1,_2), MEM_FN2(on_read,_1,_2));
}


void ClientConnection::on_read(const boost::system::error_code &err, size_t bytes){
  //std::cout << "Server Client Connection On Read:" << std::endl << std::flush;
  if (err) stop();
  std::string msg(readBuffer_, bytes);
  readBuffer_[0] = 0;
  
  std::size_t firstSpace = msg.find(" ");
  if (firstSpace == std::string::npos){
    return;
  }

  std::string type = msg.substr(0,firstSpace);
  auto it = MessageTypeDecoder.find(type);
  if ( it == MessageTypeDecoder.end() ){
    return;
  }

  MessageType msgType = it->second;
  std::string data = msg.substr(firstSpace);

  auto netMessage = std::make_shared<NetworkMessage>(data, msgType);

  clientHandler_.add_message(netMessage);

  do_read();
}


size_t ClientConnection::read_complete(const boost::system::error_code & err, size_t bytes){
  //std::cout << "Server Client Connection Read Complete:" << std::endl << std::flush;
  if ( err) return 0;
  bool found = std::find(readBuffer_, readBuffer_ + bytes, '\n') < readBuffer_ + bytes;
  //We read one-by-one until we get to enter, no buffering
  return found ? 0 : 1;
}

void ClientConnection::handler_process_messages(){
  clientHandler_.process_messages();
}

void ClientConnection::process_messages(){
  ioService_.post(MEM_FN(handler_process_messages));
}


void ClientConnection::send_login_response(){
  ioService_.post(MEM_FN(handler_send_login_response));
}

void ClientConnection::send_update_responses(){
  ioService_.post(MEM_FN(handler_send_update_responses));
}

void ClientConnection::send_monster_updates(std::vector<std::string> msgs){
  if ( msgs.empty() ){ return; }
  ioService_.post(MEM_FN1(handle_send_monster_updates, msgs));
}
