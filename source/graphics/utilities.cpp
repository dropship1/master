#include "graphics/utilities.hpp"

namespace bright{

  namespace graphics{



bright::graphics::ActorGroupRenderInfo create_cube(){ 

  bright::graphics::ActorGroupRenderInfo cubeGroupRenderInfo;

  bright::graphics::ActorRenderInfo cubeRenderInfo;
  cubeRenderInfo.hasTexture_ = false;

  auto pCubeData = std::make_shared<bright::base::ActorData>();    
  generate_cube_coords(pCubeData);
  generate_cube_normals_coords(pCubeData);
  generate_cube_texture_coords(pCubeData);

  load_data_into_graphics_memory(cubeRenderInfo, pCubeData);

  cubeGroupRenderInfo.actorRenderInfos_["cube"] = cubeRenderInfo;

  return cubeGroupRenderInfo;
}



/***********************************************************************************************
# CounterClickwise winding (-Z is into the screen as you look at the screen)
# Coords range from -1.0 to 1.0
***********************************************************************************************/
void generate_cube_coords(std::shared_ptr<bright::base::ActorData> pCubeData){
  
  //Top of cube (Y coords is highest (1.0))
  //RT
  glm::vec4 v1 = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);    
  //LT
  glm::vec4 v2 = glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
  //LB
  glm::vec4 v3 = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);                                             
  //RT
  glm::vec4 v4 = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);    
  //LB
  glm::vec4 v5 = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);                                             
  //RB
  glm::vec4 v6 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);  
  
  pCubeData->vertexCoords_.push_back(v1); pCubeData->vertexCoords_.push_back(v2); pCubeData->vertexCoords_.push_back(v3);
  pCubeData->vertexCoords_.push_back(v4); pCubeData->vertexCoords_.push_back(v5); pCubeData->vertexCoords_.push_back(v6);

  //Bottom of cube (Y Coords is lowest (-1.0))
  //RT
  v1 = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);    
  //LT
  v2 = glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);
  //LB
  v3 = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);                                             
  //RT
  v4 = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);    
  //LB
  v5 = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);                                             
  //RB
  v6 = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);  
  
  pCubeData->vertexCoords_.push_back(v1); pCubeData->vertexCoords_.push_back(v2); pCubeData->vertexCoords_.push_back(v3);
  pCubeData->vertexCoords_.push_back(v4); pCubeData->vertexCoords_.push_back(v5); pCubeData->vertexCoords_.push_back(v6); 
  
  //Left of cube (X Coords is leftmost (-1.0))
  //RT
  v1 = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);    
  //LT
  v2 = glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
  //LB
  v3 = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);                                             
  //RT
  v4 = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);    
  //LB
  v5 = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);                                             
  //RB
  v6 = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);  
  
  pCubeData->vertexCoords_.push_back(v1); pCubeData->vertexCoords_.push_back(v2); pCubeData->vertexCoords_.push_back(v3);
  pCubeData->vertexCoords_.push_back(v4); pCubeData->vertexCoords_.push_back(v5); pCubeData->vertexCoords_.push_back(v6); 

  //Right of cube (X Coords is rightmost (1.0))
  //RT
  v1 = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);    
  //LT
  v2 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  //LB
  v3 = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);                                             
  //RT
  v4 = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);    
  //LB
  v5 = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);                                             
  //RB
  v6 = glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);  
  
  pCubeData->vertexCoords_.push_back(v1); pCubeData->vertexCoords_.push_back(v2); pCubeData->vertexCoords_.push_back(v3);
  pCubeData->vertexCoords_.push_back(v4); pCubeData->vertexCoords_.push_back(v5); pCubeData->vertexCoords_.push_back(v6); 
  
  //Front of cube (Z Coords is away from us (In front of the class means as your
  //looking at something to move at it.. not away from it  (-1.0))
  //RT
  v1 = glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);    
  //LT
  v2 = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);
  //LB
  v3 = glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);                                             
  //RT
  v4 = glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);    
  //LB
  v5 = glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);                                             
  //RB
  v6 = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);  
  
  pCubeData->vertexCoords_.push_back(v1); pCubeData->vertexCoords_.push_back(v2); pCubeData->vertexCoords_.push_back(v3);
  pCubeData->vertexCoords_.push_back(v4); pCubeData->vertexCoords_.push_back(v5); pCubeData->vertexCoords_.push_back(v6); 

  //Back of cube (Z Coords is behind us (Back of the class means as your
  //looking at something to move backward away from it  (1.0))
  //RT
  v1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);    
  //LT
  v2 = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
  //LB
  v3 = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);                                             
  //RT
  v4 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);    
  //LB
  v5 = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);                                             
  //RB
  v6 = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);  
  
  pCubeData->vertexCoords_.push_back(v1); pCubeData->vertexCoords_.push_back(v2); pCubeData->vertexCoords_.push_back(v3);
  pCubeData->vertexCoords_.push_back(v4); pCubeData->vertexCoords_.push_back(v5); pCubeData->vertexCoords_.push_back(v6); 
  
}

