#include <iostream>
#include "base/worldloader.hpp"
#include "utils/fileworker.hpp"

using namespace bright::base;
using namespace bright::utils;

void print_world(std::map<std::string, ServerActor> &npcs) {
  int npcSymbol = 3;
  int boardHeight = 20;
  int boardWidth = 20;
  int board[400] = {}; // height * width

  for (auto iter = npcs.begin(); iter != npcs.end(); ++iter) {
    auto npc = (*iter).second;
    int x = (int)(npc.pos().x);
    int z = (int)(npc.pos().z);
    board[boardHeight * z + x] = npcSymbol;
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

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("test/base/data/filelist");
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();
  WorldLoader worldLoader(pFileWorker);

  auto npcs = std::map<std::string, ServerActor>();

  std::cout << "BEFORE LOADING WORLD:" << std::endl;
  print_world(npcs);
  system("PAUSE");
  worldLoader.load_world(npcs);
  std::cout << "AFTER LOADING WORLD:" << std::endl;
  print_world(npcs);
  system("PAUSE");
  std::cout << "FINISHED WORLD_LOADER TEST." << std::endl;
  system("PAUSE");
}


