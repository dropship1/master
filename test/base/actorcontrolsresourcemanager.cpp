#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "base/actorcontrolsresourcemanager.hpp"
#include "utils/fileworker.hpp"


void print_world(std::map<std::string, bright::base::ActorControlController>& npcs, std::map<std::string, bright::base::ActorControlController>& players) {
  int npcSymbol = 3;
  int playerSymbol = 1;
  int playerAndNpcSymbol = 4;
  int boardHeight = 20;
  int boardWidth = 20;
  int board[400] = {}; // height * width

  for (auto iter = npcs.begin(); iter != npcs.end(); ++iter) {
    auto npc = (*iter).second;
    int x = (int)(npc.pos().x);
    int z = (int)(npc.pos().z);
    board[boardHeight * z + x] = npcSymbol;
  }
  for (auto iter = players.begin(); iter != players.end(); ++iter) {
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

  std::cout << "STARTING WORLD_LOADER TEST...." << std::endl;

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("data/files.fl");
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();
  auto pActorControlResourceManager = std::make_shared<bright::base::ActorControlsResourceManager>(pFileWorker);

  auto& npcControllers = pActorControlResourceManager->npc_controllers();
  auto& playerControllers = pActorControlResourceManager->player_controllers();

  std::cout << "BEFORE LOADING WORLD:" << std::endl;
  print_world(npcControllers, playerControllers);
  pActorControlResourceManager->initialize();
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  system("cls");
  std::cout << "AFTER LOADING WORLD:" << std::endl;
  print_world(npcControllers, playerControllers);
  std::cout << "FINISHED WORLD_LOADER TEST." << std::endl;
  system("PAUSE");
}


