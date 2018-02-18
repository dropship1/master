#include "network/serverconnection.hpp"
#include <windows.h>
#include <iostream>
#include "graphics/renderer.hpp"
#include "graphics/loadersmanager.hpp"
#include "graphics/globalstructs.hpp"
#include "base/globalstructs.hpp"
#include "context/contextmanager.hpp"
#include "context/context.hpp"
#include "base/actorcontrolcontroller.hpp"
#include <memory>
#include <algorithm>

#include "input/inputmanager.hpp"
#include "input/keyboardeventlistener.hpp"
#include "input/keyboardevent.hpp"
#include "input/rawmouseeventlistener.hpp"
#include "input/rawmouseevent.hpp"
#include "input/mouseeventlistener.hpp"
#include "input/mouseevent.hpp"
#include "input/utils.hpp"
#include "input/globalstructs.hpp"
#include "graphics/utilities.hpp"


#include "input/commandevent.hpp"
#include "input/inputcontextmanager.hpp"


//Enable this to have a console to use cout and cin on, for debugging
//#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

void load_client_configs();

std::shared_ptr<bright::context::ContextManager> pContextManager;
std::shared_ptr<bright::base::ActorControlController> pModelsController;
std::map<std::string, std::shared_ptr<bright::base::ActorControlController>> npcContollers;
std::shared_ptr<bright::utils::FileWorker> pFileWorker;
std::shared_ptr<bright::input::InputContextManager> pInputContextManager;

std::string ip;
int port;
std::string clientName;

boost::asio::io_service service;


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

std::shared_ptr<bright::input::InputManager> pInputManager;

