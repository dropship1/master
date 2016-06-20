#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "base/actorcontrolsresourcemanager.hpp"
#include "ai/aimanager.hpp"
#include "utils/fileworker.hpp"


using namespace bright::ai;

void print_board(std::map<std::string, bright::base::ActorControlController>& npcControllers, std::map<std::string, bright::base::ActorControlController>& playerControllers) {
  int playerSymbol = 1;
  int npcSymbol = 3;
  int playerAndNpcSymbol = 4;
  int boardHeight = 20;
  int boardWidth = 20;
  int board[400] = {}; // height * width

  for (auto iter = npcControllers.begin(); iter != npcControllers.end(); ++iter) {
    auto npc = (*iter).second;
    int x = (int)(npc.pos().x);
    int z = (int)(npc.pos().z);
    board[boardHeight * z + x] = npcSymbol;
  }
  for (auto iter = playerControllers.begin(); iter != playerControllers.end(); ++iter) {
    auto player = (*iter).second;
    int x = (int)(player.pos().x);
    int z = (int)(player.pos().z);
    if (board[boardHeight * z + x] == 3) {
      board[boardHeight * z + x] = playerAndNpcSymbol;
    }
    else{
      board[boardHeight * z + x] = playerSymbol;
    }
  }
  // Print board
  for (int i = 0; i < (boardHeight * boardWidth); ++i) {
    if (board[i] == 0) {
      std::cout << " ";
    }
    else {
      std::cout << board[i];
    }
    if ( (i % boardWidth) == (boardWidth - 1)) {
      std::cout << std::endl;
    }
  }
}

int main(int argc, char* argv[]) {

  std::cout << "STARTING AI TEST...." << std::endl;


  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("test/ai/data/files.fl");
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();
  auto pActorControlResourceManager = std::make_shared<bright::base::ActorControlsResourceManager>(pFileWorker);
  pActorControlResourceManager->initialize();

  bright::ai::AIManager aiManager(pActorControlResourceManager);

  auto& npcContollers = pActorControlResourceManager->npc_controllers();
  auto& playerControllers = pActorControlResourceManager->player_controllers();

  int npcMoveCount = 0;
  do {

    //Let loose the npcs!
    npcMoveCount = aiManager.handle_ai();
    system("cls");
    print_board(npcContollers, playerControllers);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  } while(npcMoveCount != 0) ;

  std::cout << "FINISHED AI TEST." << std::endl;
  system("PAUSE");
}


