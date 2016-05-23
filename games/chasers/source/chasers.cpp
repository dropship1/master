#include "chasers.hpp"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
bool game_initialize(){

  isGameOver = false;

  // Create the game engine
  pGameEngine = GameEngine::get_engine();
  if (!pGameEngine){
    return false;
  }

  // Set the frame rate
  pGameEngine->set_fixed_time_step(30);

  return true;
}

void game_start(){

  // Seed the random number generator
  srand(GetTickCount());

}

void game_end(){
  isGameOver = true;
  // Cleanup the game engine (Got anything to clean??)
}

void game_activate(){}

void game_deactivate(){}


void game_cycle(){

  if (!isGameOver){
    pGameEngine->cycle();
  }

}


void game_cycle_graphics(){

  if (!isGameOver){
    pGameEngine->cycle_graphics();
  }

}


void handle_keys(){

  if (!isGameOver){
  }

}

