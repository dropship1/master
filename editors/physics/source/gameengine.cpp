#include "gameengine.hpp"


std::shared_ptr<GameEngine> GameEngine::pGameEngine_ = std::make_shared<GameEngine>();

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int cmdShow){

  MSG msg;
  int previous = 0;
  int lag = 0;
  int stepAmount = 0;

  auto pGameEngine = GameEngine::get_engine();

  if ( game_initialize() ){
    //Initialize the game engine
    if ( !pGameEngine->initialize() ){
      return false;
    }

    //Fixed and Variable time step game loop: see http://gameprogrammingpatterns.com/game-loop.html
    stepAmount = pGameEngine->get_fixed_time_step();
    previous = GetTickCount();

    //Enter the main message loop
    while (true){

      //Fixed and Variable time step game loop: see http://gameprogrammingpatterns.com/game-loop.html
      int current = GetTickCount();
      int elapsed = current - previous;
      previous = current;
      lag += elapsed;

      if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ){
        //Process the message
        if (msg.message == WM_QUIT)
          break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      else{
        //Make sure the game engine isn't sleeping
        if ( !pGameEngine->get_sleep() ){
          //Check the tick count to see if a game cycle has elapsed
          while ( lag > stepAmount ){            
            game_cycle();
            lag -= stepAmount;
          }
          //GameEngine::get_engine()->get_database()->insert_lag_amount(lag);
          game_cycle_graphics();
        }
      }

    }


    return (int)msg.wParam;
  }

  // End the game
  game_end();

  return TRUE;
}

