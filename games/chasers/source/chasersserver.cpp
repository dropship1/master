#include "network/asyncserver.hpp"
#include "ai/aimanager.hpp"
#include "base/actorcontrolsresourcemanager.hpp"
#include "base/controlactor.hpp"
#include "utils/fileworker.hpp"
#include "converters/aabbconverter.hpp"
#include "network/updatemessage.hpp"
#include <boost/make_shared.hpp>
#include <ctime>

void create_monster_update_responses(std::map<std::string, bright::base::ActorControlController>& monsters, std::vector<std::string>& monsterResponses);
void check_physics(std::map<std::string, bright::base::ActorControlController>& monsters, std::map<std::string, bright::base::ActorControlController>& clientActors);


int main(int argc, char* argv[]) {

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("../data/files.fl");
  //auto pFileWorker = std::make_shared<bright::utils::FileWorker>("games/chasers/data/files.fl");

  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();

  auto actorControlsResourceManager = std::make_shared<bright::base::ActorControlsResourceManager>(pFileWorker);
  actorControlsResourceManager.initialize();

  std::vector<std::string> monsterResponses;
  auto controlPlayers = actorControlsResourceManager->control_players();
  auto controlNpcs = actorControlsResourceManager->control_npcs();
  auto playerControllers = actorControlsResourceManager->player_controllers();
  auto npcControllers = actorControlsResourceManager->npc_controllers();
  auto playerAABBs = actorControlsResourceManager->player_aabbs();
  auto npcAABBs = actorControlsResourceManager->npc_aabbs();

  bright::ai::AIManager aiManager;

  auto pAsyncServer = boost::make_shared<bright::network::AsyncServer>(clientActors);
  pAsyncServer->start();
  boost::thread thread(boost::bind(&boost::asio::io_service::run, &pAsyncServer->service()));

  clock_t begin = clock();
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  while(true){
    pAsyncServer->process_messages();

    if (elapsed_secs > 0.03 ){
      begin = clock();      
      aiManager.update_monsters(monsters, clientActors);
    }

    check_physics(monsters, clientActors);

    create_monster_update_responses(monsters, monsterResponses);
    pAsyncServer->send_monster_responses(monsterResponses);
    monsterResponses.clear();

    pAsyncServer->send_login_responses();

    pAsyncServer->send_update_responses();


    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  }




}


void create_monster_update_responses(std::map<std::string, bright::base::ActorControlController>& monsters, std::vector<std::string>& monsterResponses){

  auto make_a_monster_response = [&] (std::map<std::string, bright::base::ActorControlController>::value_type& pair) {
    std::string monsterName = pair.first;
    bright::base::ActorControlController& monster = pair.second;
    if ( monster.have_update() ){
      bright::network::UpdateMessage updateMsg(monsterName, monster.pos(), monster.right(), monster.up(), monster.look(), false);
      monsterResponses.push_back( updateMsg.full_message() );
      monster.have_update(false);
    }
  };
  std::for_each(monsters.begin(), monsters.end(), make_a_monster_response);

}



void check_physics(std::map<std::string, bright::base::ActorControlController>& monsters, std::map<std::string, bright::base::ActorControlController>& clientActors){

  auto did_monster_collide_with_client = [&] (std::map<std::string, bright::base::ActorControlController>::value_type& pair) {
    std::string monsterName = pair.first;
    bright::base::ActorControlController& monster = pair.second;
    auto pMonsterAABB = monster.aabb();

    auto check_client = [&] (std::map<std::string, std::shared_ptr<bright::base::ActorControlController>>::value_type& innerPair) {
      std::string clientName = innerPair.first;
      auto pClient = innerPair.second;
      if (pClient->is_logged_in()){
        auto pClientAABB = pClient->aabb();
        bool intersect = pMonsterAABB->intersect(pClientAABB);
        if (intersect){
          monster.set_to_prev_pos();
        }
      }
    };
    std::for_each(clientActors.begin(), clientActors.end(), check_client);
  };
  std::for_each(monsters.begin(), monsters.end(), did_monster_collide_with_client);

  auto did_client_collide_with_monster = [&] (std::map<std::string, std::shared_ptr<bright::base::ActorControlController>>::value_type& pair) {
     std::string clientName = pair.first;
     auto pClient = pair.second;
     if (pClient->is_logged_in()){
      auto pClientAABB = pClient->aabb();

      auto check_monster = [&] (std::map<std::string, bright::base::ActorControlController>::value_type& innerPair) {
        std::string monsterName = innerPair.first;
        bright::base::ActorControlController& monster = innerPair.second;
        auto pMonsterAABB = monster.aabb();

        bool intersect = pClientAABB->intersect(pMonsterAABB);
        if (intersect){
          pClient->set_to_prev_pos();
        }
      };
      std::for_each(monsters.begin(), monsters.end(), check_monster);

    }
  };
  std::for_each(clientActors.begin(), clientActors.end(), did_client_collide_with_monster);

  auto did_monster_collide_with_monster = [&] (std::map<std::string, bright::base::ActorControlController>::value_type& pair) {
    std::string monsterName = pair.first;
    bright::base::ActorControlController& monster = pair.second;
    auto pMonsterAABB = monster.aabb();
  
    auto check_monster = [&] (std::map<std::string, bright::base::ActorControlController>::value_type& innerPair) -> bool {
      std::string monsterName2 = innerPair.first;
      if (monsterName2.compare(monsterName) != 0){
        bright::base::ActorControlController& monster2 = innerPair.second;
        auto pMonster2AABB = monster2.aabb();
        bool intersect = pMonsterAABB->intersect(pMonster2AABB);
        if (intersect){
          monster.set_to_prev_pos();
          monster.have_update(true);
          return true;
        }
        else{ return false; }
      }
      else{ return false; }
    };

    for(auto& monster2 : monsters) {
      if ( check_monster(monster2) == true ){
        break;
      }
    }

  };
  for(auto& monster: monsters){ did_monster_collide_with_monster(monster); } 


}