#include "context/context.hpp"

using namespace bright::context;

Context::Context(): pOpenglContext_(std::make_shared<OpenglContext>()) {
}

void Context::set_windows_opengl_context_values( HINSTANCE applicationInstance, 
                                                 HWND windowId, 
                                                 HDC deviceContext, 
                                                 HGLRC renderingContext,
                                                 std::string title, 
                                                 int majorVersion, 
                                                 int minorVersion,
                                                 int width, 
                                                 int height){

  applicationInstance_ = applicationInstance;
  windowId_ = windowId; 
  deviceContext_ = deviceContext;
  renderingContext_ = renderingContext;
  title_= title;
  majorVersion_ = majorVersion;
  minorVersion_ = minorVersion;
  width_ = width;
  height_ = height;

}


HGLRC Context::window_rendering_context(){
  return renderingContext_;
}

HWND Context::window_id(){
  return windowId_;
}


std::string Context::window_title(){
  return title_;
}


void Context::window_title(std::string title){
  title_ = title;
}


HDC Context::window_device_context(){
  return deviceContext_;
}


void Context::reshape_window(int width, int height){
  pOpenglContext_->reshape_window(width, height);
  width_ = width;
  height_ = height;
}

void Context::initialize(){
  pOpenglContext_->initialize();
}

void Context::begin_rendering(){
  pOpenglContext_->begin_rendering();
}

void Context::end_rendering(){
  pOpenglContext_->swap_buffers(deviceContext_);
}

int Context::window_width(){
  return width_;
}


int Context::window_height(){
  return height_;
}

