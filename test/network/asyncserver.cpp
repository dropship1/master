#include "network/asyncserver.hpp"
#include <boost/make_shared.hpp>
#include <ctime>

#include "utils/fileworker.hpp"
#include "converters/aabbconverter.hpp"

void handle_monsters(std::map<std::string, bright::base::ServerActor>& monsters, std::vector<std::string>& monsterResponses, bool left);


int main(int argc, char* argv[]) {

  std::vector<std::string> monsterResponses;

  std::map<std::string, bright::base::ServerActor> monsters;
  monsters["Monster1"] = bright::base::ServerActor();
  monsters["Monster2"] = bright::base::ServerActor();
  monsters["Monster1"].move_left(10.0f);
  monsters["Monster2"].move_right(10.0f);

  std::map<std::string, std::shared_ptr<bright::base::ServerActor>> clientActors;
  clientActors["Jake"] = std::make_shared<bright::base::ServerActor>();
  clientActors["Leo"] = std::make_shared<bright::base::ServerActor>();

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("test/network/data/filelist");
  auto pAABBConverter = std::make_shared<bright::converters::AABBConverter>(pFileWorker);
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();

  auto pAsyncServer = boost::make_shared<bright::network::AsyncServer>(clientActors, pAABBConverter);
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
      handle_monsters(monsters, monsterResponses, left);
      pAsyncServer->send_monster_responses(monsterResponses);
      monsterResponses.clear();
    }

    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  }

}


void handle_monsters(std::map<std::string, bright::base::ServerActor>& monsters, std::vector<std::string>& monsterResponses, bool left){

  auto make_a_monster_response = [&] (std::map<std::string, bright::base::ServerActor>::value_type& pair) { 
    std::string monsterName = pair.first;
    bright::base::ServerActor& monster = pair.second;
    if (left){  monster.move_left(0.2f); }
    else{  monster.move_right(0.2f); }
    bright::network::UpdateMessage updateMsg(monsterName, monster.pos(), monster.right(), monster.up(), monster.look(), false);
    monsterResponses.push_back( updateMsg.full_message() );
  };
  std::for_each(monsters.begin(), monsters.end(), make_a_monster_response);

}