LRESULT GameEngine::handle_event(HWND window, UINT msg, WPARAM wParam, LPARAM lParam){
  // Route Windows messages to game engine member functions
  switch (msg){
    case WM_CREATE:{
      return 0;
    }

    case WM_SETFOCUS:{
      return 0;
    }

    case WM_KILLFOCUS:{
      return 0;
    }

    case WM_SIZE: {
      pContextManager_->reshape_rendering_context_viewport(LOWORD(lParam), HIWORD(lParam));
      return 0;
    }

    case WM_DESTROY:{
      PostQuitMessage(0);
      return 0;
    }

    case WM_QUIT:{
      return 0;
    }

    case WM_INPUT:{
      UINT dwSize = 40;
      static BYTE lpb[40];
    
      GetRawInputData((HRAWINPUT)lParam, RID_INPUT,lpb, &dwSize, sizeof(RAWINPUTHEADER));
    
      RAWINPUT* raw = (RAWINPUT*)lpb;
    
      if (raw->header.dwType == RIM_TYPEKEYBOARD) {
        //https://molecularmusings.wordpress.com/2011/09/05/properly-handling-keyboard-input/
        unsigned int scanCode = raw->data.keyboard.MakeCode;
        unsigned int virtualKey = raw->data.keyboard.VKey;
        unsigned int flags = raw->data.keyboard.Flags;

        if (virtualKey == 255) {
          // discard "fake keys" which are part of an escaped sequence
          return 0;
        }
        else if (virtualKey == VK_SHIFT){
          // correct left-hand / right-hand SHIFT
          virtualKey = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
        }
        else if (virtualKey == VK_NUMLOCK){
          // correct PAUSE/BREAK and NUM LOCK silliness, and set the extended bit
          scanCode = (MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC) | 0x100);
        }
        // e0 and e1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK.
        // see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
        const bool isE0 = ((flags & RI_KEY_E0) != 0);
        const bool isE1 = ((flags & RI_KEY_E1) != 0);
         
        if (isE1){
          // for escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey.
          // however, MapVirtualKey is unable to map VK_PAUSE (this is a known bug), hence we map that by hand.
          if (virtualKey == VK_PAUSE){
            scanCode = 0x45;
          } 
          else{
            scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);
          }
        }

        switch (virtualKey){
          // right-hand CONTROL and ALT have their e0 bit set
          case VK_CONTROL:
            if (isE0)
              virtualKey = WinKeys::RIGHT_CONTROL;
            else
              virtualKey = WinKeys::LEFT_CONTROL;
            break;
         
          case VK_MENU:
            if (isE0)
              virtualKey = WinKeys::RIGHT_ALT;
            else
              virtualKey = WinKeys::LEFT_ALT;
            break;
         
          // NUMPAD ENTER has its e0 bit set
          case VK_RETURN:
            if (isE0)
              virtualKey = WinKeys::NUMPAD_ENTER;
            break;
         
          // the standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always have their e0 bit set, but the
          // corresponding keys on the NUMPAD will not.
          case VK_INSERT:
            if (!isE0)
            virtualKey = WinKeys::NUMPAD_0;
            break;
         
          case VK_DELETE:
            if (!isE0)
              virtualKey = WinKeys::NUMPAD_DECIMAL;
            break;
         
          case VK_HOME:
            if (!isE0)
              virtualKey = WinKeys::NUMPAD_7;
            break;
         
          case VK_END:
            if (!isE0)
              virtualKey = WinKeys::NUMPAD_1;
            break;
         
          case VK_PRIOR:
            if (!isE0)
              virtualKey = WinKeys::NUMPAD_9;
            break;
         
          case VK_NEXT:
            if (!isE0)
              virtualKey = WinKeys::NUMPAD_3;
            break;
         
          // the standard arrow keys will always have their e0 bit set, but the
          // corresponding keys on the NUMPAD will not.
          case VK_LEFT:
            if (!isE0)
              virtualKey = WinKeys::NUMPAD_4;
            break;
         
          case VK_RIGHT:
            if (!isE0)
              virtualKey = WinKeys::NUMPAD_6;
            break;
         
          case VK_UP:
            if (!isE0)
              virtualKey = WinKeys::NUMPAD_8;
            break;
         
          case VK_DOWN:
            if (!isE0)
              virtualKey = WinKeys::NUMPAD_2;
            break;
         
          // NUMPAD 5 doesn't have its e0 bit set
          case VK_CLEAR:
            if (!isE0)
              virtualKey = WinKeys::NUMPAD_5;
            break;
        }

        // a key can either produce a "make" or "break" scancode. this is used to differentiate between down-presses and releases
        // see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
        const bool wasUp = ((flags & RI_KEY_BREAK) != 0);
         
        // getting a human-readable string
        UINT keyc = (scanCode << 16) | (isE0 << 24);
        char buffer[512] = {};
        GetKeyNameTextA((LONG)keyc, buffer, 512);
        std::string key(buffer);
        bool isDown = !wasUp;
        pInputManager_->update_keyboard_listeners(isDown, key);
      }
      else if (raw->header.dwType == RIM_TYPEMOUSE) {
        short int data = 0;
        if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE){
          int xPosRelative = raw->data.mouse.lLastX;
          int yPosRelative = raw->data.mouse.lLastY;
          pInputManager_->update_raw_mouse_listeners_deltas(xPosRelative, yPosRelative);

          CURSORINFO cursorInfo = { 0 };
          cursorInfo.cbSize = sizeof(cursorInfo);
          GetCursorInfo(&cursorInfo);
          float newXPos = cursorInfo.ptScreenPos.x;
          float newYPos = cursorInfo.ptScreenPos.y;
          //pInputManager->update_mouse_listeners_position(newXPos, newYPos);
          
          LPPOINT point = new POINT();
          GetCursorPos(point);

          ScreenToClient(pContextManager_->windows_current_context()->window_id(), point);
          //std::cout << "xPoint" << point->x << std::endl << std::flush;
          //std::cout << "yPoint" << point->y << std::endl << std::flush;
  
        }      
        //MOUSE_LEFT_BUTTON_DOWN
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_1_DOWN){
          pInputManager_->update_raw_mouse_listeners_button(true, bright::input::MouseButton::LEFT, data);
        }        
        //MOUSE_LEFT_BUTTON_UP
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_1_UP){ 
          pInputManager_->update_raw_mouse_listeners_button(false, bright::input::MouseButton::LEFT, data);
        }
        
        //MOUSE_RIGHT_BUTTON_DOWN
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_2_DOWN){ 
          pInputManager_->update_raw_mouse_listeners_button(true, bright::input::MouseButton::RIGHT, data);
        }
        
        //MOUSE_RIGHT_BUTTON_UP
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_2_UP){ 
          pInputManager_->update_raw_mouse_listeners_button(false, bright::input::MouseButton::RIGHT, data);
        }
        
        //MOUSE_MIDDLE_BUTTON_DOWN
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_3_DOWN){ 
          pInputManager_->update_raw_mouse_listeners_button(true, bright::input::MouseButton::MIDDLE, data);
        }
        
        //MOUSE_MIDDLE_BUTTON_UP
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_3_UP){
          pInputManager_->update_raw_mouse_listeners_button(false, bright::input::MouseButton::MIDDLE, data);
        }
        
        //Scroll wheel, Raw input comes from a mouse wheel. The wheel delta is stored in usButtonData.
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_WHEEL){ 
          data = (SHORT)raw->data.mouse.usButtonData;
          pInputManager_->update_raw_mouse_listeners_button(true, bright::input::MouseButton::WHEEL, data);
        }
      }
      return 0;
    }

  }
  return DefWindowProc(window, msg, wParam, lParam);
}


