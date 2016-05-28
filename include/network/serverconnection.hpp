#ifndef BRIGHT_NETWORK_SERVER_CONNECTION_H
#define BRIGHT_NETWORK_SERVER_CONNECTION_H

#ifdef WIN32
#define _WIN32_WINNT 0x0501 // Windows XP
#include <stdio.h>
#endif

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <glm/glm.hpp>
#include <mutex>

#include "input/commandmessage.hpp"
#include "input/commandhandler.hpp"
#include "input/commandeventlistener.hpp"
#include "input/commandevent.hpp"

#include "network/globalstructs.hpp"
#include "network/networkmessage.hpp"

#include "network/serverhandler.hpp"

#include <boost/shared_ptr.hpp>

#define MEM_FN(x)       boost::bind(&ServerConnection::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&ServerConnection::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&ServerConnection::x, shared_from_this(),y,z)

namespace bright{

  namespace network{

/** simple connection to server:
    - logs in just with username (no password)
    - all connections are initiated by the client: client asks, server answers

    Possible requests:
*/
class ServerConnection : public boost::enable_shared_from_this<ServerConnection>, 
  public bright::input::CommandEventListener {

public:
  ServerConnection(boost::asio::io_service& service, std::shared_ptr<ServerHandler> pServerHandler);

  void start(boost::asio::ip::tcp::endpoint ep);
  void login();
  void stop();
  bool logged_in();
  bool connected();

  void start_reading();
  void send_commands(); 
  void update_commands();

  void process_messages();
  void on_command_event(std::shared_ptr<bright::input::CommandEvent> commandEvent);

private:

  void write_commands();

  void process();

  void add_command(std::shared_ptr<bright::input::CommandEvent> commandEvent);

  void on_connect(const boost::system::error_code & err);

  void do_write(const std::string & msg);
  void on_write(const boost::system::error_code & err, size_t bytes);

  void on_write_command(const boost::system::error_code & err, size_t bytes);

  void do_read();
  void on_read(const boost::system::error_code & err, size_t bytes);
  size_t read_complete(const boost::system::error_code & err, size_t bytes);

  void on_wait(const boost::system::error_code & err);


private:

  boost::asio::ip::tcp::socket sock_;
  enum { maxMsg = 1024 };
  char readBuffer_[maxMsg];
  char readBuffer2_[maxMsg];
  char writeBuffer_[maxMsg];
  char writeCommandBuffer_[maxMsg];
  bool connected_;

  bright::input::CommandHandler commandHandler_;
  boost::asio::io_service& service_;

  std::shared_ptr<ServerHandler> pServerHandler_;

};


  }

}


#endif












