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

LRESULT CALLBACK WndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam){
  return GameEngine::get_engine()->handle_event(window, msg, wparam, lparam);
}


GameEngine::GameEngine(): sleep_(false){
  //pGameEngine_ = shared_from_this();
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

  pContextManager_ = std::make_shared<bright::context::ContextManager>();
  pContextManager_->create_windows_opengl_context(WndProc, "Powered By The Bright Engine", 1280, 768);

  //If you're running this from the debuger/visual studio, then you need to specify the path to the 
  //data directory starting from the "bright" directory, for example:
  //games/chasers/data
  //But if you're building this and creating the executable, which goes into the bin directory
  //in games/chaser/bin then you need to specify the path as "../data".
  pFileWorker_ = std::make_shared<bright::utils::FileWorker>("../data/filelist");
  //pFileWorker_ = std::make_shared<bright::utils::FileWorker>("games/chasers/data/filelist");
  pResourceManager_ = std::make_shared<bright::base::ResourceManager>(pFileWorker_);
  pFileWorker_->read_in_list_of_files();
  pFileWorker_->create_lookup_map_of_files_content();
  pResourceManager_->initialize();
  pRenderer_ = std::make_shared<bright::graphics::Renderer>();

  load_client_configs();

  pClientActor_ = std::make_shared<bright::base::ServerActor>();
  //clientActor_->aabb( pAABBConverter_->aabb("cube") );
  pClientActor_->pos(glm::vec3(50.0f, 30.0f, -50.0f));
  pClientActor_->rotate_down(55.0f);
  pClientActor_->name(clientName_);
  pClientActor_->is_logged_in(true);

  pPlanesController_ = std::make_shared<bright::base::ClientController>();
  pPlanesController_->update( glm::vec3(0.0f,-0.5f,0.0f) );

  pPlayersController_ = std::make_shared<bright::base::ClientController>();
  //pPlayersController_->update( glm::vec3(0.0f,50.0f,-10.0f), glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f,-1.0f,0.0f) );
  pPlayersController_->update( pClientActor_->pos(), pClientActor_->right(), pClientActor_->up(), pClientActor_->look() );

  pServerHandler_ = std::make_shared<bright::network::ServerHandler>(clientName_, "mypassword", pPlayersController_, monsterContollers_);
  pServerConnection_ = boost::make_shared<bright::network::ServerConnection>(service_, pServerHandler_);
  boost::asio::ip::tcp::endpoint ep( boost::asio::ip::address::from_string(ip_), port_);

  pCommandListener_ = boost::make_shared<bright::input::CommandListener>(pClientActor_, pPlayersController_);
  pInputContextManager_->add_command_event_listener(pCommandListener_);

  pInputContextManager_->add_command_event_listener(pServerConnection_);
  std::string controlsContexts = pFileWorker_->get_file_contents("controls_contexts.cfg");
  pInputContextManager_->initialize(controlsContexts);
  std::string controlConfig = pFileWorker_->get_file_contents("controls_config.cfg");
  pInputContextManager_->set_control_config(controlConfig);

  pPlaneGroupRenderInfo_ = bright::graphics::create_plane(20.0f);
  pPlaneGroupRenderInfo_->modToWorld_ = pPlanesController_->model_to_world_transformation_matrix();
  pPlaneGroupRenderInfo_->pShader_ = pResourceManager_->graphics_loader_manager().shaders("PER_FRAG_COLOR");
  pPlaneGroupRenderInfo_->hasShader_ = true;
  pPlaneGroupRenderInfo_->cameraType_ = "1st";
  pPlaneGroupRenderInfo_->actorRenderInfos_["plane"]->diffuseColor_ = glm::vec4(0.0f,0.5f,0.0f,1.0f);

  pPlayerGroupRenderInfo_ = pResourceManager_->actor_group_render_infos()[clientName_];

  pWorldInfo_ = pResourceManager_->world_info();
  pWorldInfo_->world_to_cam_matrix( "1st", pPlayersController_->world_to_camera_transformation_matrix() );

  //Show and update the window
  pContextManager_->show_window(false);
  pContextManager_->initialize();

  pServerConnection_->start(ep);
  boost::thread thread(boost::bind(&boost::asio::io_service::run, &service_));

  return true;
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


void GameEngine::cycle(){

  pInputManager_->notify();
	pInputContextManager_->notify();
  pCommandListener_->update_commands();
  pCommandListener_->execute_commands();

  if ( !pServerConnection_->connected() ){
    return;
  }
  if ( !pServerConnection_->logged_in() ){
    pServerConnection_->process_messages();
    return;
  }

  pServerConnection_->send_commands();
  pServerConnection_->process_messages();

}

void GameEngine::cycle_graphics(){

  pContextManager_->begin_rendering();
  
  pPlayerGroupRenderInfo_->modToWorld_ = pPlayersController_->model_to_world_transformation_matrix();
  pWorldInfo_->world_to_cam_matrix( "1st", pPlayersController_->world_to_camera_transformation_matrix() );
  pRenderer_->render(pPlaneGroupRenderInfo_, pWorldInfo_);
  pRenderer_->render(pPlayerGroupRenderInfo_, pWorldInfo_);

  auto render_monster_and_add_if_not_seen_yet = [&] (std::map<std::string, std::shared_ptr<bright::base::ClientController>>::value_type& pair) {
    std::string monsterName = pair.first;
    auto controller = pair.second;
    auto it = monsterGroupRenderInfos_.find( monsterName );
    if ( it == monsterGroupRenderInfos_.end() ){
      monsterGroupRenderInfos_[monsterName] = pResourceManager_->actor_group_render_infos()[monsterName];
    }
    monsterGroupRenderInfos_[monsterName]->modToWorld_ = controller->model_to_world_transformation_matrix();
    pRenderer_->render(monsterGroupRenderInfos_[monsterName], pWorldInfo_);
  };
  std::for_each(monsterContollers_.begin(), monsterContollers_.end(), render_monster_and_add_if_not_seen_yet);
  
  pContextManager_->end_rendering();

}


void GameEngine::load_client_configs(){

  std::string fileContents = pFileWorker_->get_file_contents("client.cfg");
  std::stringstream in(fileContents);

  std::string::size_type sz;
  std::string portTemp;

  bool inServerNode = false;
  std::string line; 
  while (getline(in, line)){
    if (line.substr(0,1) == "]" ){
      break;
    }
    if (inServerNode){
      if(line.substr(0,3) == "ip="){
        ip_ = line.substr(3);
      }
      else if(line.substr(0,5) == "port="){
        portTemp = line.substr(5);
        port_ = std::stoi (portTemp,&sz);
      }
      else if(line.substr(0,11) == "clientname="){
        clientName_ = line.substr(11);
      }
      else if(line.substr(0,9) == "</Server>"){
        inServerNode = false;
      }
    }
    else if (line.substr(0,8) == "<Server>"){ 
      inServerNode = true;
    }
  }


}