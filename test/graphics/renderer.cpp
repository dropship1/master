#include <windows.h>
#include <iostream>
#include <memory>
#include <algorithm>

#include "graphics/renderer.hpp"
#include "graphics/loadersmanager.hpp"
#include "graphics/globalstructs.hpp"

#include "base/globalstructs.hpp"

#include "context/contextmanager.hpp"
#include "context/context.hpp"

#include "base/actorrendercontroller.hpp"
#include "base/actorcontrolcontroller.hpp"


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

#include "windows/entrypointeventhandler.hpp"

//Enable this to have a console to use cout and cin on, for debugging
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")


std::shared_ptr<bright::context::ContextManager> pContextManager;
bright::base::ActorRenderController actorRenderController;

std::shared_ptr<bright::windows::EntryPointEventHandler> pEntryPointEventHandler;
std::shared_ptr<bright::input::InputManager> pInputManager;


class KeyboardListener: public bright::input::KeyboardEventListener{
public:
  KeyboardListener();
  void on_keyboard_event(std::shared_ptr<bright::input::KeyboardEvent> pKeyboardInputEvent);
  int w_;
  int a_;
  int s_;
  int d_;
};


class RawMouseListener: public bright::input::RawMouseEventListener{
public:
  RawMouseListener(): movedLeft(0), movedRight(0), movedUp(0), movedDown(0){}
  int movedLeft;
  int movedRight;
  int movedUp;
  int movedDown;
  void on_raw_mouse_event(std::shared_ptr<bright::input::RawMouseEvent> pRawMouseInputEvent);
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){ 

  pEntryPointEventHandler = bright::windows::EntryPointEventHandler::get_entry_point_event_handler();

  pInputManager = pEntryPointEventHandler->get_input_manager();
  auto pKeyboardListener = std::make_shared<KeyboardListener>();
  pInputManager->add_keyboard_event_listener(pKeyboardListener);

  auto pRawMouseListener = std::make_shared<RawMouseListener>();
  pInputManager->add_raw_mouse_event_listener(pRawMouseListener);

  pContextManager = pEntryPointEventHandler->get_context_manager();

  pContextManager->create_windows_opengl_context(WndProc, "Powered By The Bright Engine", 1280, 768);

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("data/files.fl");
  auto pLoadersManager = std::make_shared<bright::graphics::LoadersManager>(pFileWorker);
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();
  auto pRenderer = std::make_shared<bright::graphics::Renderer>();
  pLoadersManager->load();

  auto pColorShader = pLoadersManager->shaders("PER_FRAG_LIGHT_COLOR");
  auto pShader = pLoadersManager->shaders("PER_FRAG_LIGHT_TEXTURE");
  auto pTexture = pLoadersManager->textures("default.dds");

  auto planeGroupRenderInfo = bright::graphics::create_plane(5.0f);
  planeGroupRenderInfo.pShader_ = pColorShader;
  planeGroupRenderInfo.hasShader_ = true;
  planeGroupRenderInfo.cameraType_ = "1st";

  auto cubeGroupRenderInfo = bright::graphics::create_cube();
  cubeGroupRenderInfo.pShader_ = pShader;
  cubeGroupRenderInfo.hasShader_ = true;
  cubeGroupRenderInfo.cameraType_ = "1st";

  bright::base::ActorControlController controller1;

  bright::base::ActorRenderController renderController1;
  renderController1.update( glm::vec3(0.0f,5.0f,0.0f) );
  cubeGroupRenderInfo.modToWorld_ = renderController1.model_to_world_transformation_matrix();
  cubeGroupRenderInfo.actorRenderInfos_["cube"].diffuseColor_ = glm::vec4(0.0f,0.0f,1.0f,1.0f);
  cubeGroupRenderInfo.actorRenderInfos_["cube"].pTexture_ = pTexture;
  cubeGroupRenderInfo.actorRenderInfos_["cube"].hasTexture_ = true;

  renderController1.update( glm::vec3(0.0f,-5.0f,0.0f) );
  planeGroupRenderInfo.modToWorld_ = renderController1.model_to_world_transformation_matrix();
  planeGroupRenderInfo.actorRenderInfos_["plane"].diffuseColor_ = glm::vec4(0.0f,1.0f,0.0f,1.0f);

  bright::base::ActorRenderController controller;
  controller.update( glm::vec3(0.0f,5.0f,20.0f) );

  bright::graphics::WorldInfo worldInfo;
  worldInfo.world_to_cam_matrix( "1st", controller.world_to_camera_transformation_matrix() );

  bright::graphics::Light ambientLight;
  ambientLight.lightDirection_ = glm::vec3(0.0f, 50.0f, 0.0f);
  ambientLight.lightIntensity_ = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
  worldInfo.ambient_light(ambientLight);

  bright::graphics::Light directionalLight;
  directionalLight.lightDirection_ = glm::vec3(0.0f, 30.0f, 100.0f);
  directionalLight.lightIntensity_ = glm::vec4(0.2f, 0.2f, 0.2f, 0.5f);
  worldInfo.directional_light(directionalLight);

  //Show and update the window
  pContextManager->show_window(false);
  pContextManager->initialize();
  pContextManager->show_cursor(false);
  pContextManager->constrain_cursor();

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
      if(pKeyboardListener->w_ > 0){
        for (int a = 0; a < pKeyboardListener->w_; ++a) {
          controller1.move_fwd(1.0f);
        }
      }
      if(pKeyboardListener->s_ > 0){
        for (int a = 0; a < pKeyboardListener->s_; ++a) {
          controller1.move_backward(1.0f);
        }
      }
      if(pKeyboardListener->a_ > 0){
        for (int a = 0; a < pKeyboardListener->a_; ++a) {
          controller1.move_left(1.0f);
        }
      }
      if(pKeyboardListener->d_ > 0){
        for (int a = 0; a < pKeyboardListener->d_; ++a) {
          controller1.move_right(1.0f);
        }
      }
      if(pRawMouseListener->movedDown > 0){
        for (int a = 0; a < pRawMouseListener->movedDown; ++a) {
          controller1.rotate_down(0.1f);
        }
        pRawMouseListener->movedDown = 0;
      }
      else if(pRawMouseListener->movedUp > 0){
        for (int a = 0; a < pRawMouseListener->movedUp; ++a) {
          controller1.rotate_up(0.1f);
        }
        pRawMouseListener->movedUp = 0;
      }
      else if(pRawMouseListener->movedLeft > 0){
        for (int a = 0; a < pRawMouseListener->movedLeft; ++a) {
          controller1.rotate_left(0.1f);
        }
        pRawMouseListener->movedLeft = 0;
      }
      else if(pRawMouseListener->movedRight > 0){
        for (int a = 0; a < pRawMouseListener->movedRight; ++a) {
          controller1.rotate_right(0.1f);
        }
        pRawMouseListener->movedRight = 0;
      }
      controller.update( controller1.pos(), controller1.right(), controller1.up(), controller1.look() );
      worldInfo.world_to_cam_matrix( "1st", controller.world_to_camera_transformation_matrix() );
      pContextManager->begin_rendering();
      pRenderer->render(planeGroupRenderInfo, worldInfo);
      pRenderer->render(cubeGroupRenderInfo, worldInfo);
      pContextManager->end_rendering();
    }
  }
  return (int)msg.wParam;

}


