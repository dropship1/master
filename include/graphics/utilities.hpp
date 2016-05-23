#ifndef BRIGHT_GRAPHICS_UTILITIES_H
#define BRIGHT_GRAPHICS_UTILITIES_H

#include <map>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <memory>
#include "graphics/globalstructs.hpp"
#include "base/globalstructs.hpp"

#include <vector>

namespace bright{

  namespace graphics{

  std::shared_ptr<bright::graphics::ActorGroupRenderInfo> create_cube();
  void generate_cube_coords(std::shared_ptr<bright::base::ActorData> pCubeData);
  void generate_cube_normals_coords(std::shared_ptr<bright::base::ActorData> pCubeData);
  void generate_cube_texture_coords(std::shared_ptr<bright::base::ActorData> pCubeData);

  std::shared_ptr<bright::graphics::ActorGroupRenderInfo> create_plane(float size);
  void generate_plane_coords(std::shared_ptr<bright::base::ActorData> pPlaneData, float size);
  void generate_plane_normals_coords(std::shared_ptr<bright::base::ActorData> pPlaneData);
  void generate_plane_texture_coords(std::shared_ptr<bright::base::ActorData> pPlaneData);

  void load_data_into_graphics_memory(std::shared_ptr<bright::graphics::ActorRenderInfo> pActorRenderInfo, std::shared_ptr<bright::base::ActorData> pActorData);

  }
}

#endif