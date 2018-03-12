#include <windows.h>

#include "input/inputmanager.hpp"
#include "input/keyboardeventlistener.hpp"
#include "input/keyboardevent.hpp"
#include "input/rawmouseeventlistener.hpp"
#include "input/rawmouseevent.hpp"
#include "input/mouseeventlistener.hpp"
#include "input/mouseevent.hpp"
#include "input/utils.hpp"
#include "context/contextmanager.hpp"
#include "context/context.hpp"
#include "input/globalstructs.hpp"
#include "audio/soundmanager.hpp"
#include "utils/fileworker.hpp"
#include "windows/entrypointeventhandler.hpp"


//Enable this to have a console to use cout and cin on, for debugging
//#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

std::shared_ptr<bright::windows::EntryPointEventHandler> pEntryPointEventHandler;
std::shared_ptr<bright::context::ContextManager> pContextManager;
std::shared_ptr<bright::input::InputManager> pInputManager;
std::shared_ptr<bright::audio::SoundManager> pSoundManager;

class KeyboardListener: public bright::input::KeyboardEventListener{
public:
  KeyboardListener();
  void on_keyboard_event(std::shared_ptr<bright::input::KeyboardEvent> pKeyboardInputEvent);
  bool shiftDown_;
  bool capsLockDown_;
};


class RawMouseListener: public bright::input::RawMouseEventListener{
  void on_raw_mouse_event(std::shared_ptr<bright::input::RawMouseEvent> pRawMouseInputEvent);
};



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){ 

  MSG msg;

  pEntryPointEventHandler = bright::windows::EntryPointEventHandler::get_entry_point_event_handler();
  auto pContextManager = pEntryPointEventHandler->get_context_manager();

  pInputManager = pEntryPointEventHandler->get_input_manager();
  auto pKeyboardListener = std::make_shared<KeyboardListener>();
  pInputManager->add_keyboard_event_listener(pKeyboardListener);

  auto pRawMouseListener = std::make_shared<RawMouseListener>();
  pInputManager->add_raw_mouse_event_listener(pRawMouseListener);

  pContextManager = pEntryPointEventHandler ->get_context_manager();
  pContextManager->create_windows_opengl_context(WndProc, "Powered By The Bright Engine", 1280, 768);
  //Show and update the window
  pContextManager->show_window(false);
  //pContextManager->constrain_cursor();
  pContextManager->initialize();

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("data/files.fl");
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();

  std::string fileContents = pFileWorker->get_file_contents("cat9.wav");  
  std::stringstream stream(fileContents);


  pSoundManager = std::make_shared<bright::audio::SoundManager>();
  pSoundManager->initialize();
  pSoundManager->create_wav_source("meow2", stream);
  pSoundManager->set_source_direction("meow2", 1.0, 0.0, 0.0);
  pSoundManager->set_source_position("meow2", -5.0, 0.0, 0.0);


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

KeyboardListener::KeyboardListener(){
  shiftDown_ = false;
  capsLockDown_ = false;
}


void KeyboardListener::on_keyboard_event(std::shared_ptr<bright::input::KeyboardEvent> pKeyboardInputEvent){

  if ( pKeyboardInputEvent->is_down() ){ 
    if (pKeyboardInputEvent->key_enum() == bright::input::Key::W){
      pSoundManager->play_source("meow2"); 
    }

    if (pKeyboardInputEvent->key_enum() == bright::input::Key::F1){
    }
    else if (pKeyboardInputEvent->key_enum() == bright::input::Key::F2){ 
    }

    if (pKeyboardInputEvent->key_enum() == bright::input::Key::F4){
      exit(1);
    }

  }
  else {
    if (pKeyboardInputEvent->key_enum() == bright::input::Key::W){
    }
  }

}


void RawMouseListener::on_raw_mouse_event(std::shared_ptr<bright::input::RawMouseEvent> pRawMouseInputEvent){

  if ( pRawMouseInputEvent->raw_mouse_event_type() == bright::input::MouseEventType::BUTTON_CLICK){ 
    if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::MB_UNDEF){  }
    if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::LEFT){  }
    if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::RIGHT){  }
    if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::MIDDLE){  }
    if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::WHEEL){  }

    if ( pRawMouseInputEvent->is_raw_mouse_button_down() ){  }
    else {  }
  }

  if ( pRawMouseInputEvent->raw_mouse_event_type() == bright::input::MouseEventType::MOVEMENT){ 
  }

}