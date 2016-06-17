#ifndef BRIGHT_NETWORK_CLIENTCONNECTION_H
#define BRIGHT_NETWORK_CLIENTCONNECTION_H

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
#include <string>
#include <mutex>

#include "network/clienthandler.hpp"
#include "base/actorcontrolcontroller.hpp"
#include "converters/aabbconverter.hpp"

namespace bright{

  namespace network{


#define MEM_FN(x)       boost::bind(&ClientConnection::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&ClientConnection::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&ClientConnection::x, shared_from_this(),y,z)


class ClientConnection : public boost::enable_shared_from_this<ClientConnection> {

public:
  ClientConnection( boost::asio::io_service& ioService, 
                    std::vector<boost::shared_ptr<ClientConnection>>& clientConnections,
                    std::map<std::string, std::shared_ptr<bright::base::ActorControlController>>& clientActors, std::mutex& commandMessagesMutex,
                    std::shared_ptr<bright::converters::AABBConverter> pAABBConverter);

  void start();
  void stop();
  bool started() const;
  void process_messages();

  void send_login_response();
  void send_update_responses();
  void send_other_client_update(std::string msg);
  void send_monster_updates(std::vector<std::string> msgs);
  boost::asio::ip::tcp::socket& sock();

  std::string username();

private:

  void do_write(const std::string & msg);
  void on_write(const boost::system::error_code &err, size_t bytes);

  void send_updates();
  void on_write_update(const boost::system::error_code &err, size_t bytes);
  void on_write_other_update(const boost::system::error_code &err, size_t bytes);
  void send_monster_updates();
  void on_write_monster_update(const boost::system::error_code &err, size_t bytes);

  void do_read();
  void on_read(const boost::system::error_code &err, size_t bytes);
  size_t read_complete(const boost::system::error_code & err, size_t bytes);

  boost::asio::ip::tcp::socket sock_;

  void handler_process_messages();
  void handler_send_update_responses();
  void handler_send_login_response();
  void handle_send_monster_updates(std::vector<std::string> msgs);

  enum { maxMsg = 1024 };
  char readBuffer_[maxMsg];
  char writeBuffer_[maxMsg];
  char writeOtherBuffer_[maxMsg];
  char writeMonsterBuffer_[maxMsg];
  char writeUpdateBuffer_[maxMsg];

  bool started_;
  std::vector<boost::shared_ptr<ClientConnection>>& clientConnections_;

  boost::asio::io_service& ioService_;

  ClientHandler clientHandler_;
  std::mutex& commandMessagesMutex_;

  std::vector<std::string> monsterMsgs_;


};



  }

}



#endif



