#include <windows.h>
#include <iostream>
#include "base/globalstructs.hpp"
#include "base/actorrenderingresourcemanager.hpp"
#include "base/actorcontrolcontroller.hpp"
#include "context/contextmanager.hpp"
#include "context/context.hpp"
#include "graphics/renderer.hpp"
#include <memory>
#include <algorithm>

//Enable this to have a console to use cout and cin on, for debugging
//#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")


std::shared_ptr<bright::context::ContextManager> pContextManager;



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
      pContextManager->reshape_rendering_context_viewport(LOWORD(lParam), HIWORD(lParam));
      return 0;
    }

    case WM_DESTROY:{
      PostQuitMessage(0);
      return 0;
    }

    case WM_QUIT:{
      return 0;
    }

  }
  return DefWindowProc(hWindow, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){ 

  pContextManager = std::make_shared<bright::context::ContextManager>();

  pContextManager->create_windows_opengl_context(WndProc, "Powered By The Bright Engine", 1280, 768);

  //If you're running this from the debuger/visual studio, then you need to specify the path to the 
  //data directory starting from the "bright" directory, for example:
  //test/graphics/data
  //But if you're building this and creating the executable, which goes into the bin directory
  //in test/graphics/bin then you need to specify the path as "../data".
  //auto pFileWorker = std::make_shared<bright::utils::FileWorker>("../data/files.fl");
  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("data/files.fl");
  auto pActorRenderingResourceManager = std::make_shared<bright::base::ActorRenderingResourceManager>(pFileWorker);
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();
  pActorRenderingResourceManager->initialize();
  auto pRenderer = std::make_shared<bright::graphics::Renderer>();

  auto actorRenderControllers = pActorRenderingResourceManager->actor_render_controllers();
  auto actorGroupRenderInfos = pActorRenderingResourceManager->actor_group_render_infos();
  bright::base::ActorControlController moveController;

  int count = 0;
  auto move_actor = [&](std::map<std::string, bright::base::ActorRenderController>::value_type& pair) {
    std::string actorRenderControllerName = pair.first;
    auto& actorRenderController = pair.second;
    moveController.move_left(4.0f);
    actorRenderController.update(moveController.pos(), moveController.right(), moveController.up(),   moveController.look());
    count += 8;
  };
  std::for_each(actorRenderControllers.begin(), actorRenderControllers.end(), move_actor);

  auto update_actor_render_info = [&](std::map<std::string, bright::graphics::ActorGroupRenderInfo>::value_type& pair) {
    std::string actorGroupRenderInfoName = pair.first;
    auto& actorGroupRenderInfo = pair.second;
    actorGroupRenderInfo.modToWorld_ = actorRenderControllers[actorGroupRenderInfo.name_].model_to_world_transformation_matrix();
  };
  std::for_each(actorGroupRenderInfos.begin(), actorGroupRenderInfos.end(), update_actor_render_info);

  bright::base::ActorRenderController controller;
  bright::base::ActorControlController moveController2;
  controller.update(moveController2.pos(), moveController2.right(), moveController2.up(), moveController2.look());
  bright::graphics::WorldInfo worldInfo;
  worldInfo.world_to_cam_matrix("1st", controller.world_to_camera_transformation_matrix());
  moveController2.rotate_left(180.0f);
  moveController2.move_backward(15.0f);
  moveController2.move_up(2.0f);
  moveController2.move_right(5.0f);
  //Show and update the window
  pContextManager->show_window(false);
  pContextManager->initialize();

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
      pContextManager->begin_rendering();

      controller.update(moveController2.pos(), moveController2.right(), moveController2.up(), moveController2.look());
      worldInfo.world_to_cam_matrix("1st", controller.world_to_camera_transformation_matrix());
      auto render_info = [&](std::map<std::string, bright::graphics::ActorGroupRenderInfo>::value_type& pair) {
        std::string actorGroupRenderInfoName = pair.first;
        auto& actorGroupRenderInfo = pair.second;
        pRenderer->render(actorGroupRenderInfo, worldInfo);
      };
      std::for_each(actorGroupRenderInfos.begin(), actorGroupRenderInfos.end(), render_info);

      pContextManager->end_rendering();
    }
  }
  return (int)msg.wParam;

}