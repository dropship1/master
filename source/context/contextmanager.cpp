#include "context/contextmanager.hpp"

using namespace bright::context;


ContextManager::ContextManager(){}


ContextManager::~ContextManager() {

  //Remove the rendering context from our device context
  wglMakeCurrent( pCurrentContext_->window_device_context(), 0 ); 
  
  //Delete our rendering context
  wglDeleteContext( pCurrentContext_->window_rendering_context() ); 
  
  //After this call our WindowIdentifier will no longer have a Device (Graphics card)
  //connected to it 
  ReleaseDC( pCurrentContext_->window_id(), pCurrentContext_->window_device_context() ); 

}


std::shared_ptr<Context> ContextManager::windows_current_context(){
  return pCurrentContext_;
}


HGLRC ContextManager::create_windows_opengl_rendering_context(HDC deviceContext){
  
  HGLRC renderingContext;

  //An application can only set the pixel format of a window one time. Once a window's pixel 
  //format is set, it cannot be changed.
  PIXELFORMATDESCRIPTOR  pixelFormatDescriptor;
  memset(&pixelFormatDescriptor, 0, sizeof(pixelFormatDescriptor)); 
  pixelFormatDescriptor.nSize      = sizeof(PIXELFORMATDESCRIPTOR); 
  pixelFormatDescriptor.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; 
  pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA; 
  pixelFormatDescriptor.cColorBits = 32; 
  pixelFormatDescriptor.cDepthBits = 32; 
  pixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE; 
  
  //Check if our pixelFormatDescriptor is valid and get a pixel format back
  int pixelFormat = ChoosePixelFormat(deviceContext, &pixelFormatDescriptor); 
  if (!pixelFormat){
    return false;
  }
  //Try and set the pixel format based on our pixelFormatDescriptor
  bool result = SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDescriptor); 
  if (!result){
    return false;
  }
  
  //Create an OpenGL 2.1 context for our device context
  //The wglCreateContext function creates a new OpenGL rendering context, which is suitable 
  //for drawing on the device referenced by deviceContext.
  //The rendering context has the same pixel format as the device context.
  HGLRC depracatedRenderingContext = wglCreateContext(deviceContext);
  wglMakeCurrent(deviceContext, depracatedRenderingContext); 

  //And now we will go ahead and enable GLEW so that we can use our new OpenGL modern 
  //context creation extension.
  glewExperimental=TRUE;
  GLenum error = glewInit(); 
  if (error != GLEW_OK){
    return false;
  }

  //Setting up attributes that we will use to create our modern opengl context
  //Set the major version
  //Set the minor version
  //Set our opengl context (rendering context) to be forward compatible
  std::array<int, 2> pGlVersion = opengl_version_info();
  int  majorVersion = pGlVersion[0];
  int  minorVersion = pGlVersion[1];
  int attributes[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, majorVersion, 
    WGL_CONTEXT_MINOR_VERSION_ARB, minorVersion, 
    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, 0 
  };
  
   //If the OpenGL modern  context creation extension is available
   //Create and OpenGL modern context based on the attributes above
   //Remove the deprecated context from being active
   //Delete the deprecated context
   //Make our OpenGL modern context current
  if (wglewIsSupported("WGL_ARB_create_context") == 1) {
    renderingContext = wglCreateContextAttribsARB(deviceContext, NULL, attributes); 
    wglMakeCurrent(NULL, NULL); 
    wglDeleteContext(depracatedRenderingContext); 
    wglMakeCurrent(deviceContext, renderingContext); 
  }
  else {
    //Didn't have support for OpenGL modern context , use the OpenGL 2.1 context
    renderingContext = depracatedRenderingContext; 
  }
  
  return renderingContext;
}


HDC ContextManager::create_windows_device_context(HWND windowId){
   return GetDC(windowId);
}


std::shared_ptr<Context> ContextManager::create_windows_opengl_context( WNDPROC      proc,
                                                                        std::string  title       = "Powered By The Bright Engine",
                                                                        int          width       = 1280, 
                                                                        int          height      = 768 ){

  HINSTANCE instAppInstance = create_windows_application_instance();
  HWND windowId = create_windows_window(proc, title, width, height, false, instAppInstance);
  HDC deviceContext = create_windows_device_context(windowId);
  HGLRC renderingContext = create_windows_opengl_rendering_context(deviceContext);
  std::array<int, 2> pGlVersion = opengl_version_info();
  int majorVers = pGlVersion[0];
  int minorVers = pGlVersion[1];

  auto pContext = std::make_shared<Context>();
  pContext->set_windows_opengl_context_values( instAppInstance,
                                               windowId, 
                                               deviceContext, 
                                               renderingContext, 
                                               title, 
                                               majorVers, 
                                               minorVers,
                                               width, 
                                               height
                                              );

  pCurrentContext_ = pContext;
  contexts_[title] = pContext;
  return pContext;
}


HINSTANCE ContextManager::create_windows_application_instance(){
  return GetModuleHandle(NULL);
}