LRESULT CALLBACK WndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam){
  return GameEngine::get_engine()->handle_event(window, msg, wparam, lparam);
}


GameEngine::GameEngine(): sleep_(false), 
                          pFileWorker_(std::make_shared<bright::utils::FileWorker>("editors/physics/data/files.fl")), 
                          pActorControlsResourceManager_(std::make_shared<bright::base::ActorControlsResourceManager>(pFileWorker_)), 
                          pActorRenderingResourceManager_(std::make_shared<bright::base::ActorRenderingResourceManager>(pFileWorker_)), 
                          playerContollers_(pActorControlsResourceManager_->player_controllers()), 
                          npcContollers_(pActorControlsResourceManager_->npc_controllers()), 
                          actorRenderContollers_(pActorRenderingResourceManager_->actor_render_controllers()), 
                          actorGroupRenderInfos_(pActorRenderingResourceManager_->actor_group_render_infos()), 
                          controlPlayers_(pActorControlsResourceManager_->control_players()), 
                          controlNpcs_(pActorControlsResourceManager_->control_npcs())
{
  //pGameEngine_ = shared_from_this();

  //If you're running this from the debuger/visual studio, then you need to specify the path to the 
  //data directory starting from the "bright" directory, for example:
  //games/chasers/data
  //But if you're building this and creating the executable, which goes into the bin directory
  //in games/chaser/bin then you need to specify the path as "../data".
  //pFileWorker_ = std::make_shared<bright::utils::FileWorker>("../data/files.fl");
}

GameEngine::~GameEngine(){

}

bool GameEngine::initialize(){

  pInputManager_ = std::make_shared<bright::input::InputManager>();
  pInputContextManager_ = std::make_shared<bright::input::InputContextManager>();
  pInputManager_->add_keyboard_event_listener(pInputContextManager_);
  // TODO: Finish mouse events
  pInputManager_->add_raw_mouse_event_listener(pInputContextManager_);
  pInputManager_->add_mouse_event_listener(pInputContextManager_);

  std::string controlsContexts = pFileWorker_->get_file_contents("controls_contexts.cfg");
  pInputContextManager_->initialize(controlsContexts);
  std::string controlConfig = pFileWorker_->get_file_contents("controls_config.cfg");
  pInputContextManager_->set_control_config(controlConfig);

  pContextManager_ = std::make_shared<bright::context::ContextManager>();
  pContextManager_->create_windows_opengl_context(WndProc, "Powered By The Bright Engine", 1280, 768);

  pActorCreator_ = std::make_shared<bright::base::ActorCreator>();

  pFileWorker_->initialize();
  pActorControlsResourceManager_->initialize();
  pActorRenderingResourceManager_->initialize();
  pRenderer_ = std::make_shared<bright::graphics::Renderer>();

  playerControlName_ = pActorControlsResourceManager_->client_control_name();
  playerRenderName_ = pActorControlsResourceManager_->client_render_name();
  playerController_ = playerContollers_[playerControlName_];
  playerRenderController_ = actorRenderContollers_[playerRenderName_];
  playerCameraType_ = actorGroupRenderInfos_[playerRenderName_].cameraType_;

  pCommandListener_ = boost::make_shared<bright::input::CommandListener>(playerController_, playerRenderController_, pActorCreator_);
  pInputContextManager_->add_command_event_listener(pCommandListener_);

  worldInfo_ = pActorRenderingResourceManager_->world_info();

  //Show and update the window
  pContextManager_->show_window(false);
  pContextManager_->initialize();

  count = 1;
  //Start a thread here to handle background (asynchronous operations) operations
  //boost::thread thread(boost::bind(&boost::asio::io_service::run, &service_));

  return true;
}



