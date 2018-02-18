#ifndef BRIGHT_GRAPHICS_GLOBAL_STRUCTS_H
#define BRIGHT_GRAPHICS_GLOBAL_STRUCTS_H

#include <GL/glew.h>
#include <GL/wglew.h>

#include <vector>
#include <map>

#include <glm/glm.hpp>

#include "graphics/shader.hpp"
#include "graphics/texture.hpp"

namespace bright{

  namespace graphics{
  

enum CameraType {
  FIRSTPERSON, THIRDPERSON, FREE
};

struct Light{
  glm::vec3 lightDirection_;
  glm::vec4 lightIntensity_;
};


struct ShaderConfig{
  std::string vertexFileContents_;
  std::string fragmentFileContents_;
  std::string fragmentFilename_;
  std::string vertexFilename_;
  bool usePerpective_;
  bool hasTextures_;
  std::string textureUniformName_;
  std::string camToClipLocationName_;
  std::string type_;
};


struct TextureConfig{
  std::string filename_;
  std::string fileContents_;
  std::shared_ptr<std::stringstream> fileStringStream_;
};


struct ActorRenderInfo{
  ActorRenderInfo(std::shared_ptr<Texture> pTexture): pTexture_(pTexture){ 
    hasTexture_ = true;
    vertexCoordsSize_ = 0;
    diffuseColor_ = glm::vec4(1.0f,0.0f,0.0f,1.0f);
  }

  ActorRenderInfo(){ 
    hasTexture_ = false;
    vertexCoordsSize_ = 0;
    diffuseColor_ = glm::vec4(1.0f,0.0f,0.0f,1.0f);
  }

  GLuint vbo_;
  GLuint tbo_;
  GLuint nbo_;
  GLuint vao_;

  std::shared_ptr<Texture> pTexture_;

  int vertexCoordsSize_;

  glm::vec4 diffuseColor_;

  bool hasTexture_;

};

struct ActorGroupRenderInfo{
  ActorGroupRenderInfo(std::shared_ptr<Shader> pShader): pShader_(pShader){ 
    hasShader_  = true;
  }

  ActorGroupRenderInfo(){ 
    hasShader_  = false;
  }

  ActorRenderInfo& actor_render_info(std::string name){
    return actorRenderInfos_[name];
  }

  std::shared_ptr<Shader> pShader_;

  glm::mat4 modToWorld_;

  bool hasShader_;

  std::string name_;

  std::map<std::string, ActorRenderInfo> actorRenderInfos_;
  std::string cameraType_;

};


  }
}

#endif
