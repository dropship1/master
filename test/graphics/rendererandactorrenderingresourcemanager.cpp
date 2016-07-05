#include <windows.h>
#include <iostream>
#include "base/actorrenderingresourcemanager.hpp"
#include "graphics/renderer.hpp"
#include "graphics/globalstructs.hpp"
#include "base/globalstructs.hpp"
#include "context/contextmanager.hpp"
#include "context/context.hpp"
#include "base/actorcontrolcontroller.hpp"
#include "base/actorrendercontroller.hpp"
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


//Enable this to have a console to use cout and cin on, for debugging
//#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")


std::shared_ptr<bright::context::ContextManager> pContextManager;
std::shared_ptr<bright::base::ActorControlController> pClientController;
std::shared_ptr<bright::base::ActorControlController> pServerController;


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

class KeyboardListener: public bright::input::KeyboardEventListener{
public:
  KeyboardListener();
  void on_keyboard_event(std::shared_ptr<bright::input::KeyboardEvent> pKeyboardInputEvent);
  bool w_;
  bool a_;
  bool s_;
  bool d_;
};


class RawMouseListener: public bright::input::RawMouseEventListener{
  void on_raw_mouse_event(std::shared_ptr<bright::input::RawMouseEvent> pRawMouseInputEvent);
};

class MouseListener: public bright::input::MouseEventListener{
  void on_mouse_event(std::shared_ptr<bright::input::MouseEvent> pMouseInputEvent);
};




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
        if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE){
          int xPosRelative = raw->data.mouse.lLastX;
          int yPosRelative = raw->data.mouse.lLastY;
          pInputManager->update_raw_mouse_listeners_deltas(xPosRelative, yPosRelative);

          CURSORINFO cursorInfo = { 0 };
          cursorInfo.cbSize = sizeof(cursorInfo);
          GetCursorInfo(&cursorInfo);
          float newXPos = cursorInfo.ptScreenPos.x;
          float newYPos = cursorInfo.ptScreenPos.y;
          //pInputManager->update_mouse_listeners_position(newXPos, newYPos);
          
          LPPOINT point = new POINT();
          GetCursorPos(point);

          ScreenToClient(pContextManager->windows_current_context()->window_id(), point);
          std::cout << "xPoint" << point->x << std::endl << std::flush;
          std::cout << "yPoint" << point->y << std::endl << std::flush;
  
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
  auto pKeyboardListener = std::make_shared<KeyboardListener>();
  pInputManager->add_keyboard_event_listener(pKeyboardListener);

  auto pRawMouseListener = std::make_shared<RawMouseListener>();
  pInputManager->add_raw_mouse_event_listener(pRawMouseListener);

  auto pMouseListener = std::make_shared<MouseListener>();
  pInputManager->add_mouse_event_listener(pMouseListener);

  pContextManager = std::make_shared<bright::context::ContextManager>();

  pContextManager->create_windows_opengl_context(WndProc, "Powered By The Bright Engine", 1280, 768);

  //If you're running this from the debuger/visual studio, then you need to specify the path to the 
  //data directory starting from the "bright" directory, for example:
  //test/graphics/data
  //But if you're building this and creating the executable, which goes into the bin directory
  //in test/graphics/bin then you need to specify the path as "../data".
  //auto pFileWorker = std::make_shared<bright::utils::FileWorker>("../data/files.fl");
  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("test/graphics/data/files.fl");
  auto pResourceManager = std::make_shared<bright::base::ActorRenderingResourceManager>(pFileWorker);
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();
  pResourceManager->initialize();
  auto pRenderer = std::make_shared<bright::graphics::Renderer>();

  auto pAltairGroupRenderInfo = pResourceManager->actor_group_render_infos()["Jake"];
  auto pPlaneGroupRenderInfo = bright::graphics::create_plane(5.0f);

  pServerController = std::make_shared<bright::base::>();
  pServerController->update_pos( glm::vec3(0.0f,-5.0f,0.0f) );

  pClientController = std::make_shared<bright::base::ClientController>();
  pClientController->update( pServerController->pos(), pServerController->right(), pServerController->up(), pServerController->look() );
  pPlaneGroupRenderInfo->modToWorld_ = pClientController->model_to_world_transformation_matrix();
  pPlaneGroupRenderInfo->pShader_ = pResourceManager->graphics_loader_manager().shaders("PER_FRAG_COLOR");
  pPlaneGroupRenderInfo->cameraType_ = "1st";
  pPlaneGroupRenderInfo->actorRenderInfos_["plane"]->diffuseColor_ = glm::vec4(0.0f,0.5f,0.0f,1.0f);

  auto pJakeController = pResourceManager->controllers()["Jake"];
  pJakeController->update( glm::vec3(0.0f,5.0f,10.0f) );
  pAltairGroupRenderInfo->modToWorld_ = pJakeController->model_to_world_transformation_matrix();

  auto pWorldInfo = pResourceManager->world_info();
  pServerController->update_pos( glm::vec3(0.0f,-5.0f,0.0f) );
  pClientController->update( pServerController->pos(), pServerController->right(), pServerController->up(), pServerController->look() );
  pWorldInfo->world_to_cam_matrix( "1st", pClientController->world_to_camera_transformation_matrix() );
 
  //Show and update the window
  pContextManager->show_window(false);
  pContextManager->initialize();

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
      if(pKeyboardListener->w_){
        pServerController->move_fwd(0.4f);
        pClientController->update( pServerController->pos() );
      }
      if(pKeyboardListener->s_){
        pServerController->move_backward(0.4f);
        pClientController->update( pServerController->pos() );
      }
      if(pKeyboardListener->a_){
        pServerController->move_left(0.4f);
        pClientController->update( pServerController->pos() );
      }
      if(pKeyboardListener->d_){
        pServerController->move_right(0.4f);
        pClientController->update( pServerController->pos() );
      }
      pWorldInfo->world_to_cam_matrix( "1st", pClientController->world_to_camera_transformation_matrix() );
      pContextManager->begin_rendering();
      pRenderer->render(pPlaneGroupRenderInfo, pWorldInfo);
      pRenderer->render(pAltairGroupRenderInfo, pWorldInfo);
      pContextManager->end_rendering();
    }
  }
  return (int)msg.wParam;

}


