#include "network/asyncserver.hpp"
#include "ai/aimanager.hpp"
#include "base/worldloader.hpp"
#include "utils/fileworker.hpp"
#include "converters/aabbconverter.hpp"
#include <boost/make_shared.hpp>
#include <ctime>

void create_monster_update_responses(std::map<std::string, bright::base::ServerActor>& monsters, std::vector<std::string>& monsterResponses);
void check_physics(std::map<std::string, bright::base::ServerActor>& monsters, std::map<std::string, std::shared_ptr<bright::base::ServerActor>>& clientActors);

int main(int argc, char* argv[]) {

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("../data/filelist");
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();

  std::vector<std::string> monsterResponses;
  std::map<std::string, bright::base::ServerActor> monsters;

  bright::base::WorldLoader worldLoader(pFileWorker);
  worldLoader.load_world(monsters);

  std::map<std::string, std::shared_ptr<bright::base::ServerActor>> clientActors;

  bright::ai::AIManager aiManager;

  auto pAABBConverter = std::make_shared<bright::converters::AABBConverter>(pFileWorker);
  pAABBConverter->batch_read_obj_aabb_binary();

  auto pAsyncServer = boost::make_shared<bright::network::AsyncServer>(clientActors, pAABBConverter);
  pAsyncServer->start();
  boost::thread thread(boost::bind(&boost::asio::io_service::run, &pAsyncServer->service()));

  clock_t begin = clock();
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  while(true){
    pAsyncServer->process_messages();
    
    pAsyncServer->send_login_responses();

    pAsyncServer->send_update_responses();

    if (elapsed_secs > 0.03 ){
      begin = clock();      
      aiManager.update_monsters(monsters, clientActors);
    }

    check_physics(monsters, clientActors);

    create_monster_update_responses(monsters, monsterResponses);
    pAsyncServer->send_monster_responses(monsterResponses);
    monsterResponses.clear();


    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  }




}


void create_monster_update_responses(std::map<std::string, bright::base::ServerActor>& monsters, std::vector<std::string>& monsterResponses){

  auto make_a_monster_response = [&] (std::map<std::string, bright::base::ServerActor>::value_type& pair) {
    std::string monsterName = pair.first;
    bright::base::ServerActor& monster = pair.second;
    if ( monster.have_update() ){
      bright::network::UpdateMessage updateMsg(monsterName, monster.pos(), monster.right(), monster.up(), monster.look(), false);
      monsterResponses.push_back( updateMsg.full_message() );
      monster.have_update(false);
    }
  };
  std::for_each(monsters.begin(), monsters.end(), make_a_monster_response);

}



void check_physics(std::map<std::string, bright::base::ServerActor>& monsters, std::map<std::string, std::shared_ptr<bright::base::ServerActor>>& clientActors){


  auto did_monster_collide_with_client = [&] (std::map<std::string, bright::base::ServerActor>::value_type& pair) {
    std::string monsterName = pair.first;
    bright::base::ServerActor& monster = pair.second;
    if ( monster.have_update() ){
      auto pMonsterAABB = monster.aabb();

      auto check_client = [&] (std::map<std::string, std::shared_ptr<bright::base::ServerActor>>::value_type& innerPair) {
        std::string clientName = innerPair.first;
        auto pClient = innerPair.second;
        if (pClient->is_logged_in()){
          auto pClientAABB = pClient->aabb();
          bool intersect = pMonsterAABB->intersect(pClientAABB);
          if (intersect){
            monster.set_to_prev_pos();
            //pClient->pos(glm::vec3(0.0f, 0.0f, 0.0f)); 
            //pClient->have_update(true);
          }
        }
      };
      std::for_each(clientActors.begin(), clientActors.end(), check_client);


    }
  };
  std::for_each(monsters.begin(), monsters.end(), did_monster_collide_with_client);

  auto did_client_collide_with_monster = [&] (std::map<std::string, std::shared_ptr<bright::base::ServerActor>>::value_type& pair) {
     std::string clientName = pair.first;
     auto pClient = pair.second;
     if (pClient->is_logged_in()){
      auto pClientAABB = pClient->aabb();

      auto check_monster = [&] (std::map<std::string, bright::base::ServerActor>::value_type& innerPair) {
        std::string monsterName = innerPair.first;
        bright::base::ServerActor& monster = innerPair.second;
        auto pMonsterAABB = monster.aabb();

        bool intersect = pClientAABB->intersect(pMonsterAABB);
        if (intersect){
          pClient->set_to_prev_pos();
          //pClient->pos(glm::vec3(0.0f, 0.0f, 0.0f)); 
          pClient->have_update(true);
        }
      };
      std::for_each(monsters.begin(), monsters.end(), check_monster);

    }
  };
  std::for_each(clientActors.begin(), clientActors.end(), did_client_collide_with_monster);

  auto did_monster_collide_with_monster = [&] (std::map<std::string, bright::base::ServerActor>::value_type& pair) {
    std::string monsterName = pair.first;
    bright::base::ServerActor& monster = pair.second;
    if ( monster.have_update() ){
      auto pMonsterAABB = monster.aabb();

      auto check_monster = [&] (std::map<std::string, bright::base::ServerActor>::value_type& innerPair) {
        std::string monsterName2 = innerPair.first;
        if (monsterName2.compare(monsterName) != 0){
          bright::base::ServerActor& monster2 = innerPair.second;
          auto pMonster2AABB = monster2.aabb();
          bool intersect = pMonsterAABB->intersect(pMonster2AABB);
          if (intersect){
            monster.set_to_prev_pos();
            //monster.pos(glm::vec3(0.0f, 0.0f, 0.0f)); 
            monster.have_update(true);
          }
       }
      };
      std::for_each(monsters.begin(), monsters.end(), check_monster);


    }
  };
  std::for_each(monsters.begin(), monsters.end(), did_monster_collide_with_monster);



}