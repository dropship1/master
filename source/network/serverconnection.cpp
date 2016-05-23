#include "network/serverconnection.hpp"

using namespace bright::network;

ServerConnection::ServerConnection(boost::asio::io_service& service, std::shared_ptr<ServerHandler> pServerHandler) : 
 sock_(service), connected_(false), pServerHandler_(pServerHandler), commandHandler_(), service_(service){
}


void ServerConnection::start(boost::asio::ip::tcp::endpoint ep) {
  //std::cout << "ServerConnection Start: Trying to connect" << std::endl << std::flush;
  sock_.async_connect(ep, MEM_FN1(on_connect,_1));
}


void ServerConnection::on_connect(const boost::system::error_code & err) {
  //std::cout << "ServerConnection On Connect:" << std::endl << std::flush;
  if (err) {
    stop();
  }
  connected_ = true;
  start_reading();
}  

void ServerConnection::stop() {
  //std::cout << "ServerConnection Stop:" << std::endl << std::flush;
  if (!connected_) return;
  connected_ = false;
  sock_.close();
}


bool ServerConnection::connected() { 
  return connected_; 
}

bool ServerConnection::logged_in() { 
  return pServerHandler_->is_logged_in(); 
}

void ServerConnection::login(){
  //std::cout << "ServerHandler Login:" << std::endl << std::flush;
  do_write(pServerHandler_->get_login_message());
}


void ServerConnection::on_command_event(std::shared_ptr<bright::input::CommandEvent> pCommandEvent){
  if( !logged_in() ) { return; }
  service_.post(MEM_FN1(add_command,pCommandEvent));
}

void ServerConnection::add_command(std::shared_ptr<bright::input::CommandEvent> pCommandEvent){
  commandHandler_.handle_command(pCommandEvent);
}

void ServerConnection::send_commands() {
  service_.post(MEM_FN(update_commands));
  service_.post(MEM_FN(write_commands));
}

void ServerConnection::update_commands() {
  commandHandler_.update();
}

void ServerConnection::write_commands() {
  //std::cout << "ServerConnection Do Write:" << std::endl << std::flush;
  if( !commandHandler_.more_commands() ){ return; }
  std::string message = commandHandler_.get_next_command().message();
  commandHandler_.remove_one_command();
  
  strcpy(writeCommandBuffer_, message.c_str());
  size_t size = message.size();
  async_write(sock_, boost::asio::buffer(writeCommandBuffer_, size), MEM_FN2(on_write_command,_1,_2));
}


void ServerConnection::on_write_command(const boost::system::error_code & err, size_t bytes) {
  //std::cout << "ServerConnection On Write:" << std::endl << std::flush;
  writeCommandBuffer_[0] = 0;

  if (err) {
    stop();
  }

  if ( commandHandler_.more_commands() ) {
    write_commands();
  }

}


void ServerConnection::process() {
  pServerHandler_->process_messages();
}


void ServerConnection::process_messages() {
  service_.post(MEM_FN(process));
}


void ServerConnection::do_write(const std::string & msg) {
  //std::cout << "ServerConnection Do Write:" << std::endl << std::flush;
  strcpy(writeBuffer_, msg.c_str());
  async_write(sock_, boost::asio::buffer(writeBuffer_, msg.size()), MEM_FN2(on_write,_1,_2));
}


void ServerConnection::on_write(const boost::system::error_code & err, size_t bytes) {
  //std::cout << "ServerConnection On Write:" << std::endl << std::flush;
  writeBuffer_[0] = 0;
  if (err) {
    stop();
  }
}


void ServerConnection::start_reading() {
  //std::cout << "ServerConnection Do Read:" << std::endl << std::flush;
  do_read();
}


void ServerConnection::do_read() {
  //std::cout << "ServerConnection Do Read:" << std::endl << std::flush;
  async_read(sock_, boost::asio::buffer(readBuffer_), MEM_FN2(read_complete,_1,_2), MEM_FN2(on_read,_1,_2));
}


void ServerConnection::on_read(const boost::system::error_code & err, size_t bytes) {
  //std::cout << "ServerConnection On Read:" << std::endl << std::flush;
  if (err) { stop(); }
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

  pServerHandler_->add_message(netMessage);

  do_read();
}


size_t ServerConnection::read_complete(const boost::system::error_code & err, size_t bytes) {
  //std::cout << "ServerConnection Read Complete:" << std::endl << std::flush;
  if ( err) return 0;
  bool found = std::find(readBuffer_, readBuffer_ + bytes, '\n') < readBuffer_ + bytes;
  //we read one-by-one until we get to enter, no buffering
  return found ? 0 : 1;
}





