/***********************************************************************************************
***********************************************************************************************/
void generate_cube_normals_coords(std::shared_ptr<bright::base::ActorData> pCubeData){

  //Top of cube
  glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal); 
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal);
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal);
  
  //Bottom of Cube
  normal = glm::vec3(0.0f, -1.0f, 0.0f);
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal); 
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal);
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal);
  
  //Left of Cube
  normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal); 
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal);
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal);
  
  //Right of Cube
  normal = glm::vec3(1.0f, 0.0f, 0.0f);
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal); 
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal);
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal);
  
  //Front of Cube
  normal = glm::vec3(0.0f, 0.0f, -1.0f);
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal); 
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal);
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal);
  
  //Back of Cube
  normal = glm::vec3(0.0f, 0.0f, 1.0f);
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal); 
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal);
  pCubeData->normalCoords_.push_back(normal); pCubeData->normalCoords_.push_back(normal);

}

/***********************************************************************************************
***********************************************************************************************/
void generate_cube_texture_coords(std::shared_ptr<bright::base::ActorData> pCubeData){

  //LB
  glm::vec3 lb = glm::vec3(0.0f, 0.0f, 1.0f); 
  //RB
  glm::vec3 rb = glm::vec3(1.0f, 0.0f, 1.0f); 
  //LT
  glm::vec3 lt = glm::vec3(0.0f, 1.0f, 1.0f);
  //RT
  glm::vec3 rt = glm::vec3(1.0f, 1.0f, 1.0f); 
  
  for (int i = 0; i < pCubeData->vertexCoords_.size(); i+=6){
    pCubeData->textureCoords_.push_back(rt); pCubeData->textureCoords_.push_back(lt); pCubeData->textureCoords_.push_back(lb); 
    pCubeData->textureCoords_.push_back(rt); pCubeData->textureCoords_.push_back(lb); pCubeData->textureCoords_.push_back(rb); 
  }

}


bright::graphics::ActorGroupRenderInfo create_plane(float size){ 

  bright::graphics::ActorGroupRenderInfo planeGroupRenderInfo;
  bright::graphics::ActorRenderInfo planeRenderInfo;
  planeRenderInfo.hasTexture_ = false;

  auto pPlaneData = std::make_shared<bright::base::ActorData>();
  generate_plane_coords(pPlaneData, size);
  generate_plane_normals_coords(pPlaneData);
  generate_plane_texture_coords(pPlaneData);

  load_data_into_graphics_memory(planeRenderInfo, pPlaneData);

  planeGroupRenderInfo.actorRenderInfos_["plane"] = planeRenderInfo;

  return planeGroupRenderInfo;
}


