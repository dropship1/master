#ifndef BRIGHT_GRAPHICS_IMAGE_LOADER_H
#define BRIGHT_GRAPHICS_IMAGE_LOADER_H

#include <gli/gli.hpp>
#include <memory>
#include <string>

namespace bright{

  namespace graphics{

/**
 * @ingroup graphics
 *
 * @brief The imageloader class
 * 
 * @details 
 * This is where all image loading functionality resides
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class ImageLoader{

public:

  ImageLoader();

  std::shared_ptr<gli::texture2D> load_single_dds_image(std::string path, std::string imageFile);
  std::shared_ptr<gli::texture2D> load_single_dds_image(std::stringstream& stream);
  
private:


};

  }
}

#endif