void GameEngine::cycle(){

  pInputManager_->notify();
	pInputContextManager_->notify();
  pCommandListener_->update_commands();
  pCommandListener_->execute_commands();

  //playerContollers_;
  //npcContollers_;
  //actorRenderContollers_;
  //bulletRenderContollers_;
  //actorGroupRenderInfos_;

  //auto bullets = pActorCreator_->get_bullets();
  count++; 
  //auto move_bullets = [&] (std::shared_ptr<bright::base::ServerActor> pBullet) {
  //  //auto it = monsterGroupRenderInfos_.find( monsterName );
  //  //if ( it == monsterGroupRenderInfos_.end() ){
  //  //  bulletRenderInfos_ = pResourceManager_->actor_group_render_infos()["cube"];
  //  //}
  //  //pRenderer_->render(monsterGroupRenderInfos_[monsterName], pWorldInfo_);
  //  if (!pBullet->have_update()){   
  //    if (count <= 200){ pBullet->move_fwd(0.2f);   if ( count == 200){pBullet->rotate_left(180.0f);} } 
  //    else if (count > 200){ pBullet->move_backward(0.2f); if ( count == 400){pBullet->rotate_left(180.0f);} }
  //  };
  //};
  //std::for_each(bullets.begin(), bullets.end(), move_bullets);
  //create_bullet_controllers();
  //if (count == 400){ count = 0; }
  //
  //check_physics();
  //update_monster_controllers();

}

void GameEngine::cycle_graphics(){

  pContextManager_->begin_rendering();

  worldInfo_.world_to_cam_matrix( playerCameraType_, playerRenderController_.world_to_camera_transformation_matrix() );

  auto render_players = [&] (std::map<std::string, bright::base::ControlActor>::value_type& pair) {
    auto actorGroupRenderInfo = actorGroupRenderInfos_[pair.second.render_name()];
    auto playerController = playerContollers_[pair.second.control_name()];
    auto playerRenderController = actorRenderContollers_[pair.second.render_name()];
    auto pos = playerController.pos();
    auto right = playerController.right();
    auto up = playerController.up();
    auto look = playerController.look();
    playerRenderController.update(pos, right, up, look);
    actorGroupRenderInfo.modToWorld_ = playerRenderController.model_to_world_transformation_matrix();
    pRenderer_->render(actorGroupRenderInfo, worldInfo_);
  };
  std::for_each(controlPlayers_.begin(), controlPlayers_.end(), render_players);

  auto render_npcs = [&] (std::map<std::string, bright::base::ControlActor>::value_type& pair) {
    auto actorGroupRenderInfo = actorGroupRenderInfos_[pair.second.render_name()];
    auto npcController = npcContollers_[pair.second.control_name()];
    auto npcRenderController = actorRenderContollers_[pair.second.render_name()];
    auto pos = npcController.pos();
    auto right = npcController.right();
    auto up = npcController.up();
    auto look = npcController.look();
    npcRenderController.update(pos, right, up, look);
    actorGroupRenderInfo.modToWorld_ = npcRenderController.model_to_world_transformation_matrix();
    pRenderer_->render(actorGroupRenderInfo, worldInfo_);
  };
  std::for_each(controlNpcs_.begin(), controlNpcs_.end(), render_npcs);

  //int count = 0;
  //auto render_bullet_and_add_if_not_seen_yet = [&] (std::map<std::string, std::shared_ptr<bright::base::ClientController>>::value_type& pair) {
  //  std::string bulletNumber = pair.first;
  //  auto controller = pair.second;
  ////  std::string Result;          // string which will contain the result
  ////  
  ////  std::ostringstream convert;   // stream used for the conversion
  ////  
  ////  convert << count;      // insert the textual representation of 'Number' in the characters in the stream
  ////  
  ////  Result = convert.str(); // set 'Result' to the contents of the stream
  ////  
  ////
  //  auto it = bulletGroupRenderInfos_.find( bulletNumber );
  //  if ( it == bulletGroupRenderInfos_.end() ){
  //    bulletGroupRenderInfos_[bulletNumber] = pResourceManager_->actor_group_render_infos()["User"];
  //  }
  //  bulletGroupRenderInfos_[bulletNumber]->modToWorld_ = controller->model_to_world_transformation_matrix();
  //  pRenderer_->render(bulletGroupRenderInfos_[bulletNumber], pWorldInfo_);
  ////  count++;
  //};
  //std::for_each(bulletContollers_.begin(), bulletContollers_.end(), render_bullet_and_add_if_not_seen_yet);
  
  pContextManager_->end_rendering();

}


