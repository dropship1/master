#include <windows.h>
#include <iostream>
#include "context/context.hpp"
#include <memory>
#include <algorithm>

//Enable this to have a console to use cout and cin on, for debugging
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

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
      return 0;
    }

    case WM_DESTROY:{
      PostQuitMessage(0);
      return 0;
    }

    case WM_KEYDOWN:{
      return 0;
    }
    case WM_KEYUP:{
      return 0;
    }

    case WM_MOUSEMOVE:{
      return 0;
    }

    case WM_LBUTTONDOWN:{
      return 0;
    }

    case WM_LBUTTONUP:{
      return 0;
    }

    case WM_INPUT:{
      return 0;
    }

  }
  return DefWindowProc(hWindow, msg, wParam, lParam);
}


HWND create_windows_window( WNDPROC     proc,
                            std::string title, 
                            int         width, 
                            int         height,
                            bool        fullscreen,
                            HINSTANCE   applicationInstance );


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){ 

  bright::context::Context context;

  HINSTANCE appInstance = GetModuleHandle(NULL);
  HWND windowId = create_windows_window( WndProc,
                                         "First Title", 
                                         1028, 
                                         768,
                                         false,
                                         appInstance );
  HDC deviceContext = GetDC(windowId);
  HGLRC renderingContext = NULL;
  
  context.set_windows_opengl_context_values( appInstance, 
                                             windowId, 
                                             deviceContext, 
                                             renderingContext,
                                             "First Title", 
                                             3,
                                             1,
                                             1028,
                                             768
                                            );

  //(Add more testing later, this is pretty simple verification)
  std::cout << "Context: " << std::endl << std::flush;
  std::cout << "deviceContext: " << context.window_device_context() << std::endl << std::flush;
  std::cout << "renderingContext: " << context.window_rendering_context() << std::endl << std::flush;
  std::cout << "windowID: " << context.window_id() << std::endl << std::flush;
  std::cout << "windowTitle: " << context.window_title() << std::endl << std::flush;
  std::cout << "windowWidth: " << context.window_width() << std::endl << std::flush;
  std::cout << "windowHeight " << context.window_height() << std::endl << std::flush;
  std::cout << std::endl << std::flush;
 
  std::cout << "the title is set to Second Title" << std::endl << std::flush;
  context.window_title( "Second Title" );
  std::cout << "windowTitle: " << context.window_title() << std::endl << std::flush;

  std::cout << "the width is set to 1029" << std::endl << std::flush;
  std::cout << "the height is set to 769" << std::endl << std::flush;
  context.reshape_window(1029, 769);
  std::cout << "windowWidth: " << context.window_width() << std::endl << std::flush;
  std::cout << "windowHeight " << context.window_height() << std::endl << std::flush;

  std::cout << "ctr+c to exit or x on the window" << std::endl;
  while (true){ }

}


HWND create_windows_window( WNDPROC     proc,
                            std::string title, 
                            int         width, 
                            int         height,
                            bool        fullscreen,
                            HINSTANCE   applicationInstance ){

  WNDCLASSEX window; 
  HWND windowId;

  std::wstring titleTemp = std::wstring(title.begin(), title.end());
  LPCWSTR sw = titleTemp.c_str();
  LPCTSTR wtchar = titleTemp.c_str();

  window.cbSize        = sizeof(window);     
  window.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  window.lpfnWndProc   = proc;
  window.cbClsExtra    = 0;
  window.cbWndExtra    = 0;
  window.hInstance     = applicationInstance;
  window.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
  window.hCursor       = LoadCursor(NULL, IDC_ARROW);
  window.hbrBackground = NULL;
  window.lpszMenuName  = NULL;
  window.lpszClassName = sw;
  window.hIconSm = (HICON) LoadImage( applicationInstance,
                                      MAKEINTRESOURCE(5),
                                      IMAGE_ICON, 
                                      GetSystemMetrics(SM_CXSMICON), 
                                      GetSystemMetrics(SM_CYSMICON), 
                                      LR_DEFAULTCOLOR
                                     ); 

  if (!RegisterClassEx(&window)) {
    MessageBox(NULL,L"Failed To Register The Window Class.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
    return windowId;
  }
  
  DWORD exStyle;
  DWORD style;
  if (fullscreen){
    //Window Extended Style
    exStyle=WS_EX_APPWINDOW;								
    //Windows Style
    style=WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    //Hide Mouse Pointer
    ShowCursor(FALSE);										
  }
  else{
    //Window Extended Style
    exStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    //Windows Style    
    style=WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	
  }

  if (fullscreen){
    //Device Mode
    DEVMODE screenSettings;								
    //Makes Sure Memory's Cleared
    memset(&screenSettings,0,sizeof(screenSettings));
    //Size Of The Devmode Structure
    screenSettings.dmSize=sizeof(screenSettings);		
    //Selected Screen Width
    screenSettings.dmPelsWidth	= width;				
    //Selected Screen Height
    screenSettings.dmPelsHeight	= height;	
    //Selected Bits Per Pixel    
    screenSettings.dmBitsPerPel	= 32;					
    screenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

    //Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
    if (ChangeDisplaySettings(&screenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL){
    	// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
    	if (MessageBox(NULL,L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",L"BRIGHT GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES){
    		fullscreen = false;		// Windowed Mode Selected.  fullscreen = FALSE
    	}
    	else{
    		// Pop Up A Message Box Letting User Know The Program Is Closing.
    		MessageBox(NULL,L"Program Will Now Close.",L"ERROR",MB_OK|MB_ICONSTOP);
    		return windowId;								
    	}
    }
  }

  windowId = CreateWindow( sw,wtchar,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,(HWND)NULL,(HMENU)NULL,applicationInstance,(LPVOID)NULL);

  if (!windowId){
    return false;
  }

  return windowId;

}