#include "network/asyncserver.hpp"
#include <boost/make_shared.hpp>
#include <ctime>

#include "utils/fileworker.hpp"
#include "converters/aabbconverter.hpp"
#include "base/actorcontrolsresourcemanager.hpp"

void handle_npcs(std::map<std::string, bright::base::ActorControlController>& npcControllers, std::vector<std::string>& npcResponses, bool left);


int main(int argc, char* argv[]) {

  std::vector<std::string> npcResponses;

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("data/files.fl");
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();

  auto pActorControlResourceManager = std::make_shared<bright::base::ActorControlsResourceManager>(pFileWorker);
  pActorControlResourceManager->initialize();

  auto& npcControllers = pActorControlResourceManager->npc_controllers();
  auto& playerControllers = pActorControlResourceManager->player_controllers();


  auto pAsyncServer = boost::make_shared<bright::network::AsyncServer>(playerControllers);
  pAsyncServer->start();
  boost::thread thread(boost::bind(&boost::asio::io_service::run, &pAsyncServer->service()));

  bool left = true;
  int count = 0;
  clock_t begin = clock();
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  while(true){
    pAsyncServer->process_messages();
    
    pAsyncServer->send_login_responses();
    
    pAsyncServer->send_update_responses();

    if (elapsed_secs > 0.03){
      begin = clock();
      if (count < 100){ left = true; }
      if (count > 100){ left = false; }
      if (count > 200){ count = 0; }
      
      ++count;
      handle_npcs(npcControllers, npcResponses, left);
      pAsyncServer->send_npc_updates(npcResponses);
      npcResponses.clear();
    }

    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  }

}


void handle_npcs(std::map<std::string, bright::base::ActorControlController>& npcControllers, std::vector<std::string>& npcResponses, bool left){

  auto make_a_npc_response = [&] (std::map<std::string, bright::base::ActorControlController>::value_type& pair) { 
    std::string npcName = pair.first;
    auto& npcController = pair.second;
    if (left){  npcController.move_left(0.2f); }
    else{  npcController.move_right(0.2f); }
    bright::network::UpdateMessage updateMsg(npcName, npcController.pos(), npcController.rotation(), false);
    npcResponses.push_back( updateMsg.full_message() );
  };
  std::for_each(npcControllers.begin(), npcControllers.end(), make_a_npc_response);

}