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
#include "windows/entrypointeventhandler.hpp"
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

//Enable this to have a console to use cout and cin on, for debugging
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

std::shared_ptr<bright::context::ContextManager> pContextManager;
std::shared_ptr<bright::input::InputManager> pInputManager;
std::shared_ptr<bright::input::InputContextManager> pInputContextManager;
std::shared_ptr<bright::windows::EntryPointEventHandler> pEntryPointEventHandler;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){ 

  MSG msg;

  pEntryPointEventHandler = bright::windows::EntryPointEventHandler::get_entry_point_event_handler();

  pInputManager = pEntryPointEventHandler->get_input_manager();
  pInputContextManager = std::make_shared<bright::input::InputContextManager>();
  pInputManager->add_keyboard_event_listener(pInputContextManager);
  // TODO: Finish mouse events
  pInputManager->add_raw_mouse_event_listener(pInputContextManager);

  auto pCommandListener = boost::make_shared<CommandListener>();
  pInputContextManager->add_command_event_listener(pCommandListener);


  //Initialize InputContextManager from config files
  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("data/filelist");
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


  pContextManager = pEntryPointEventHandler->get_context_manager();
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

