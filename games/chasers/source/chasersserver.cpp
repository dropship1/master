#include "network/asyncserver.hpp"
#include "ai/aimanager.hpp"
#include "base/actorcontrolsresourcemanager.hpp"
#include "base/controlactor.hpp"
#include "utils/fileworker.hpp"
#include "converters/aabbconverter.hpp"
#include "network/updatemessage.hpp"
#include "physics/physicsmanager.hpp"

#include <boost/make_shared.hpp>
#include <ctime>

void create_npc_update_responses(std::map<std::string, bright::base::ActorControlController>& npcControllers, std::vector<std::string>& npcResponses);


int main(int argc, char* argv[]) {

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("../data/files.fl");
  //auto pFileWorker = std::make_shared<bright::utils::FileWorker>("games/chasers/data/files.fl");

  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();

  auto pActorControlsResourceManager = std::make_shared<bright::base::ActorControlsResourceManager>(pFileWorker);
  pActorControlsResourceManager->initialize();

  std::vector<std::string> npcResponses;
  auto controlPlayers = pActorControlsResourceManager->control_players();
  auto controlNpcs = pActorControlsResourceManager->control_npcs();
  auto playerControllers = pActorControlsResourceManager->player_controllers();
  auto npcControllers = pActorControlsResourceManager->npc_controllers();
  auto playerAABBs = pActorControlsResourceManager->player_aabbs();
  auto npcAABBs = pActorControlsResourceManager->npc_aabbs();

  bright::ai::AIManager aiManager(pActorControlsResourceManager);
  bright::physics::PhysicsManager physicsManager(pActorControlsResourceManager);

  auto pAsyncServer = boost::make_shared<bright::network::AsyncServer>(playerControllers);
  pAsyncServer->start();
  boost::thread thread(boost::bind(&boost::asio::io_service::run, &pAsyncServer->service()));

  clock_t begin = clock();
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  while(true){
    pAsyncServer->process_messages();

    if (elapsed_secs > 0.03 ){
      begin = clock();      
      aiManager.handle_ai();
    }

    physicsManager.handle_physics();

    create_npc_update_responses(npcControllers, npcResponses);
    pAsyncServer->send_monster_responses(npcResponses);
    npcResponses.clear();

    pAsyncServer->send_login_responses();

    pAsyncServer->send_update_responses();


    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  }




}


void create_npc_update_responses(std::map<std::string, bright::base::ActorControlController>& npcControllers, std::vector<std::string>& npcResponses){

  auto make_an_npc_response = [&] (std::map<std::string, bright::base::ActorControlController>::value_type& pair) {
    std::string npcName = pair.first;
    bright::base::ActorControlController& npc = pair.second;
    if ( npc.have_update() ){
      bright::network::UpdateMessage updateMsg(npcName, npc.pos(), npc.right(), npc.up(), npc.look(), false);
      npcResponses.push_back( updateMsg.full_message() );
      npc.have_update(false);
    }
  };
  std::for_each(npcControllers.begin(), npcControllers.end(), make_an_npc_response);

}
