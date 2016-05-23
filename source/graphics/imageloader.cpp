#include "graphics/imageloader.hpp"

using namespace bright::graphics;

ImageLoader::ImageLoader(){
}

std::shared_ptr<gli::texture2D> ImageLoader::load_single_dds_image(std::string path, std::string imageFile){

  std::string fullPathAndName = path+"/"+imageFile;
  auto pTexture2D = std::make_shared<gli::texture2D>( gli::load_dds( fullPathAndName.c_str() ) ); 

  return pTexture2D;
}


std::shared_ptr<gli::texture2D> ImageLoader::load_single_dds_image(std::stringstream& stream){

  auto pTexture2D = std::make_shared<gli::texture2D>( gli::load_dds(stream) ); 

  return pTexture2D;
}