HWND ContextManager::create_windows_window( WNDPROC     proc,
                                            std::string title, 
                                            int         width, 
                                            int         height,
                                            bool        fullscreen,
                                            HINSTANCE   applicationInstance ){

  WNDCLASSEX window; 
  HWND windowId;

  std::string titleTemp = std::string(title.begin(), title.end());
  LPCSTR sw = titleTemp.c_str();
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
    MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
    return windowId;
  }
  
  DWORD style;
  style = update_window_size_current_context(false, width, height);

  windowId = CreateWindow( sw, wtchar, style ,0, 0, width, height, (HWND)NULL, (HMENU)NULL, applicationInstance, (LPVOID)NULL);

  if (!windowId){
    return false;
  }

  RAWINPUTDEVICE Rid[2];
  Rid[0].usUsagePage   = 0x01; 
  Rid[0].usUsage       = 0x02; 
  Rid[0].dwFlags       = RIDEV_NOLEGACY;    // adds HID mouse and also ignores legacy mouse messages
  Rid[0].hwndTarget    = windowId;

  Rid[1].usUsagePage = 0x01; 
  Rid[1].usUsage     = 0x06; 
  Rid[1].dwFlags     = RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
  Rid[1].hwndTarget  = windowId;

  if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) {
    //registration failed. Call GetLastError for the cause of the error
    //Pop Up A Message Box Letting User Know The Program Is Closing.
    MessageBox(NULL,"Program Will Now Close. Could not register mouse and keyboard","ERROR",MB_OK|MB_ICONSTOP);
    return windowId;			
  }

  return windowId;
}


void ContextManager::set_current_context(std::string windowId){
  pPreviousContext_ = pCurrentContext_;
  pCurrentContext_ = contexts_[windowId];
}

void ContextManager::show_window(bool hidePrevious){
  if (hidePrevious){
    HWND prevoiusWinId = pPreviousContext_->window_id();
    ShowWindow(prevoiusWinId, SW_HIDE);
  }
  HWND currentWinId = pCurrentContext_->window_id();
  ShowWindow(currentWinId, SW_SHOW);
  UpdateWindow(currentWinId);
}


void ContextManager::initialize(){
  pCurrentContext_->initialize();
}


void ContextManager::begin_rendering(){
  pCurrentContext_->begin_rendering();
}


void ContextManager::end_rendering(){
  pCurrentContext_->end_rendering();
}

std::array<int, 2> ContextManager::opengl_version_info(){

  int majorVersion = 0;
  int minorVersion = 1;
  pGlVersion_[majorVersion] = -1;
  pGlVersion_[minorVersion] = -1;
  //Were going to see what version we got, either the old 2.1 or a new modern version
  //Set up a array to hold the values for major and minor version that we retrieve 
  //These are avaliable for checking in our major and minor class variables 
  glGetIntegerv(GL_MAJOR_VERSION, &pGlVersion_[majorVersion]); 
  glGetIntegerv(GL_MINOR_VERSION, &pGlVersion_[minorVersion]); 

  return pGlVersion_;
}


void ContextManager::constrain_cursor(){
  //RECT clip;
  ////Get the dimensions of the application's window. 
  //GetWindowRect(pCurrentContext_->window_id(), &clip);  
	//clip.left += 5;
	//clip.top += 30;
	//clip.right -= 5;
	//clip.bottom -= 5;
  ////Confine the cursor to the application's window. 
  //ClipCursor(&clip); 


  RECT rect;
  GetClientRect(pCurrentContext_->window_id(), &rect);

  POINT ul;
  ul.x = rect.left;
  ul.y = rect.top;

  POINT lr;
  lr.x = rect.right;
  lr.y = rect.bottom;

  MapWindowPoints(pCurrentContext_->window_id(), nullptr, &ul, 1);
  MapWindowPoints(pCurrentContext_->window_id(), nullptr, &lr, 1);

  rect.left = ul.x;
  rect.top = ul.y;

  rect.right = lr.x;
  rect.bottom = lr.y;

  ClipCursor(&rect);

}

void ContextManager::reshape_context(bool isFullScreen, int width, int height){
  DWORD style = update_window_size_current_context(isFullScreen, width, height);
  if(isFullScreen){
    SetWindowLongPtr(pCurrentContext_->window_id(), GWL_STYLE, style | WS_VISIBLE);
    MoveWindow(pCurrentContext_->window_id(), 0, 0, width, height, TRUE);
  }
  else{
    SetWindowLongPtr(pCurrentContext_->window_id(), GWL_STYLE, style | WS_VISIBLE);\
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    AdjustWindowRect(&rect, WS_CAPTION | WS_POPUPWINDOW, FALSE);
    MoveWindow(pCurrentContext_->window_id(), 0, 0, rect.right-rect.left, rect.bottom-rect.top, TRUE);
  }
  pCurrentContext_->reshape_window(width, height);
}

void ContextManager::reshape_rendering_context_viewport(int width, int height){
  pCurrentContext_->reshape_window(width, height);
}

void ContextManager::remove_system_menu(){
  LONG_PTR style = GetWindowLongPtr(pCurrentContext_->window_id(), GWL_STYLE); //get current style
  SetWindowLongPtr(pCurrentContext_->window_id(), GWL_STYLE, style & ~WS_SYSMENU); //remove system menu 
}


void ContextManager::exit(){
  PostQuitMessage(0);
}

DWORD ContextManager::update_window_size_current_context(bool isFullscreen, int width, int height){

  DWORD style;
  if (isFullscreen){							
    //Windows Style
    style= WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;								
  }
  else{
    //Windows Style    
    style= WS_OVERLAPPEDWINDOW;	
  }

  if (isFullscreen){
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
    	if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","BRIGHT GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES){
    	}
    	else{
    		// Pop Up A Message Box Letting User Know The Program Is Closing.
    		MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);						
    	}
    }

  }

  return style;
}

int ContextManager::show_cursor(bool show) {
  if (show) {
    while (ShowCursor(TRUE) < 0);
  }
  else {
    while (ShowCursor(FALSE) >= 0);
  }
  return 1;
}




 