KeyboardListener::KeyboardListener(){
  w_ = 0;
  s_ = 0;
  a_ = 0;
  d_ = 0;
}


void KeyboardListener::on_keyboard_event(std::shared_ptr<bright::input::KeyboardEvent> pKeyboardInputEvent){

  if ( pKeyboardInputEvent->is_down() ){ 
    if (pKeyboardInputEvent->key_enum() == bright::input::Key::W){ 
      ++w_;
    }
    else if (pKeyboardInputEvent->key_enum() == bright::input::Key::S){ 
      ++s_;
    }
    if (pKeyboardInputEvent->key_enum() == bright::input::Key::A){ 
      ++a_;
    }
    else if (pKeyboardInputEvent->key_enum() == bright::input::Key::D){ 
      ++d_;
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
    if (pKeyboardInputEvent->key_enum() == bright::input::Key::W) {
      --w_;
    }
    else if (pKeyboardInputEvent->key_enum() == bright::input::Key::S) {
      --s_;
    }
    if (pKeyboardInputEvent->key_enum() == bright::input::Key::A) {
      --a_;
    }
    else if (pKeyboardInputEvent->key_enum() == bright::input::Key::D) {
      --d_;
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
  //  std::cout << "deltaWheel: " << pRawMouseInputEvent->delta_rate_wheel() << std::endl << std::flush;
  //}
  //
  if ( pRawMouseInputEvent->raw_mouse_event_type() == bright::input::MouseEventType::MOVEMENT){ 
    if (pRawMouseInputEvent->delta_rate_x() < 0){ 
      ++movedLeft;
    }
    else if (pRawMouseInputEvent->delta_rate_x() > 0){ 
      ++movedRight;
    }
    else if (pRawMouseInputEvent->delta_rate_y() < 0){ 
      ++movedUp;
    }
    else if (pRawMouseInputEvent->delta_rate_y() > 0){ 
      ++movedDown;
    }
  //  std::cout << "2 : MOVEMENT" << std::endl << std::flush; 
  //  std::cout << "deltaX: " << pRawMouseInputEvent->delta_rate_x() << std::endl << std::flush;
  //  std::cout << "deltaY: " << pRawMouseInputEvent->delta_rate_y() << std::endl << std::flush;
  }
  //
  //std::cout << std::endl << std::flush;

}