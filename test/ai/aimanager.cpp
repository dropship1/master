#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "ai/aimanager.hpp"

using namespace bright::ai;

void print_board(std::map<std::string, bright::base::ActorControlController> &monsters, std::map<std::string, std::shared_ptr<bright::base::ActorControlController>> &players) {
  int playerSymbol = 1;
  int monsterSymbol = 3;
  int playerAndNpcSymbol = 4;
  int boardHeight = 20;
  int boardWidth = 20;
  int board[400] = {}; // height * width

  for (auto iter = monsters.begin(); iter != monsters.end(); ++iter) {
    auto monster = (*iter).second;
    int x = (int)(monster.pos().x);
    int z = (int)(monster.pos().z);
    board[boardHeight * z + x] = monsterSymbol;
  }
  for (auto iter = players.begin(); iter != players.end(); ++iter) {
    auto player = (*iter).second;
    int x = (int)(player->pos().x);
    int z = (int)(player->pos().z);
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
  AIManager aimanager;

  float monsterPositions[]  = { 0.0f,  0.0f, 10.0f,
                                3.0f,  0.0f, 1.0f,
                                14.0f, 0.0f, 2.0f };

  // Setup Monster positions and rotations
  auto monsters = std::map<std::string, bright::base::ActorControlController>();
  for (int i = 0; i < 9; i += 3) {
    bright::base::ActorControlController monster;
    monster.pos(glm::vec3(monsterPositions[i],
                monsterPositions[i+1],
                monsterPositions[i+2]) );
    monsters["Zombie" + std::to_string(i)] = monster;
  }

  float playerPositions[]  = { 5.0f,  0.0f, 5.0f,
                               2.0f,  0.0f, 11.0f,
                               19.0f, 0.0f, 19.0f };

  // Setup player positions and rotations
  auto players = std::map<std::string, std::shared_ptr<bright::base::ActorControlController>>();
  for (int i = 0; i < 9; i += 3) {
    auto pPlayer = std::make_shared<bright::base::ActorControlController>();
    pPlayer->pos(glm::vec3(playerPositions[i],
                playerPositions[i+1],
                playerPositions[i+2]) );
    players["Player" + std::to_string(i)] = pPlayer;
  }

  int monsterMoveCount = 0;
  do {

    //Let loose the monsters!
    monsterMoveCount = aimanager.update_monsters(monsters, players);
    system("cls");
    print_board(monsters, players);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  } while(monsterMoveCount != 0) ;

  std::cout << "FINISHED AI TEST." << std::endl;
  system("PAUSE");
}