//void GameEngine::check_physics(){
//
//  //did_client_collide_with_monster
//   auto pClientActorAABB = pClientActor_->aabb();
//
//   auto check_monster = [&] (std::map<std::string, bright::base::ServerActor>::value_type& innerPair) {
//     std::string monsterName = innerPair.first;
//     bright::base::ServerActor& monster = innerPair.second;
//     auto pMonsterAABB = monster.aabb();
//
//     bool intersect = pClientActorAABB->intersect(pMonsterAABB);
//     if (intersect){
//       pClientActor_->set_to_prev_pos();
//     }
//   };
//   std::for_each(monsters_.begin(), monsters_.end(), check_monster);
//
//
//  auto bullets = pActorCreator_->get_bullets();
//  auto check_bullets = [&] (std::shared_ptr<bright::base::ServerActor> pBullet) {
//    auto pBulletAABB = pBullet->aabb();
//
//    auto check_monster2 = [&] (std::map<std::string, bright::base::ServerActor>::value_type& pair) {
//      std::string monsterName = pair.first;
//      bright::base::ServerActor& monster = pair.second;
//      auto pMonsterAABB = monster.aabb();
//       bool intersect = pBulletAABB->intersect(pMonsterAABB);
//       if (intersect){
//         monster.move_left(5.0f);
//         pBullet->move_fwd(20.0f);
//         pBullet->have_update(true);
//       }  
//    };
//    std::for_each(monsters_.begin(), monsters_.end(), check_monster2);
//
//  };
//  std::for_each(bullets.begin(), bullets.end(), check_bullets);
//
//}



//void GameEngine::create_npc_controllers(){
//
//  auto check_monster = [&] (std::map<std::string, bright::base::ServerActor>::value_type& innerPair) {
//    std::string monsterName = innerPair.first;
//    bright::base::ServerActor& monster = innerPair.second;
//    monsterContollers_[monsterName] = std::make_shared<bright::base::ClientController>();
//    monsterContollers_[monsterName]->update( monster.pos(), monster.right(), monster.up(), monster.look() ); 
//  };
//  std::for_each(monsters_.begin(), monsters_.end(), check_monster);
//
//}

//void GameEngine::update_npc_controllers(){
//
//  auto check_monster = [&] (std::map<std::string, bright::base::ServerActor>::value_type& innerPair) {
//    std::string monsterName = innerPair.first;
//    bright::base::ServerActor& monster = innerPair.second;
//    monsterContollers_[monsterName]->update( monster.pos(), monster.right(), monster.up(), monster.look() ); 
//  };
//  std::for_each(monsters_.begin(), monsters_.end(), check_monster);
//
//}


//void GameEngine::create_bullet_controllers(){
//
//  auto bullets = pActorCreator_->get_bullets();
//  int count = 0;
//  auto check_bullet = [&] (std::shared_ptr<bright::base::ActorControlController> pBullet ) {
//
//    std::string bulletNumber;          // string which will contain the result
//    
//    std::ostringstream convert;   // stream used for the conversion
//    
//    convert << count;      // insert the textual representation of 'Number' in the characters in the stream
//    
//    bulletNumber = convert.str(); // set 'Result' to the contents of the stream
//    auto it = bulletContollers_.find( bulletNumber );
//    if ( it == bulletContollers_.end() ){
//      bulletContollers_[bulletNumber] = bright::base::ActorControlController();
//    }
//    bulletContollers_[bulletNumber].update( pBullet->pos(), pBullet->rotation() ); 
//    count++;
//  };
//  std::for_each(bullets.begin(), bullets.end(), check_bullet);
//
//}