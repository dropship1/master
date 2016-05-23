#ifndef BRIGHT_GRAPHICS_TEXTURE_LOADER_H
#define BRIGHT_GRAPHICS_TEXTURE_LOADER_H


//External libraries
#include <GL/glew.h>
#include <GL/wglew.h>
#include "glm/glm.hpp"
#include <gli/gli.hpp>
#include <array>
#include "graphics/imageloader.hpp"
#include "graphics/globalstructs.hpp"
#include "graphics/texture.hpp"
#include <sstream>
#include <iostream>
#include <string>
#include <map>

namespace bright{

  namespace graphics{

/**
 * @ingroup graphics
 *
 * @brief The shader loader class
 * 
 * @details 
 * This is where all texture loading functionality resides
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class TextureLoader {

public:
  TextureLoader();
  
  std::shared_ptr<Texture> create_texture(std::shared_ptr<bright::graphics::TextureConfig> textureConfig);

private:
  void create_samplers(std::shared_ptr<Texture> pTexture);

  ImageLoader imageLoader_;
  std::map<std::string, std::shared_ptr<Texture>> textures_;


};

  }
}

#endif