#include <windows.h>
#include <iostream>
#include "graphics/loadersmanager.hpp"
#include "graphics/globalstructs.hpp"
#include "context/contextmanager.hpp"
#include "context/context.hpp"
#include "windows/entrypointeventhandler.hpp"
#include <memory>
#include <algorithm>
#include <map>


std::shared_ptr<bright::windows::EntryPointEventHandler> pEntryPointEventHandler;

//Enable this to have a console to use cout and cin on, for debugging
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){ 


  pEntryPointEventHandler = bright::windows::EntryPointEventHandler::get_entry_point_event_handler();
  auto pContextManager = pEntryPointEventHandler->get_context_manager();

  pContextManager->create_windows_opengl_context(WndProc, "Powered By The Bright Engine", 1280, 768);

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("data/files.fl");
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();
  auto pLoadersManager = std::make_shared<bright::graphics::LoadersManager>(pFileWorker);
  pLoadersManager->load();

  auto pShader = pLoadersManager->shaders("PER_FRAG_LIGHT_TEXTURE");
  auto pTexture = pLoadersManager->textures("default.dds");
  
  //(Add more testing later, this is pretty simple verification)
  std::cout << "Shader: " << std::endl << std::flush;
  std::cout << "programID: " << pShader->program_id() << std::endl << std::flush;
  std::cout << "fragmentID_: " << pShader->fragment_id() << std::endl << std::flush;
  std::cout << "vertextID: " << pShader->vertex_id() << std::endl << std::flush;
  std::cout << "successStatus: " << pShader->success_status() << std::endl << std::flush;
  std::cout << "statusString: " << pShader->status_string() << std::endl << std::flush;
  std::cout << std::endl << std::flush;

  //Loop over all uniforms (Add more testing later, this is pretty simple verification)
  std::cout << "Uniforms:" << std::endl << std::flush;
  auto verify_uniform = [&] (std::string uniformName) { 
    unsigned int uniformLocation = pShader->uniform_location(uniformName);
    std::cout << "uniformName: " << uniformName << std::endl << std::flush;
    std::cout << "uniformLocation: " << uniformLocation << std::endl << std::flush;
    std::cout << std::endl << std::flush;
  };

  std::vector<std::string> uniforms;
  uniforms.push_back("dirToLight");
  uniforms.push_back("lightIntensity");
  uniforms.push_back("ambientIntensity");
  uniforms.push_back("colorTexture");
  uniforms.push_back("cameraToClipMatrix");
  uniforms.push_back("modelToWorldMatrix");
  uniforms.push_back("worldToCamMatrix");
  uniforms.push_back("normalToWorldMatrix");
  std::for_each(uniforms.begin(), uniforms.end(), verify_uniform);


  //(Add more testing later, this is pretty simple verification)
  std::cout << "Texture: " << std::endl << std::flush;
  std::cout << "textureID: " << pTexture->texture_id() << std::endl << std::flush;
  std::cout << "successStatus: " << pTexture->success_status() << std::endl << std::flush;
  std::cout << "statusString: " << pTexture->status_string() << std::endl << std::flush;
  std::cout << std::endl << std::flush;

  //Loop over all uniforms (Add more testing later, this is pretty simple verification)
  std::cout << "Samplers:" << std::endl << std::flush;
  std::cout << "numSamplers: " << pTexture->num_samplers() << std::endl << std::flush;
  auto verify_sampler = [&] (unsigned int samplerId) { 
    std::cout << "samplerID: " << samplerId << std::endl << std::flush;
    std::cout << std::endl << std::flush;
  };

  std::for_each(pTexture->samplers().begin(), pTexture->samplers().end(), verify_sampler);



  std::cout << "ctr+c to exit or x on the window" << std::endl;
  while (true){ }

}

