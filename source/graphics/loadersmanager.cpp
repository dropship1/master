#include "graphics/loadersmanager.hpp"

using namespace bright::graphics;


LoadersManager::LoadersManager(std::shared_ptr<bright::utils::FileWorker> pFileWorker):
  pFileWorker_(pFileWorker), 
  textureLoader_(),
  shaderLoader_(){
}

//load_obj_file
void LoadersManager::load(){

  std::vector<ShaderConfig> shaderConfigs;
  load_shader_configs("shaders.res", shaderConfigs);
  load_shaders(shaderConfigs);

  std::vector<TextureConfig> textureConfigs;
  load_texture_configs("textures.res", textureConfigs);
  load_textures(textureConfigs);

  //"data/lights.cfg"

}

void LoadersManager::load_texture_configs(std::string fullPathAndName, std::vector<TextureConfig>& textureConfigs){

  std::string fileContents = pFileWorker_->get_file_contents(fullPathAndName);
  std::stringstream in(fileContents);

  std::string filename;
  std::string path;

  bool inTextureNode = false;
  std::string line; 
  while (getline(in, line)){
    if (line.substr(0,1) == "]"){
      break;
    }
    if (inTextureNode){
      if(line.substr(0,5) == "file="){
        filename = line.substr(5);
      }
      else if(line.substr(0,10) == "</Texture>"){
        TextureConfig textureConfig;
        textureConfig.filename_ = filename;
        textureConfig.fileContents_ = pFileWorker_->get_file_contents(filename);
        textureConfig.fileStringStream_ = pFileWorker_->get_file_stringstream(filename);
        textureConfigs.push_back(textureConfig);
        inTextureNode = false;
      }
    }
    else if (line.substr(0,9) == "<Texture>"){ 
      inTextureNode = true;
    }
  }


}

void LoadersManager::load_shader_configs(std::string fullPathAndName, std::vector<ShaderConfig>& shaderConfigs){

  std::string vertexFilename;
  std::string fragmentFilename;
  bool usePerpective = false;
  bool hasTexture = false;
  std::string camToClipLocationName;
  std::string textureUniformName;
  std::string type;

  std::string fileContents = pFileWorker_->get_file_contents(fullPathAndName);
  std::stringstream in(fileContents);

  bool inShaderNode = false;
  std::string line; 
  while (getline(in, line)){
    if (line.substr(0,1) == "]"){
      break;
    }
    if (inShaderNode){
      if(line.substr(0,13) == "vertexshader="){
        vertexFilename = line.substr(13);
      }
      else if(line.substr(0,15) == "fragmentshader="){
        fragmentFilename = line.substr(15);
      }
      else if(line.substr(0,12) == "textureunit="){
        textureUniformName = line.substr(12);
        hasTexture = true;
      }
      else if(line.substr(0,12) == "perspective="){
        camToClipLocationName = line.substr(12);
        usePerpective = true;
      }
      else if(line.substr(0,5) == "type="){
        type = line.substr(5);
      }
      else if(line.substr(0,9) == "</Shader>"){
        ShaderConfig shaderConfig;
        shaderConfig.fragmentFilename_ = fragmentFilename;
        shaderConfig.vertexFilename_ = vertexFilename;
        shaderConfig.fragmentFileContents_ = pFileWorker_->get_file_contents(fragmentFilename);
        shaderConfig.vertexFileContents_ = pFileWorker_->get_file_contents(vertexFilename);
        shaderConfig.type_ = type;
        shaderConfig.usePerpective_ = false;
        if (usePerpective){
          shaderConfig.usePerpective_ = true;
          shaderConfig.camToClipLocationName_ = camToClipLocationName;
        }
        shaderConfig.hasTextures_ = false;
        if (hasTexture){
          shaderConfig.hasTextures_ = true;
          shaderConfig.textureUniformName_ = textureUniformName;
        }
        shaderConfigs.push_back(shaderConfig);
        usePerpective = false;
        hasTexture = false;
        inShaderNode = false;
      }
    }
    else if (line.substr(0,8) == "<Shader>"){ 
      inShaderNode = true;
    }
  }


}

void LoadersManager::load_textures(std::vector<TextureConfig>& textureConfigs){

  auto create_texture_from_config = [&] (TextureConfig& textureConfig) { 
    auto texture = textureLoader_.create_texture(textureConfig);
    textures_[textureConfig.filename_] = std::make_shared<bright::graphics::Texture>(texture);
  };
  std::for_each(textureConfigs.begin(), textureConfigs.end(), create_texture_from_config);

}


void LoadersManager::load_shaders(std::vector<ShaderConfig>& shaderConfigs){

  auto create_shader_from_config = [&] (ShaderConfig& shaderConfig) { 
    auto shader = shaderLoader_.load_single_shader_program(shaderConfig);
    shaders_[shaderConfig.type_] = std::make_shared<bright::graphics::Shader>(shader);
  };
  std::for_each(shaderConfigs.begin(), shaderConfigs.end(), create_shader_from_config);

}


std::shared_ptr<Texture> LoadersManager::textures(std::string textureName){
  return textures_[textureName];
}


std::shared_ptr<Shader> LoadersManager::shaders(std::string shaderName){
  return shaders_[shaderName];
}