#include "network/serverconnection.hpp"
#include <windows.h>
#include <iostream>
#include "graphics/renderer.hpp"
#include "graphics/loadersmanager.hpp"
#include "graphics/globalstructs.hpp"
#include "base/globalstructs.hpp"
#include "base/actorcontrolsresourcemanager.hpp"
#include "base/actorrenderingresourcemanager.hpp"
#include "context/contextmanager.hpp"
#include "context/context.hpp"
#include "base/actorcontrolcontroller.hpp"
#include <memory>
#include <algorithm>

#include "input/commandlistener.hpp"
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


#include "input/commandevent.hpp"
#include "input/inputcontextmanager.hpp"


//Enable this to have a console to use cout and cin on, for debugging
//#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

std::shared_ptr<bright::windows::EntryPointEventHandler> pEntryPointEventHandler;
std::shared_ptr<bright::context::ContextManager> pContextManager;
std::shared_ptr<bright::utils::FileWorker> pFileWorker;
std::shared_ptr<bright::input::InputContextManager> pInputContextManager;
std::shared_ptr<bright::input::InputManager> pInputManager;
std::shared_ptr<bright::input::CommandHandler> pCommandHandler;

boost::shared_ptr<bright::input::CommandListener> pCommandListenerLucy;
boost::shared_ptr<bright::input::CommandListener> pCommandListenerKane;
boost::shared_ptr<bright::input::CommandListener> pCommandListenerZombie1;
boost::shared_ptr<bright::input::CommandListener> pCommandListenerZombie2;
boost::shared_ptr<bright::input::CommandListener> pCommandListenerZombie3;



boost::asio::io_service service;


void update_render_controllers(
  std::map<std::string, bright::base::ActorRenderController>& renderControllers, 
  std::map<std::string, bright::base::ActorControlController>& npcControllers,
  std::map<std::string, bright::base::ActorControlController>& playerControllers);

void update_group_render_infos(
  std::map<std::string, bright::graphics::ActorGroupRenderInfo>& groupRenderInfos,
  std::map<std::string, bright::base::ActorRenderController>& renderControllers);

void render_all_group_render_infos(
  std::map<std::string, bright::graphics::ActorGroupRenderInfo>& groupRenderInfos,
  bright::graphics::Renderer& renderer,
  bright::graphics::WorldInfo& worldInfo);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){ 

  pEntryPointEventHandler = bright::windows::EntryPointEventHandler::get_entry_point_event_handler();
  pInputContextManager = std::make_shared<bright::input::InputContextManager>();
  pInputManager = pEntryPointEventHandler->get_input_manager();
  pInputManager->add_keyboard_event_listener(pInputContextManager);
  pInputManager->add_raw_mouse_event_listener(pInputContextManager);

  pContextManager = pEntryPointEventHandler->get_context_manager();
  pContextManager->create_windows_opengl_context(WndProc, "Powered By The Bright Engine", 1280, 768);

  pFileWorker = std::make_shared<bright::utils::FileWorker>("data/files.fl");
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();

  auto pActorControlResourceManager = std::make_shared<bright::base::ActorControlsResourceManager>(pFileWorker);
  pActorControlResourceManager->initialize();
  auto pActorRenderingResourceManager = std::make_shared<bright::base::ActorRenderingResourceManager>(pFileWorker);
  pActorRenderingResourceManager->initialize();
  auto& npcControllers = pActorControlResourceManager->npc_controllers();
  auto& playerControllers = pActorControlResourceManager->player_controllers();

  auto& actorRenderControllers = pActorRenderingResourceManager->actor_render_controllers();
  auto& actorGroupRenderInfos = pActorRenderingResourceManager->actor_group_render_infos();

  std::string clientName = pActorControlResourceManager->client_control_name();
  std::string ip = pActorControlResourceManager->ip();
  int port = pActorControlResourceManager->port();
  auto pActorCreator = std::make_shared<bright::base::ActorCreator>();
  pCommandHandler = std::make_shared<bright::input::CommandHandler>(playerControllers["Lucy"], actorRenderControllers["Lucy"], pActorCreator);
  auto pServerHandler = std::make_shared<bright::network::ServerHandler>(clientName, "mypassword", playerControllers, npcControllers);
  auto pServerConnection = boost::make_shared<bright::network::ServerConnection>(service, pServerHandler, pCommandHandler);
  boost::asio::ip::tcp::endpoint ep( boost::asio::ip::address::from_string(ip), port );

  //pCommandListenerLucy = boost::make_shared<bright::input::CommandListener>(playerControllers["Lucy"], actorRenderControllers["Lucy"]);
  //pCommandListenerKane = boost::make_shared<bright::input::CommandListener>(playerControllers["Kane"], actorRenderControllers["Kane"]);
  //pCommandListenerZombie1 = boost::make_shared<bright::input::CommandListener>(playerControllers["Zombie_001"], actorRenderControllers["Zombie_001"]);
  //pCommandListenerZombie2 = boost::make_shared<bright::input::CommandListener>(playerControllers["Zombie_002"], actorRenderControllers["Zombie_002"]);
  //pCommandListenerZombie3 = boost::make_shared<bright::input::CommandListener>(playerControllers["Zombie_003"], actorRenderControllers["Zombie_003"]);
  //pInputContextManager->add_command_event_listener(pCommandListenerLucy);
  //pInputContextManager->add_command_event_listener(pCommandListenerKane);
  //pInputContextManager->add_command_event_listener(pCommandListenerZombie1);
  //pInputContextManager->add_command_event_listener(pCommandListenerZombie2);
  //pInputContextManager->add_command_event_listener(pCommandListenerZombie3);

  pInputContextManager->add_command_event_listener(pServerConnection);
  std::string controlsContexts = pFileWorker->get_file_contents("contexts.ctrls");
  pInputContextManager->initialize(controlsContexts);
  std::string controlConfig = pFileWorker->get_file_contents("config.ctrls");
  pInputContextManager->set_control_config(controlConfig);

  auto renderer = bright::graphics::Renderer();

  auto worldInfo = pActorRenderingResourceManager->world_info();

  //Show and update the window
  pContextManager->show_window(false);
  pContextManager->initialize();

  pServerConnection->start(ep);
  boost::thread thread(boost::bind(&boost::asio::io_service::run, &service));
  while( !pServerConnection->connected() ){
    int i = 0;
  }
  pServerConnection->login();
  while( !pServerConnection->logged_in() ){
    pServerConnection->process_messages();
  }


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
	    pInputContextManager->notify();
      //pServerConnection->send_commands();
      pServerConnection->process_messages();
      pContextManager->begin_rendering();

      //auto& npcControllers = pActorControlResourceManager->npc_controllers();
      //auto& playerControllers = pActorControlResourceManager->player_controllers();
      //
      //auto actorRenderControllers = pActorRenderingResourceManager->actor_render_controllers();
      //auto actorGroupRenderInfos = pActorRenderingResourceManager->actor_group_render_infos();

      //update all render controllers with new control controllers info
      update_render_controllers(actorRenderControllers, npcControllers, playerControllers);

      //update all group render infos mod to worlds with render controllers data
      update_group_render_infos(actorGroupRenderInfos, actorRenderControllers);

      //update the world info with the players render controller you want to view the world from
      worldInfo.world_to_cam_matrix( "1st", actorRenderControllers["Lucy"].world_to_camera_transformation_matrix() );

      //render all group render infos
      render_all_group_render_infos(actorGroupRenderInfos, renderer, worldInfo);
      //pRenderer->render(zombie1GroupRenderInfo, worldInfo);

      pContextManager->end_rendering();
    }
  }
  return (int)msg.wParam;

}