float lastX = FLT_MAX;
float lastY = FLT_MAX;
float lastZ = FLT_MAX;

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam){
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
      pContextManager->reshape_rendering_context_viewport(LOWORD(lParam), HIWORD(lParam));
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
        pInputManager->update_keyboard_listeners(isDown, key);
      }
      else if (raw->header.dwType == RIM_TYPEMOUSE) {
        short int data = 0;
        int stop = 0;
        
        //int xPosRelative = raw->data.mouse.lLastX;
        //int yPosRelative = raw->data.mouse.lLastY;
        //pInputManager->update_raw_mouse_listeners_deltas(xPosRelative, yPosRelative);
        //if (raw->data.mouse.usFlags & (1 << MOUSE_ATTRIBUTES_CHANGED)) {
        //  std::cout << "MOUSE_ATTRIBUTES_CHANGED" <<  std::endl << std::flush;
        //}
        //if (raw->data.mouse.usFlags & (1 << MOUSE_MOVE_ABSOLUTE)) {
        //  std::cout << "MOUSE_MOVE_ABSOLUTE" << std::endl << std::flush;
        //}
        //if (raw->data.mouse.usFlags == MOUSE_VIRTUAL_DESKTOP) {
        //  std::cout << "MOUSE_VIRTUAL_DESKTOP" << std::endl << std::flush;
        //}
        if ((raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == MOUSE_MOVE_ABSOLUTE){
          
          std::cout << "raw->data.mouse.usFlags" << raw->data.mouse.usFlags << std::endl << std::flush;
          std::cout << "MOUSE_MOVE_ABSOLUTE" << std::endl << std::flush;
          //int xPosRelative = raw->data.mouse.lLastX;
          //int yPosRelative = raw->data.mouse.lLastY;
          //pInputManager->update_raw_mouse_listeners_deltas(xPosRelative, yPosRelative);
          //
          //CURSORINFO cursorInfo = { 0 };
          //cursorInfo.cbSize = sizeof(cursorInfo);
          //GetCursorInfo(&cursorInfo);
          //float newXPos = cursorInfo.ptScreenPos.x;
          //float newYPos = cursorInfo.ptScreenPos.y;
          //pInputManager->update_mouse_listeners_position(newXPos, newYPos);
          
          //LPPOINT point = new POINT();
          //GetCursorPos(point);

          //ScreenToClient(pContextManager->windows_current_context()->window_id(), point);
          //std::cout << "xPoint" << point->x << std::endl << std::flush;
          //std::cout << "yPoint" << point->y << std::endl << std::flush;
          //https://stackoverflow.com/questions/31949476/raw-input-mouse-lastx-lasty-with-odd-values-while-logged-in-through-rdp

          const bool virtual_desktop = (raw->data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) == MOUSE_VIRTUAL_DESKTOP;
          const int width = GetSystemMetrics(virtual_desktop ? SM_CXVIRTUALSCREEN : SM_CXSCREEN);
          const int height = GetSystemMetrics(virtual_desktop ? SM_CYVIRTUALSCREEN : SM_CYSCREEN);
          float absolute_posX = (raw->data.mouse.lLastX / float(USHRT_MAX)) * width;
          float absolute_posY = (raw->data.mouse.lLastY / float(USHRT_MAX)) * height;
          float absolute_posZ = 0;
          std::cout << "width" << width << std::endl << std::flush;
          std::cout << "height" << height << std::endl << std::flush;
          std::cout << "absolute_posX" << absolute_posX << std::endl << std::flush;
          std::cout << "absolute_posY" << absolute_posY << std::endl << std::flush;
          if ((lastX != FLT_MAX) && (lastY != FLT_MAX) && (lastZ != FLT_MAX)) {
            pInputManager->update_raw_mouse_listeners_deltas(absolute_posX - lastX, absolute_posY - lastY);
          }
          lastX = absolute_posX;
          lastY = absolute_posY;
          lastZ = absolute_posZ;
        }
        else{
          pInputManager->update_raw_mouse_listeners_deltas(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
        }
        //MOUSE_LEFT_BUTTON_DOWN
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_1_DOWN){
          pInputManager->update_raw_mouse_listeners_button(true, bright::input::MouseButton::LEFT, data);
        }        
        //MOUSE_LEFT_BUTTON_UP
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_1_UP){ 
          pInputManager->update_raw_mouse_listeners_button(false, bright::input::MouseButton::LEFT, data);
        }
        
        //MOUSE_RIGHT_BUTTON_DOWN
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_2_DOWN){ 
          pInputManager->update_raw_mouse_listeners_button(true, bright::input::MouseButton::RIGHT, data);
        }
        
        //MOUSE_RIGHT_BUTTON_UP
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_2_UP){ 
          pInputManager->update_raw_mouse_listeners_button(false, bright::input::MouseButton::RIGHT, data);
        }
        
        //MOUSE_MIDDLE_BUTTON_DOWN
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_3_DOWN){ 
          pInputManager->update_raw_mouse_listeners_button(true, bright::input::MouseButton::MIDDLE, data);
        }
        
        //MOUSE_MIDDLE_BUTTON_UP
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_3_UP){
          pInputManager->update_raw_mouse_listeners_button(false, bright::input::MouseButton::MIDDLE, data);
        }
        
        //Scroll wheel, Raw input comes from a mouse wheel. The wheel delta is stored in usButtonData.
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_WHEEL){ 
          data = (SHORT)raw->data.mouse.usButtonData;
          pInputManager->update_raw_mouse_listeners_button(true, bright::input::MouseButton::WHEEL, data);
        }
      }
      return 0;
    }

  }
  return DefWindowProc(hWindow, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){ 

  pInputManager = std::make_shared<bright::input::InputManager>();
  pInputContextManager = std::make_shared<bright::input::InputContextManager>();
  pInputManager->add_keyboard_event_listener(pInputContextManager);
  // TODO: Finish mouse events
  pInputManager->add_raw_mouse_event_listener(pInputContextManager);
  //pInputManager->add_mouse_event_listener(pInputContextManager);

  //If you're running this from the debuger/visual studio, then you need to specify the path to the 
  //data directory starting from the "bright" directory, for example:
  //test/network/data
  //But if you're building this and creating the executable, which goes into the bin directory
  //in test/network/bin then you need to specify the path as "../data".
  pFileWorker = std::make_shared<bright::utils::FileWorker>("data/filelist");
  //pFileWorker = std::make_shared<bright::utils::FileWorker>("test/network/data/filelist");
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();

  load_client_configs();
  auto pServerHandler = std::make_shared<bright::network::ServerHandler>(clientName, "mypassword", pModelsController, npcContollers);
  auto pServerConnection = boost::make_shared<bright::network::ServerConnection>(service, pServerHandler);
  boost::asio::ip::tcp::endpoint ep( boost::asio::ip::address::from_string(ip), port );

  pInputContextManager->add_command_event_listener(pServerConnection);
  std::string controlsContexts = pFileWorker->get_file_contents("controls_contexts.cfg");
  pInputContextManager->initialize(controlsContexts);
  std::string controlConfig = pFileWorker->get_file_contents("controls_config.cfg");
  pInputContextManager->set_control_config(controlConfig);

  pContextManager = std::make_shared<bright::context::ContextManager>();

  pContextManager->create_windows_opengl_context(WndProc, "Powered By The Bright Engine", 1280, 768);

  auto pLoadersManager = std::make_shared<bright::graphics::LoadersManager>(pFileWorker);
  auto pRenderer = std::make_shared<bright::graphics::Renderer>();
  pLoadersManager->load();
 
  auto pShader = pLoadersManager->shaders("PER_FRAG_LIGHT_COLOR");

  auto planeGroupRenderInfo = bright::graphics::create_plane(5.0f);
  planeGroupRenderInfo.pShader_ = pShader;
  planeGroupRenderInfo.hasShader_ = true;
  planeGroupRenderInfo.cameraType_ = "1st";

  auto cubeGroupRenderInfo = bright::graphics::create_cube();
  cubeGroupRenderInfo.pShader_ = pShader;
  cubeGroupRenderInfo.hasShader_ = true;
  cubeGroupRenderInfo.cameraType_ = "1st";

  auto cubeGroupRenderInfo2 = bright::graphics::create_cube();
  cubeGroupRenderInfo2.pShader_ = pShader;
  cubeGroupRenderInfo2.hasShader_ = true;
  cubeGroupRenderInfo2.cameraType_ = "1st";

  auto cubeGroupRenderInfo3 = bright::graphics::create_cube();
  cubeGroupRenderInfo3.pShader_ = pShader;
  cubeGroupRenderInfo3.hasShader_ = true;
  cubeGroupRenderInfo3.cameraType_ = "1st";

  auto cubeGroupRenderInfo4 = bright::graphics::create_cube();
  cubeGroupRenderInfo4.pShader_ = pShader;
  cubeGroupRenderInfo4.hasShader_ = true;
  cubeGroupRenderInfo4.cameraType_ = "1st";

  cubeGroupRenderInfo.modToWorld_ = pModelsController->model_to_world_transformation_matrix();
  cubeGroupRenderInfo.actorRenderInfos_["cube"]->diffuseColor_ = glm::vec4(0.0f,0.0f,1.0f,1.0f);
  cubeGroupRenderInfo.actorRenderInfos_["cube"]->hasTexture_ = false;

  pCubeGroupRenderInfo2->modToWorld_ = monsterContollers["Leo"]->model_to_world_transformation_matrix();
  pCubeGroupRenderInfo2->actorRenderInfos_["cube"]->diffuseColor_ = glm::vec4(0.0f,0.0f,1.0f,1.0f);
  pCubeGroupRenderInfo2->actorRenderInfos_["cube"]->hasTexture_ = false;

  pCubeGroupRenderInfo3->modToWorld_ = monsterContollers["Monster1"]->model_to_world_transformation_matrix();
  pCubeGroupRenderInfo3->actorRenderInfos_["cube"]->diffuseColor_ = glm::vec4(0.0f,0.0f,1.0f,1.0f);
  pCubeGroupRenderInfo3->actorRenderInfos_["cube"]->hasTexture_ = false;

  pCubeGroupRenderInfo4->modToWorld_ = monsterContollers["Monster2"]->model_to_world_transformation_matrix();
  pCubeGroupRenderInfo4->actorRenderInfos_["cube"]->diffuseColor_ = glm::vec4(0.0f,0.0f,1.0f,1.0f);
  pCubeGroupRenderInfo4->actorRenderInfos_["cube"]->hasTexture_ = false;

  pModelsController->update( glm::vec3(0.0f,-5.0f,0.0f) );
  pPlaneGroupRenderInfo->modToWorld_ = pModelsController->model_to_world_transformation_matrix();
  pPlaneGroupRenderInfo->actorRenderInfos_["plane"]->diffuseColor_ = glm::vec4(0.0f,1.0f,0.0f,1.0f);
  pPlaneGroupRenderInfo->actorRenderInfos_["plane"]->hasTexture_ = false;

  auto pWorldInfo = std::make_shared<bright::graphics::WorldInfo>();
  pWorldInfo->world_to_cam_matrix( "1st", pModelsController->world_to_camera_transformation_matrix() );

  bright::graphics::Light ambientLight;
  ambientLight.lightDirection_ = glm::vec3(0.0f, 50.0f, 0.0f);
  ambientLight.lightIntensity_ = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
  pWorldInfo->ambient_light(ambientLight);

  bright::graphics::Light directionalLight;
  directionalLight.lightDirection_ = glm::vec3(0.0f, 30.0f, 100.0f);
  directionalLight.lightIntensity_ = glm::vec4(0.2f, 0.2f, 0.2f, 0.5f);
  pWorldInfo->directional_light(directionalLight);

  //Show and update the window
  pContextManager->show_window(false);
  pContextManager->initialize();
  pModelsController->update( glm::vec3(0.0f,0.0f,0.0f) );

  pServerConnection->start(ep);
  boost::thread thread(boost::bind(&boost::asio::io_service::run, &service));
  while( !pServerConnection->connected() ){
    int i = 0;
  }
  pServerConnection->login();
  while( !pServerConnection->logged_in() ){
    pServerConnection->process_messages();
  }

  MSG msg;
  //Enter the main message loop
  while (true){
    if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ){
      // Process the message
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else{
      pInputManager->notify();
	    pInputContextManager->notify();
      pServerConnection->send_commands();
      pServerConnection->process_messages();
      pContextManager->begin_rendering();

      pCubeGroupRenderInfo->modToWorld_ = pModelsController->model_to_world_transformation_matrix();
      pCubeGroupRenderInfo2->modToWorld_ = monsterContollers["Leo"]->model_to_world_transformation_matrix();
      pCubeGroupRenderInfo3->modToWorld_ = monsterContollers["Monster1"]->model_to_world_transformation_matrix();
      pCubeGroupRenderInfo4->modToWorld_ = monsterContollers["Monster2"]->model_to_world_transformation_matrix();
      pWorldInfo->world_to_cam_matrix( "1st", pModelsController->world_to_camera_transformation_matrix() );
      pRenderer->render(pPlaneGroupRenderInfo, pWorldInfo);
      pRenderer->render(pCubeGroupRenderInfo, pWorldInfo);
      pRenderer->render(pCubeGroupRenderInfo2, pWorldInfo);
      pRenderer->render(pCubeGroupRenderInfo3, pWorldInfo);
      pRenderer->render(pCubeGroupRenderInfo4, pWorldInfo);

      pContextManager->end_rendering();
    }
  }
  return (int)msg.wParam;

}


void load_client_configs(){

  std::string fileContents = pFileWorker->get_file_contents("client.cfg");
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
        ip = line.substr(3);
      }
      else if(line.substr(0,5) == "port="){
        portTemp = line.substr(5);
        port = std::stoi (portTemp,&sz);
      }
      else if(line.substr(0,11) == "clientname="){
        clientName = line.substr(11);
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