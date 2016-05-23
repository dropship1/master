#include <windows.h>

#include "context/context.hpp"
#include "context/contextmanager.hpp"
#include "context/openglcontext.hpp"

//Enable this to have a console to use cout and cin on, for debugging
//#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

std::shared_ptr<bright::context::ContextManager> pContextManager;
std::shared_ptr<bright::context::Context> pContext1;
std::shared_ptr<bright::context::Context> pContext2;
void toggle_context();

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

    case WM_DESTROY:{
      PostQuitMessage(0);
      return 0;
    }

    case WM_INPUT:{
      UINT dwSize = 40;
      static BYTE lpb[40];
    
      GetRawInputData((HRAWINPUT)lParam, RID_INPUT,lpb, &dwSize, sizeof(RAWINPUTHEADER));
    
      RAWINPUT* raw = (RAWINPUT*)lpb;

      if (raw->header.dwType == RIM_TYPEMOUSE) {
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_1_DOWN){
          toggle_context();
        }
      }
      return 0;
    }

  }
  return DefWindowProc(hWindow, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){ 

  MSG msg;

  pContextManager = std::make_shared<bright::context::ContextManager>();

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