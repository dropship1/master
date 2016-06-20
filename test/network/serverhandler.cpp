#include "network/globalstructs.hpp"
#include "network/serverhandler.hpp"
#include "network/networkmessage.hpp"
#include "network/loginmessage.hpp"
#include "input/commandmessage.hpp"

#include "base/actorcontrolcontroller.hpp"

#include <memory>
#include <iostream>
#include <string>


int main(int argc, char* argv[]) {


  //Setup the players controller
  std::map<std::string, bright::base::ActorControlController> playerControllers;
  playerControllers["client1"] = bright::base::ActorControlController();
  auto& playerController = playerControllers["client1"];

  //Setup the monsters controllers
  std::map<std::string, bright::base::ActorControlController> npcControllers;
  npcControllers["npc1"] = bright::base::ActorControlController();
  auto& npc1Controller = npcControllers["npc1"];

  //Create the serverhandler
  auto pServerHandler = std::make_shared<bright::network::ServerHandler>("client1", "mypassword", playerControllers, npcControllers);

  //Create some fake network messages events for the server handler to process
  auto pNetworkMessage1 = std::make_shared<bright::network::NetworkMessage>("client1 true 6 8 3 1 0 0", bright::network::MessageType::SERVER_UPDATE_RESPONSE);
  pServerHandler->add_message(pNetworkMessage1);
  std::cout << "Did serverhandler store the network Message properly?" << std::flush << std::endl;
  std::cout << "No because we're not logged into the server yet." << std::flush << std::endl;
  std::cout << "client1 logged in? " << ( (pServerHandler->is_logged_in() ) ? "Yes" : "No") << std::flush << std::endl;
  std::cout << std::flush << std::endl;  

  //Lets create a fake serverLoginResponse message
  auto pNetworkMessage2 = std::make_shared<bright::network::NetworkMessage>("true client1-logged-in-ok", bright::network::MessageType::SERVER_LOGIN_RESPONSE);
  pServerHandler->add_message(pNetworkMessage2);
  std::cout << "Did serverhandler store the login response Message properly?" << std::flush << std::endl;
  std::cout << "Yes, and now we should have a loginResponseMessage stored" << std::flush << std::endl;
  std::cout << "But until we process messages, the client is still not logged in" << std::flush << std::endl;
  std::cout << "client1 logged in? " << ( (pServerHandler->is_logged_in() ) ? "Yes" : "No") << std::flush << std::endl;
  std::cout << std::flush << std::endl;  

  pServerHandler->process_messages();
  std::cout << "Did serverhandler get logged in now?" << std::flush << std::endl;
  std::cout << "Yes, because we processed our loginResponse message that said we were logged in now" << std::flush << std::endl;
  std::cout << "client1 logged in? " << ( (pServerHandler->is_logged_in() ) ? "Yes" : "No") << std::flush << std::endl;
  std::cout << std::flush << std::endl;

  //Now we can start accepting update messages since we're logged into the server
  pServerHandler->add_message(pNetworkMessage1);
  std::cout << "Did serverhandler store the network Message properly?" << std::flush << std::endl;
  std::cout << "Yes because we were logged in to the server now" << std::flush << std::endl;
  std::cout << "client1 logged in? " << ( (pServerHandler->is_logged_in() ) ? "Yes" : "No") << std::flush << std::endl;
  std::cout << std::flush << std::endl;

  //Lets see if processing the message actually moves the player controller
  std::cout << "PlayerController before" << std::flush << std::endl;
  std::cout << "position:" << playerController.pos().x << " : " << playerController.pos().y << " : " << playerController.pos().z << std::flush << std::endl;
  std::cout << std::flush << std::endl;

  pServerHandler->process_messages();
  std::cout << "PlayerController after" << std::flush << std::endl;
  std::cout << "position:" << playerController.pos().x << " : " << playerController.pos().y << " : " << playerController.pos().z << std::flush << std::endl;
  std::cout << std::flush << std::endl;

  //Create some fake network messages events for the server handler to process to move the monster
  auto pNetworkMessage3 = std::make_shared<bright::network::NetworkMessage>("npc1 false 2 5 3 1 0 0", bright::network::MessageType::SERVER_UPDATE_RESPONSE);
  pServerHandler->add_message(pNetworkMessage3);
  std::cout << "Did serverhandler store the network Message properly?" << std::flush << std::endl;
  std::cout << "It should have" << std::flush << std::endl;
  std::cout << std::flush << std::endl;

  //Lets see if processing the message actually moves the monster controller
  std::cout << "NpcController before" << std::flush << std::endl;
  std::cout << "position:" << npcControllers["npc1"].pos().x << " : " << npcControllers["npc1"].pos().y << " : " << npcControllers["npc1"].pos().z << std::flush << std::endl;
  std::cout << std::flush << std::endl;

  pServerHandler->process_messages();
  std::cout << "NpcController after" << std::flush << std::endl;
  std::cout << "position:" << npcControllers["npc1"].pos().x << " : " << npcControllers["npc1"].pos().y << " : " << npcControllers["npc1"].pos().z << std::flush << std::endl;
  std::cout << std::flush << std::endl;


  std::cout << "ctr+c to exit or x on the window" << std::endl;
  while (true){ }
}