#include "graphics/loadersmanager.hpp"

using namespace bright::graphics;


LoadersManager::LoadersManager(std::shared_ptr<bright::utils::FileWorker> pFileWorker):
  pFileWorker_(pFileWorker), 
  pTextureLoader_( std::make_shared<TextureLoader>() ),
  pShaderLoader_( std::make_shared<ShaderLoader>() ){
}

//load_obj_file
void LoadersManager::load(){

  std::vector<std::shared_ptr<ShaderConfig>> shaderConfigs;
  load_shader_configs("shaders.res", shaderConfigs);
  load_shaders(shaderConfigs);

  std::vector<std::shared_ptr<TextureConfig>> textureConfigs;
  load_texture_configs("textures.res", textureConfigs);
  load_textures(textureConfigs);

  //"data/lights.cfg"

}

void LoadersManager::load_texture_configs(std::string fullPathAndName, std::vector<std::shared_ptr<TextureConfig>>& textureConfigs){

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
        auto pTextureConfig = std::make_shared<TextureConfig>();
        pTextureConfig->filename_ = filename;
        pTextureConfig->fileContents_ = pFileWorker_->get_file_contents(filename);
        pTextureConfig->fileStringStream_ = pFileWorker_->get_file_stringstream(filename);
        textureConfigs.push_back(pTextureConfig);
        inTextureNode = false;
      }
    }
    else if (line.substr(0,9) == "<Texture>"){ 
      inTextureNode = true;
    }
  }


}

void LoadersManager::load_shader_configs(std::string fullPathAndName, std::vector<std::shared_ptr<ShaderConfig>>& shaderConfigs){

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
        auto pShaderConfig = std::make_shared<ShaderConfig>();
        pShaderConfig->fragmentFilename_ = fragmentFilename;
        pShaderConfig->vertexFilename_ = vertexFilename;
        pShaderConfig->fragmentFileContents_ = pFileWorker_->get_file_contents(fragmentFilename);
        pShaderConfig->vertexFileContents_ = pFileWorker_->get_file_contents(vertexFilename);
        pShaderConfig->type_ = type;
        pShaderConfig->usePerpective_ = false;
        if (usePerpective){
          pShaderConfig->usePerpective_ = true;
          pShaderConfig->camToClipLocationName_ = camToClipLocationName;
        }
        pShaderConfig->hasTextures_ = false;
        if (hasTexture){
          pShaderConfig->hasTextures_ = true;
          pShaderConfig->textureUniformName_ = textureUniformName;
        }
        shaderConfigs.push_back(pShaderConfig);
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

void LoadersManager::load_textures(std::vector<std::shared_ptr<TextureConfig>>& textureConfigs){

  auto create_texture_from_config = [&] (std::shared_ptr<TextureConfig> pTextureConfig) { 
    auto pTexture = pTextureLoader_->create_texture(pTextureConfig);
    textures_[pTextureConfig->filename_] = pTexture;
  };
  std::for_each(textureConfigs.begin(), textureConfigs.end(), create_texture_from_config);

}


void LoadersManager::load_shaders(std::vector<std::shared_ptr<ShaderConfig>>& shaderConfigs){

  auto create_shader_from_config = [&] (std::shared_ptr<ShaderConfig> pShaderConfig) { 
    auto pShader = pShaderLoader_->load_single_shader_program(pShaderConfig);
    shaders_[pShaderConfig->type_] = pShader;
  };
  std::for_each(shaderConfigs.begin(), shaderConfigs.end(), create_shader_from_config);

}


std::shared_ptr<Texture> LoadersManager::textures(std::string textureName){
  return textures_[textureName];
}


std::shared_ptr<Shader> LoadersManager::shaders(std::string shaderName){
  return shaders_[shaderName];
}