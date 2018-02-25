#include <windows.h>
#include <iostream>
#include "graphics/shaderloader.hpp"
#include "graphics/shader.hpp"
#include "graphics/globalstructs.hpp"
#include "context/contextmanager.hpp"
#include "context/context.hpp"
#include "utils/fileworker.hpp"
#include <memory>
#include <algorithm>
#include "windows/entrypointeventhandler.hpp"

std::shared_ptr<bright::windows::EntryPointEventHandler> pEntryPointEventHandler;

//Enable this to have a console to use cout and cin on, for debugging
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){ 

  pEntryPointEventHandler = bright::windows::EntryPointEventHandler::get_entry_point_event_handler();
  auto pContextManager = pEntryPointEventHandler->get_context_manager();

  pContextManager->create_windows_opengl_context(WndProc, "Powered By The Bright Engine", 1280, 768);

  auto pShaderLoader = std::make_shared<bright::graphics::ShaderLoader>();

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("data/files.fl");
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();

  bright::graphics::ShaderConfig shaderConfig;
  shaderConfig.camToClipLocationName_ = "cameraToClipMatrix";
  shaderConfig.type_ = "PER_FRAG_LIGHT_COLOR";
  shaderConfig.fragmentFileContents_ = pFileWorker->get_file_contents("pflt.f");
  shaderConfig.vertexFileContents_ = pFileWorker->get_file_contents("pflt.v");
  shaderConfig.usePerpective_ = true;
  auto shader = pShaderLoader->load_single_shader_program(shaderConfig);

  //(Add more testing later, this is pretty simple verification)
  std::cout << "Shader: " << std::endl << std::flush;
  std::cout << "programID: " << shader.program_id() << std::endl << std::flush;
  std::cout << "fragmentID_: " << shader.fragment_id() << std::endl << std::flush;
  std::cout << "vertextID: " << shader.vertex_id() << std::endl << std::flush;
  std::cout << "successStatus: " << shader.success_status() << std::endl << std::flush;
  std::cout << "statusString: " << shader.status_string() << std::endl << std::flush;
  std::cout << std::endl << std::flush;
  
  std::map<std::string,unsigned int> uniformLocations_;

  //Loop over all uniforms (Add more testing later, this is pretty simple verification)
  std::cout << "Uniforms:" << std::endl << std::flush;
  auto verify_uniform = [&] (std::string uniformName) { 
    unsigned int uniformLocation = shader.uniform_location(uniformName);
    std::cout << "uniformName: " << uniformName << std::endl << std::flush;
    std::cout << "uniformLocation: " << uniformLocation << std::endl << std::flush;
    std::cout << std::endl << std::flush;
  };

  std::vector<std::string> uniforms;
  uniforms.push_back("diffuseColor");
  uniforms.push_back("dirToLight");
  uniforms.push_back("lightIntensity");
  uniforms.push_back("ambientIntensity");
  uniforms.push_back("cameraToClipMatrix");
  uniforms.push_back("modelToWorldMatrix");
  uniforms.push_back("worldToCamMatrix");
  uniforms.push_back("normalToWorldMatrix");
  std::for_each(uniforms.begin(), uniforms.end(), verify_uniform);


  std::cout << "ctr+c to exit or x on the window" << std::endl;
  while (true){ }

}