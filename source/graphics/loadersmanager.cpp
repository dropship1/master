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

void bright::graphics::LoadersManager::get_split_vertex_and_fragment_file_data(std::array<std::string, 2>& vertexAndFragmentFileData, std::string filename)
{

  std::string fileContents = pFileWorker_->get_file_contents(filename);
  std::stringstream in(fileContents);

  std::string vertexFileData;
  std::string fragmentFileData;
  std::string line;
  bool inFragmentData = false;
  bool inVertexData = false;
  while (getline(in, line)) {
    if (line.substr(0, 17) == "//Fragment shader") {
      inFragmentData = true;
      inVertexData = false;
    }
    if (line.substr(0, 15) == "//Vertex shader") {
      inFragmentData = false;
      inVertexData = true;
    }
    if (inFragmentData) {
      fragmentFileData += "\n" + line;
    }
    else if (inVertexData) {
      vertexFileData += "\n" + line;
    }
  }

  vertexAndFragmentFileData[0] = vertexFileData;
  vertexAndFragmentFileData[1] = fragmentFileData;

}

void LoadersManager::load_shader_configs(std::string fullPathAndName, std::vector<ShaderConfig>& shaderConfigs){

  std::string vertexFileData;
  std::string fragmentFileData;
  std::string filename;
  bool usePerpective = false;
  bool hasTexture = false;
  std::string camToClipLocationName;
  std::string textureUniformName;
  std::string type;
  std::array<std::string, 2> vertexAndFragmentFileData;

  std::string fileContents = pFileWorker_->get_file_contents(fullPathAndName);
  std::stringstream in(fileContents);

  bool inShaderNode = false;
  std::string line; 
  while (getline(in, line)){
    if (line.substr(0,1) == "]"){
      break;
    }
    if (inShaderNode){
      if(line.substr(0,5) == "file="){
        filename = line.substr(5);
        get_split_vertex_and_fragment_file_data(vertexAndFragmentFileData, filename);
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
        shaderConfig.fragmentFilename_ = filename;
        shaderConfig.vertexFilename_ = filename;
        shaderConfig.vertexFileContents_ = vertexAndFragmentFileData[0];
        shaderConfig.fragmentFileContents_ = vertexAndFragmentFileData[1];
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