#ifndef BRIGHT_GAME_ENGINE_H
#define BRIGHT_GAME_ENGINE_H

#include <iostream>
#include <memory>
#include <algorithm>

#include <boost/make_shared.hpp>

#include "graphics/renderer.hpp"
#include "graphics/loadersmanager.hpp"
#include "graphics/globalstructs.hpp"
#include "graphics/worldinfo.hpp"
#include "base/globalstructs.hpp"
#include "context/contextmanager.hpp"
#include "context/context.hpp"
#include "base/clientcontroller.hpp"
#include "base/resourcemanager.hpp"
#include "base/serveractor.hpp"

#include "converters/aabbconverter.hpp"

#include "input/inputmanager.hpp"
#include "input/keyboardeventlistener.hpp"
#include "input/keyboardevent.hpp"
#include "input/rawmouseeventlistener.hpp"
#include "input/rawmouseevent.hpp"
#include "input/mouseeventlistener.hpp"
#include "input/mouseevent.hpp"
#include "input/commandlistener.hpp"
#include "input/utils.hpp"
#include "input/globalstructs.hpp"
#include "graphics/utilities.hpp"

#include "base/worldloader.hpp"
#include "base/actorcreator.hpp"

#include "input/commandevent.hpp"
#include "input/inputcontextmanager.hpp"
#include <sstream>



int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int cmdShow);

LRESULT CALLBACK  WndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

enum WinKeys {
  RIGHT_CONTROL = VK_RCONTROL,
  LEFT_CONTROL = VK_LCONTROL,
  RIGHT_ALT = VK_RMENU,
  LEFT_ALT = VK_LMENU,
  NUMPAD_ENTER = VK_SEPARATOR,
  NUMPAD_DECIMAL = VK_DECIMAL,
  NUMPAD_0 = VK_NUMPAD0,
  NUMPAD_1 = VK_NUMPAD1,
  NUMPAD_2 = VK_NUMPAD2,
  NUMPAD_3 = VK_NUMPAD3,
  NUMPAD_4 = VK_NUMPAD4,
  NUMPAD_5 = VK_NUMPAD5,
  NUMPAD_6 = VK_NUMPAD6,
  NUMPAD_7 = VK_NUMPAD7,
  NUMPAD_8 = VK_NUMPAD8,
  NUMPAD_9 = VK_NUMPAD9
};


bool game_initialize();
void game_start();
void game_end();
void game_activate();
void game_deactivate();
void game_cycle();
void game_cycle_graphics();


class GameEngine: public std::enable_shared_from_this<GameEngine>{

public:
  GameEngine();
  virtual ~GameEngine();

  bool initialize();
  LRESULT handle_event(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

  void cycle();
  void cycle_graphics();

  int get_fixed_time_step() { return fixedTimeStep_; };
  void set_fixed_time_step(int timeStep) { fixedTimeStep_ = timeStep; };
  bool get_sleep() { return sleep_; };
  void set_sleep(bool sleep) { sleep_ = sleep; };
  static std::shared_ptr<GameEngine> get_engine(){ return pGameEngine_; };


private:
  // Member Variables
  static std::shared_ptr<GameEngine> pGameEngine_;
  int fixedTimeStep_;
  bool sleep_;

  std::shared_ptr<bright::context::ContextManager> pContextManager_;

  std::shared_ptr<bright::base::ActorCreator> pActorCreator_;

  std::shared_ptr<bright::base::ClientController> pPlanesController_;
  std::shared_ptr<bright::base::ClientController> pPlayersController_;
  std::map<std::string, std::shared_ptr<bright::base::ClientController>> monsterContollers_;
  std::map<std::string, std::shared_ptr<bright::base::ClientController>> bulletContollers_;

  std::shared_ptr<bright::utils::FileWorker> pFileWorker_;

  std::shared_ptr<bright::input::InputManager> pInputManager_;
  std::shared_ptr<bright::input::InputContextManager> pInputContextManager_;
  boost::shared_ptr<bright::input::CommandListener> pCommandListener_;

  std::shared_ptr<bright::base::ResourceManager> pResourceManager_;

  std::shared_ptr<bright::graphics::ActorGroupRenderInfo> pPlayerGroupRenderInfo_;
  std::shared_ptr<bright::graphics::ActorGroupRenderInfo> pPlaneGroupRenderInfo_;
  std::map<std::string, std::shared_ptr<bright::graphics::ActorGroupRenderInfo>> monsterGroupRenderInfos_;
  std::map<std::string, std::shared_ptr<bright::graphics::ActorGroupRenderInfo>> bulletGroupRenderInfos_;
  std::shared_ptr<bright::graphics::WorldInfo> pWorldInfo_;
  std::shared_ptr<bright::graphics::Renderer> pRenderer_;

  std::shared_ptr<bright::base::ServerActor> pClientActor_;
  std::map<std::string, bright::base::ServerActor> monsters_;
  std::map<std::string, bright::base::ServerActor> players_;
  int count;

  std::shared_ptr<bright::converters::AABBConverter> pAABBConverter_;

  void check_physics();
  void create_monster_controllers();
  void create_bullet_controllers();
  void update_monster_controllers();
  
};


#endif

