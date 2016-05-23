#ifndef BRIGHT_GRAPHICS_WORLD_INFO_H
#define BRIGHT_GRAPHICS_WORLD_INFO_H

#include "graphics/globalstructs.hpp"

#include <glm/glm.hpp>

#include <algorithm>

namespace bright{

  namespace graphics{

class WorldInfo{

public:
  WorldInfo();
  

  glm::mat4 world_to_cam_matrix(std::string cameraType);
  void world_to_cam_matrix(std::string cameraType, glm::mat4 worldToCamMatrix);

  bright::graphics::Light& directional_light();
  bright::graphics::Light& ambient_light();

  void directional_light(bright::graphics::Light directionalLight);
  void ambient_light(bright::graphics::Light ambientLight);

private:  
  std::map<std::string, glm::mat4> worldToCameraMatrices_;

  bright::graphics::Light directionalLight_;
  bright::graphics::Light ambientLight_;


};


  }
}

#endif