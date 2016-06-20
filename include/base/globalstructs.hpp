#ifndef BRIGHT_BASE_GLOBAL_STRUCTS_H
#define BRIGHT_BASE_GLOBAL_STRUCTS_H

#include <vector>
#include <map>


#include <GL/glew.h>
#include <GL/wglew.h>
#include <map>
#include "glm/glm.hpp"
#include "input/globalstructs.hpp"
#include "graphics/globalstructs.hpp"

namespace bright{

  namespace base{
  

struct ActorData{

  std::vector<glm::vec4> vertexCoords_;
  std::vector<glm::vec3> normalCoords_;
  std::vector<glm::vec3> textureCoords_;

};

struct ActorConfig{
  ActorConfig(){ 
    renderType_  = "default";
    moveSpeed_ = glm::vec3(2.0f, 2.0f, 2.0f);
    rotateSpeed_ = glm::vec3(45.0f, 45.0f, 45.0f);
  }
  std::string renderType_;
  bright::input::InputBindings inputBindings_;
  //Meters per second
  glm::vec3 moveSpeed_;
  //Degrees per second
  glm::vec3 rotateSpeed_;
};


  }
}

#endif
