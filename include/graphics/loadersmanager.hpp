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

  void load_textures(std::vector<std::shared_ptr<TextureConfig>>& textureConfigs);
  void load_shaders(std::vector<std::shared_ptr<ShaderConfig>>& shaderFileSets);

  void load_shader_configs(std::string fullPathAndName, std::vector<std::shared_ptr<ShaderConfig>>& shaderConfigs);
  void load_texture_configs(std::string fullPathAndName, std::vector<std::shared_ptr<TextureConfig>>& textureConfigs);

  std::shared_ptr<TextureLoader> pTextureLoader_;
  std::shared_ptr<ShaderLoader> pShaderLoader_;

  std::map<std::string, std::shared_ptr<Texture>> textures_;
  std::map<std::string, std::shared_ptr<Shader>> shaders_;
  std::shared_ptr<bright::utils::FileWorker> pFileWorker_;


};

  }
}

#endif