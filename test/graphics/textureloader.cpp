#include <windows.h>
#include <iostream>
#include "graphics/textureloader.hpp"
#include "graphics/texture.hpp"
#include "graphics/globalstructs.hpp"
#include "context/contextmanager.hpp"
#include "context/context.hpp"
#include "utils/fileworker.hpp"
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


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){ 


  auto pContextManager = std::make_shared<bright::context::ContextManager>();

  pContextManager->create_windows_opengl_context(WndProc, "Powered By The Bright Engine", 1280, 768);

  auto pTextureLoader = std::make_shared<bright::graphics::TextureLoader>();

  bright::graphics::TextureConfig textureConfig;
  textureConfig.filename_ = "dirt.dds";

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("data/files.fl");
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();

  textureConfig.fileContents_ = pFileWorker->get_file_contents("dirt.dds");
  auto texture = pTextureLoader->create_texture(textureConfig);

  //(Add more testing later, this is pretty simple verification)
  std::cout << "Texture: " << std::endl << std::flush;
  std::cout << "textureID: " << texture.texture_id() << std::endl << std::flush;
  std::cout << "successStatus: " << texture.success_status() << std::endl << std::flush;
  std::cout << "statusString: " << texture.status_string() << std::endl << std::flush;
  std::cout << std::endl << std::flush;


  std::map<std::string,unsigned int> uniformLocations_;

  //Loop over all uniforms (Add more testing later, this is pretty simple verification)
  std::cout << "Samplers:" << std::endl << std::flush;
  std::cout << "numSamplers: " << texture.num_samplers() << std::endl << std::flush;
  auto verify_sampler = [&] (unsigned int samplerId) { 
    std::cout << "samplerID: " << samplerId << std::endl << std::flush;
    std::cout << std::endl << std::flush;
  };

  std::for_each(texture.samplers().begin(), texture.samplers().end(), verify_sampler);

  std::cout << "ctr+c to exit or x on the window" << std::endl;
  while (true){ }

}

