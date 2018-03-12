#ifndef BRIGHT_GRAPHICS_LOADERS_MANAGER_H
#define BRIGHT_GRAPHICS_LOADERS_MANAGER_H

//External libraries
#include <GL/glew.h>
#include <GL/wglew.h>
#include "glm/glm.hpp"
#include "graphics/imageloader.hpp"
#include "graphics/textureloader.hpp"
#include "graphics/shaderloader.hpp"
#include "graphics/globalstructs.hpp"
#include "utils/fileworker.hpp"

#include <iostream>
#include <string>
#include <map>

namespace bright{

  namespace graphics{

/**
 * @ingroup graphics
 *
 * @brief The loaders manager class
 * 
 * @details 
 * This is manages all the graphics loaders
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class LoadersManager {

public:
  LoadersManager(std::shared_ptr<bright::utils::FileWorker> pFileWorker);
  
  void load();

  std::shared_ptr<Texture> textures(std::string textureName);
  std::shared_ptr<Shader> shaders(std::string shaderName);

private:

  void load_textures(std::vector<TextureConfig>& textureConfigs);
  void load_shaders(std::vector<ShaderConfig>& shaderFileSets);

  void load_shader_configs(std::string fullPathAndName, std::vector<ShaderConfig>& shaderConfigs);
  void load_texture_configs(std::string fullPathAndName, std::vector<TextureConfig>& textureConfigs);

  void get_split_vertex_and_fragment_file_data(std::array<std::string, 2>& vertexAndFragmentFileData, std::string filename);

  TextureLoader textureLoader_;
  ShaderLoader shaderLoader_;

  std::map<std::string, std::shared_ptr<Texture>> textures_;
  std::map<std::string, std::shared_ptr<Shader>> shaders_;

  std::shared_ptr<bright::utils::FileWorker> pFileWorker_;

};

  }
}

#endif