KeyboardListener::KeyboardListener(){
  w_ = false;
  s_ = false;
  a_ = false;
  d_ = false;
}


void KeyboardListener::on_keyboard_event(std::shared_ptr<bright::input::KeyboardEvent> pKeyboardInputEvent){

  if ( pKeyboardInputEvent->is_down() ){ 
    if (pKeyboardInputEvent->key_enum() == bright::input::Key::W){ 
      w_ = true;
    }
    else if (pKeyboardInputEvent->key_enum() == bright::input::Key::S){ 
      s_ = true;
    }
    if (pKeyboardInputEvent->key_enum() == bright::input::Key::A){ 
      a_ = true;
    }
    else if (pKeyboardInputEvent->key_enum() == bright::input::Key::D){ 
      d_ = true;
    }
    else if (pKeyboardInputEvent->key_enum() == bright::input::Key::F1){ 
      pContextManager->reshape_context(true,1920,1080);
    }
    else if (pKeyboardInputEvent->key_enum() == bright::input::Key::F2){ 
      pContextManager->reshape_context(false,1280,768);
    }
    else if (pKeyboardInputEvent->key_enum() == bright::input::Key::F4){ 
      pContextManager->exit();
    }

  }
  else {
    if (pKeyboardInputEvent->key_enum() == bright::input::Key::W){ 
      w_ = false;
    }
    else if (pKeyboardInputEvent->key_enum() == bright::input::Key::S){ 
      s_ = false;      
    }
    if (pKeyboardInputEvent->key_enum() == bright::input::Key::A){ 
      a_ = false;
    }
    else if (pKeyboardInputEvent->key_enum() == bright::input::Key::D){ 
      d_ = false;      
    }
  }
}


void RawMouseListener::on_raw_mouse_event(std::shared_ptr<bright::input::RawMouseEvent> pRawMouseInputEvent){
  //std::cout << "RawMouse Listener:" << std::endl << std::flush;
  //std::cout << "type: ";
  //if ( pRawMouseInputEvent->raw_mouse_event_type() == bright::input::MouseEventType::ME_UNDEF){ std::cout << "0 : ME_UNDEF" << std::endl << std::flush; }
  //
  //if ( pRawMouseInputEvent->raw_mouse_event_type() == bright::input::MouseEventType::BUTTON_CLICK){ 
  //  std::cout << "1 : BUTTON_CLICK" << std::endl << std::flush; 
  //  std::cout << "button: ";
  //  if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::MB_UNDEF){ std::cout << "0 : MB_UNDEF" << std::endl << std::flush; }
  //  if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::LEFT){ std::cout << "1 : LEFT" << std::endl << std::flush; }
  //  if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::RIGHT){ std::cout << "2 : RIGHT" << std::endl << std::flush; }
  //  if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::MIDDLE){ std::cout << "3 : MIDDLE" << std::endl << std::flush; }
  //  if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::WHEEL){ std::cout << "4 : WHEEL" << std::endl << std::flush; }
  //  std::cout << "upOrDown?: ";
  //  if ( pRawMouseInputEvent->is_raw_mouse_button_down() ){ std::cout << "Down" << std::endl << std::flush; }
  //  else { std::cout << "Up" << std::endl << std::flush; }
  //  std::cout << "deltaWheel: " << pRa4wMouseInputEvent->delta_rate_wheel() << std::endl << std::flush;
  //}
  //
  if ( pRawMouseInputEvent->raw_mouse_event_type() == bright::input::MouseEventType::MOVEMENT){ 
    if (pRawMouseInputEvent->delta_rate_x() < 0){ 
      pServerController->rotate_left(0.01f);
      pClientController->update( pServerController->pos(), pServerController->right(), pServerController->up(), pServerController->look() );
    }
    else if (pRawMouseInputEvent->delta_rate_x() > 0){ 
      pServerController->rotate_right(0.01f);
      pClientController->update( pServerController->pos(), pServerController->right(), pServerController->up(), pServerController->look() );
    }
    else if (pRawMouseInputEvent->delta_rate_y() < 0){ 
      pServerController->rotate_up(0.01f);
      pClientController->update( pServerController->pos(), pServerController->right(), pServerController->up(), pServerController->look() );
    }
    else if (pRawMouseInputEvent->delta_rate_y() > 0){ 
      pServerController->rotate_down(0.01f);
      pClientController->update( pServerController->pos(), pServerController->right(), pServerController->up(), pServerController->look() );
    }
  //  std::cout << "2 : MOVEMENT" << std::endl << std::flush; 
  //  std::cout << "deltaX: " << pRawMouseInputEvent->delta_rate_x() << std::endl << std::flush;
  //  std::cout << "deltaY: " << pRawMouseInputEvent->delta_rate_y() << std::endl << std::flush;
  }
  //
  //std::cout << std::endl << std::flush;

}


void MouseListener::on_mouse_event(std::shared_ptr<bright::input::MouseEvent> pMouseInputEvent){

  //std::cout << "Mouse Listener:" << std::endl << std::flush;
  //std::cout << "xPos: " << pMouseInputEvent->x_pos() << std::endl << std::flush;
  //std::cout << "yPos: " << pMouseInputEvent->y_pos() << std::endl << std::flush;
  //std::cout << std::endl << std::flush;

}