/***********************************************************************************************
# CounterClickwise winding (-Z is into the screen as you look at the screen)
# Coords range from -1.0 to 1.0
***********************************************************************************************/
void generate_plane_coords(std::shared_ptr<bright::base::ActorData> pPlaneData, float size){
  
  //Top of cube (Y coords is highest (1.0))
  //RT
  glm::vec4 v1 = glm::vec4((size * 10.0f), 0.0f, (size * -10.0f), 1.0f);    
  //LT
  glm::vec4 v2 = glm::vec4((size * -10.0f), 0.0f, (size * -10.0f), 1.0f);
  //LB
  glm::vec4 v3 = glm::vec4((size * -10.0f), 0.0f, (size * 10.0f), 1.0f);                                             
  //RT
  glm::vec4 v4 = glm::vec4((size * 10.0f), 0.0f, (size * -10.0f), 1.0f);    
  //LB
  glm::vec4 v5 = glm::vec4((size * -10.0f), 0.0f, (size * 10.0f), 1.0f);                                             
  //RB
  glm::vec4 v6 = glm::vec4((size * 10.0f), 0.0f, (size * 10.0f), 1.0f);  
  
  pPlaneData->vertexCoords_.push_back(v1); pPlaneData->vertexCoords_.push_back(v2); pPlaneData->vertexCoords_.push_back(v3);
  pPlaneData->vertexCoords_.push_back(v4); pPlaneData->vertexCoords_.push_back(v5); pPlaneData->vertexCoords_.push_back(v6);
  
}

/***********************************************************************************************
***********************************************************************************************/
void generate_plane_normals_coords(std::shared_ptr<bright::base::ActorData> pPlaneData){

  //Plane
  glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
  pPlaneData->normalCoords_.push_back(normal); pPlaneData->normalCoords_.push_back(normal); 
  pPlaneData->normalCoords_.push_back(normal); pPlaneData->normalCoords_.push_back(normal);
  pPlaneData->normalCoords_.push_back(normal); pPlaneData->normalCoords_.push_back(normal);
  
}

/***********************************************************************************************
***********************************************************************************************/
void generate_plane_texture_coords(std::shared_ptr<bright::base::ActorData> pPlaneData){

  //LB
  glm::vec3 lb = glm::vec3(0.0f, 0.0f, 1.0f); 
  //RB
  glm::vec3 rb = glm::vec3(1.0f, 0.0f, 1.0f); 
  //LT
  glm::vec3 lt = glm::vec3(0.0f, 1.0f, 1.0f);
  //RT
  glm::vec3 rt = glm::vec3(1.0f, 1.0f, 1.0f); 
  
  for (int i = 0; i < pPlaneData->vertexCoords_.size(); i+=6){
    pPlaneData->textureCoords_.push_back(rt); pPlaneData->textureCoords_.push_back(lt); pPlaneData->textureCoords_.push_back(lb); 
    pPlaneData->textureCoords_.push_back(rt); pPlaneData->textureCoords_.push_back(lb); pPlaneData->textureCoords_.push_back(rb); 
  }

}


void load_data_into_graphics_memory(bright::graphics::ActorRenderInfo& actorRenderInfo, std::shared_ptr<bright::base::ActorData> pActorData){ 

  glGenBuffers(1, &actorRenderInfo.vbo_);
  glGenBuffers(1, &actorRenderInfo.tbo_);
  glGenBuffers(1, &actorRenderInfo.nbo_);
  glGenVertexArrays(1, &actorRenderInfo.vao_);
  
  actorRenderInfo.vertexCoordsSize_ = pActorData->vertexCoords_.size();
  
  //MAKE VBOs
  glBindBuffer(GL_ARRAY_BUFFER, actorRenderInfo.vbo_);
  glBufferData(GL_ARRAY_BUFFER, pActorData->vertexCoords_.size()*sizeof(glm::vec4), &pActorData->vertexCoords_[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ARRAY_BUFFER, actorRenderInfo.nbo_);
  glBufferData(GL_ARRAY_BUFFER, pActorData->normalCoords_.size()*sizeof(glm::vec3), &pActorData->normalCoords_[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ARRAY_BUFFER, actorRenderInfo.tbo_);
  glBufferData(GL_ARRAY_BUFFER, pActorData->textureCoords_.size()*sizeof(glm::vec3), &pActorData->textureCoords_[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //Make VAO
  glBindVertexArray(actorRenderInfo.vao_);
  
  glBindBuffer(GL_ARRAY_BUFFER, actorRenderInfo.vbo_);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, actorRenderInfo.nbo_);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, actorRenderInfo.tbo_);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);

}


  }

}




