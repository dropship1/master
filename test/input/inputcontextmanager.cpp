#include <windows.h>

#include "input/inputmanager.hpp"
#include "input/keyboardeventlistener.hpp"
#include "input/keyboardevent.hpp"
#include "input/rawmouseeventlistener.hpp"
#include "input/rawmouseevent.hpp"
#include "input/mouseeventlistener.hpp"
#include "input/mouseevent.hpp"
#include "input/commandeventlistener.hpp"
#include "input/commandevent.hpp"
#include "input/utils.hpp"
#include "context/contextmanager.hpp"
#include "context/context.hpp"
#include "input/globalstructs.hpp"
#include "utils/fileworker.hpp"
#include "utils/utilities.hpp"

#include "input/inputcontextmanager.hpp"
#include <boost/algorithm/string/trim.hpp>
#include <boost/make_shared.hpp>
#include <vector>


class CommandListener: public bright::input::CommandEventListener{
public:
	CommandListener() {};
  void on_command_event(std::shared_ptr<bright::input::CommandEvent> pCommandInputEvent);
};

void CommandListener::on_command_event(std::shared_ptr<bright::input::CommandEvent> pCommandInputEvent){

  std::cout << "Command Listener:" << std::endl << std::flush;
  std::cout << "commandString: " << pCommandInputEvent->command_string() << std::endl << std::flush;
  std::cout << std::endl << std::flush;
}


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

//Enable this to have a console to use cout and cin on, for debugging
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

std::shared_ptr<bright::context::ContextManager> pContextManager;
std::shared_ptr<bright::input::InputManager> pInputManager;
std::shared_ptr<bright::input::InputContextManager> pInputContextManager;


LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam){
  // Route Windows messages to game engine member functions
  switch (msg){
    case WM_CREATE:{
      return 0;
    }

    case WM_SETFOCUS:{
      pContextManager->constrain_cursor();
      return 0;
    }

    case WM_KILLFOCUS:{
      return 0;
    }

    case WM_SIZE: {
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
          pInputManager->update_mouse_listeners_position(newXPos, newYPos);
          
          LPPOINT point = new POINT();
          GetCursorPos(point);

          ScreenToClient(pContextManager->windows_current_context()->window_id(), point);
          //std::cout << "xPoint" << point->x << std::endl << std::flush;
          //std::cout << "yPoint" << point->y << std::endl << std::flush;
  
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

  MSG msg;


  pInputManager = std::make_shared<bright::input::InputManager>();
  pInputContextManager = std::make_shared<bright::input::InputContextManager>();
  pInputManager->add_keyboard_event_listener(pInputContextManager);
  // TODO: Finish mouse events
  pInputManager->add_raw_mouse_event_listener(pInputContextManager);
  pInputManager->add_mouse_event_listener(pInputContextManager);

  auto pCommandListener = boost::make_shared<CommandListener>();
  pInputContextManager->add_command_event_listener(pCommandListener);


  //Initialize InputContextManager from config files
  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("test/input/data/filelist");
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();
  std::string controls_contexts = pFileWorker->get_file_contents("controls_contexts.cfg");
  std::cout << "THIS IS A TEST" << std::endl;
  std::cout << "--" << controls_contexts << "--" << std::endl;
  pInputContextManager->initialize(controls_contexts);
  system("PAUSE");
  std::string control_config = pFileWorker->get_file_contents("controls_config.cfg");
  std::cout << "DEBUG: control_config: " << control_config << std::endl;
  pInputContextManager->set_control_config(control_config);
  system("PAUSE");


  pContextManager = std::make_shared<bright::context::ContextManager>();
  pContextManager->create_windows_opengl_context(WndProc, "Powered By The Bright Engine", 1280, 768);
  //Show and update the window
  pContextManager->show_window(false);
  pContextManager->constrain_cursor();
  pContextManager->initialize();

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
      pContextManager->begin_rendering();
      pContextManager->end_rendering();
    }
  }
  return (int)msg.wParam;

}

