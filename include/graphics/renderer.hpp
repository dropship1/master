#ifndef BRIGHT_GRAPHICS_RENDERER_H
#define BRIGHT_GRAPHICS_RENDERER_H

#include <gl/glew.h>

#include "graphics/globalstructs.hpp"
#include "graphics/worldinfo.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <memory>

#include <stdlib.h>

namespace bright{

  namespace graphics{


/**
 * @ingroup graphics
 *
 * @brief The renderer
 * 
 * @details 
 * This class handles rendering using the current world data and the current actor to render via ActorRenderInfo
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class Renderer{
public:

/** 
 * @brief Construct a new default renderer.
 * @details
 */ 
  Renderer();
  
/** 
 * @brief Tell the renderer to render this actor in the world.
 * @param pActorRenderInfo All the relevant actor info needed to know how to render the actor.
 * @param pWorldData All the relevant world info needed to know how to render the actor in the world.
 */
  void render(std::shared_ptr<bright::graphics::ActorGroupRenderInfo> pActorRenderInfo, std::shared_ptr<WorldInfo> pWorldInfo);

private:

  
};


  }
}

#endif
  
