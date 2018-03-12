#include <windows.h>

#include "context/context.hpp"
#include "context/contextmanager.hpp"
#include "context/openglcontext.hpp"

#include "input/inputmanager.hpp"
#include "input/rawmouseeventlistener.hpp"
#include "input/rawmouseevent.hpp"

#include "windows/entrypointeventhandler.hpp"

//Enable this to have a console to use cout and cin on, for debugging
//#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

std::shared_ptr<bright::windows::EntryPointEventHandler> pEntryPointEventHandler;
std::shared_ptr<bright::context::ContextManager> pContextManager;
std::shared_ptr<bright::context::Context> pContext1;
std::shared_ptr<bright::context::Context> pContext2;
void toggle_context();

std::shared_ptr<bright::input::InputManager> pInputManager;


class RawMouseListener : public bright::input::RawMouseEventListener {
  void on_raw_mouse_event(std::shared_ptr<bright::input::RawMouseEvent> pRawMouseInputEvent);
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){ 

  MSG msg;

  pEntryPointEventHandler = bright::windows::EntryPointEventHandler::get_entry_point_event_handler();
  pContextManager = pEntryPointEventHandler->get_context_manager();

  pInputManager = pEntryPointEventHandler->get_input_manager();
  auto pRawMouseListener = std::make_shared<RawMouseListener>();
  pInputManager->add_raw_mouse_event_listener(pRawMouseListener);

  pContextManager->create_windows_opengl_context(WndProc, "Powered By The Bright Engine1", 1280, 768);
  pContextManager->initialize( );
  pContext1 = pContextManager->windows_current_context();

  pContextManager->create_windows_opengl_context(WndProc, "Powered By The Bright Engine2", 800, 600);
  pContextManager->initialize( );
  pContext2 = pContextManager->windows_current_context();

  //Show and update the window
  pContextManager->show_window(false);

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
      pContextManager->begin_rendering();
      pContextManager->end_rendering();
    }
  }
  return (int)msg.wParam;

}

void toggle_context(){
  static bool toggle = true;
  
  if(toggle){
    pContextManager->set_current_context( pContext1->window_title() );
    pContextManager->show_window(true);
    toggle = false;
  }
  else{
    pContextManager->set_current_context( pContext2->window_title() );
    pContextManager->show_window(true);
    toggle = true;
  }

}


void RawMouseListener::on_raw_mouse_event(std::shared_ptr<bright::input::RawMouseEvent> pRawMouseInputEvent) {
  if ( pRawMouseInputEvent->raw_mouse_event_type() == bright::input::MouseEventType::BUTTON_CLICK){ 
    if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::LEFT){ 
      if (pRawMouseInputEvent->is_raw_mouse_button_down()) { 
        toggle_context();
      }
      else { 
        //up
      }
    }
  }

}