void update_render_controllers(
  std::map<std::string, bright::base::ActorRenderController>& renderControllers,
  std::map<std::string, bright::base::ActorControlController>& npcControllers,
  std::map<std::string, bright::base::ActorControlController>& playerControllers){

  auto update_render_controller = [&](std::map<std::string, bright::base::ActorRenderController>::value_type& pair) {
    std::string name = pair.first;
    auto& renderController = pair.second;
    bool did_not_find_it = false;
    auto it = npcControllers.find(name);
    if (it == npcControllers.end()) {
      it = playerControllers.find(name);
      if (it == playerControllers.end()) {
        did_not_find_it = true;
      }
    }
    if (did_not_find_it == false){
      auto& controlController = it->second;
      renderController.update(controlController.pos(), controlController.right(), controlController.up(), controlController.look());
    }
  };
  std::for_each(renderControllers.begin(), renderControllers.end(), update_render_controller);

}


void update_group_render_infos(
  std::map<std::string, bright::graphics::ActorGroupRenderInfo>& groupRenderInfos,
  std::map<std::string, bright::base::ActorRenderController>& renderControllers) {

  auto update_group_render_info = [&](std::map<std::string, bright::graphics::ActorGroupRenderInfo>::value_type& pair) {
    std::string name = pair.first;
    auto& groupRenderInfo = pair.second;
    bool did_not_find_it = false;
    auto it = renderControllers.find(name);
    if (it == renderControllers.end()) {
      did_not_find_it = true;
    }
    if (did_not_find_it == false) {
      auto& renderController = it->second;
      groupRenderInfo.modToWorld_ = renderController.model_to_world_transformation_matrix();
    }
  };
  std::for_each(groupRenderInfos.begin(), groupRenderInfos.end(), update_group_render_info);

}

void render_all_group_render_infos(
  std::map<std::string, bright::graphics::ActorGroupRenderInfo>& groupRenderInfos,
  bright::graphics::Renderer& renderer,
  bright::graphics::WorldInfo& worldInfo) {

  auto update_group_render_info = [&](std::map<std::string, bright::graphics::ActorGroupRenderInfo>::value_type& pair) {
    std::string name = pair.first;
    auto& groupRenderInfo = pair.second;
    renderer.render(groupRenderInfo, worldInfo);
  };
  std::for_each(groupRenderInfos.begin(), groupRenderInfos.end(), update_group_render_info);

}

