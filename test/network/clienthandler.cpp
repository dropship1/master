#include "network/globalstructs.hpp"
#include "network/clienthandler.hpp"
#include "network/networkmessage.hpp"
#include "network/loginmessage.hpp"
#include "input/commandmessage.hpp"

#include "base/serveractor.hpp"

#include "utils/fileworker.hpp"
#include "converters/aabbconverter.hpp"

#include <memory>
#include <iostream>
#include <string>


int main(int argc, char* argv[]) {

  std::map<std::string, std::shared_ptr<bright::base::ServerActor>> clientActors;

  //Add client (as ServerActors) to our clients map
  auto client1 = std::make_shared<bright::base::ServerActor>();
  client1->pos( glm::vec3(5.0f,7.0f,22.0f) );
  client1->rotation( glm::vec3(7.0f,5.0f,3.0f) );
  client1->name("client1");

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("test/network/data/filelist");
  auto pAABBConverter = std::make_shared<bright::converters::AABBConverter>(pFileWorker);
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();

  clientActors[client1->name()] = client1;

  //Create a client handler
  bright::network::ClientHandler client1Handler(clientActors, pAABBConverter);


  //Create some fake login messages for the client handler to process
  auto pNetMessage1 = std::make_shared<bright::network::NetworkMessage>("client1 mypassword",bright::network::MessageType::CLIENT_LOGIN_REQUEST);
  client1Handler.add_message(pNetMessage1);

  std::cout << "Is client logged in?" << std::flush << std::endl;
  std::cout << "client1? " << ((client1Handler.is_logged_in()) ? "Yes" : "No") << std::flush << std::endl;
  std::cout << std::flush << std::endl;

  client1Handler.process_messages();
  std::cout << "Is client logged in?" << std::flush << std::endl;
  std::cout << "After Client1 processed its messages" << std::flush << std::endl;
  std::cout << "client1? " << ((client1Handler.is_logged_in()) ? "Yes" : "No") << std::flush << std::endl;
  std::cout << std::flush << std::endl;

  //Now lets test some commands
  //Create some fake command messages for the client handler to process
  auto pNetMessage2 = std::make_shared<bright::network::NetworkMessage>("MOVE_FORWARD STATE STATE_ON",bright::network::MessageType::CLIENT_COMMAND);
  auto pNetMessage3 = std::make_shared<bright::network::NetworkMessage>("CAMERA_X_AXIS AXIS 1",bright::network::MessageType::CLIENT_COMMAND);
  client1Handler.add_message(pNetMessage2);
  client1Handler.add_message(pNetMessage3);

  //Lets see if we can change client1's position
  //Lets see if we can change client1's rotation
  glm::vec3 client1CurrPos = clientActors[client1->name()]->pos();
  glm::vec3 client1CurrRot = clientActors[client1->name()]->rotation();
  std::cout << "Did clients position change?" << std::flush << std::endl;
  std::cout << "Did clients rotation change?" << std::flush << std::endl;
  std::cout << "Before Client1 processed its messages" << std::flush << std::endl;
  std::cout << "pos " << client1CurrPos.x << ":" << client1CurrPos.y << ":" << client1CurrPos.z << std::flush << std::endl;
  std::cout << "rotation " << client1CurrRot.x << ":" << client1CurrRot.y << ":" << client1CurrRot.z << std::flush << std::endl;
  std::cout << std::flush << std::endl;

  client1Handler.process_messages();
  client1CurrPos = clientActors[client1->name()]->pos();
  client1CurrRot = clientActors[client1->name()]->rotation();
  std::cout << "After Client1 processed its messages" << std::flush << std::endl;
  std::cout << "pos " << client1CurrPos.x << ":" << client1CurrPos.y << ":" << client1CurrPos.z << std::flush << std::endl;
  std::cout << "rotation " << client1CurrRot.x << ":" << client1CurrRot.y << ":" << client1CurrRot.z << std::flush << std::endl;
  std::cout << std::flush << std::endl;

  std::cout << "There should also exist 2 update responses at this point in the clientHandler" << std::flush << std::endl;
  std::cout << "And needToSendUpdates should be true" << std::flush << std::endl;
  std::cout << std::flush << std::endl;

  std::cout << "ctr+c to exit or x on the window" << std::endl;
  while (true){ }
}