#include "graphics/renderer.hpp"

using namespace bright::graphics;

Renderer::Renderer(){
}


void Renderer::render(ActorGroupRenderInfo& actorGroupRenderInfo, WorldInfo& worldInfo){
  
  glm::mat4 worldToCamMat = worldInfo.world_to_cam_matrix( actorGroupRenderInfo.cameraType_ );

  auto pShader = actorGroupRenderInfo.pShader_;

  glUseProgram( pShader->program_id() );
  glActiveTexture( GL_TEXTURE0 + pShader->texture_unit() );
  //Currently everything is controlled from the root/top level root actor
  //No controll per actor (group) yet
  glm::mat4 modToWorldMat = actorGroupRenderInfo.modToWorld_;
    int count = 0;
  auto render_actor = [&] (std::map<std::string, ActorRenderInfo>::value_type& childActorInfoPair) {
  
    auto childActorInfo = childActorInfoPair.second;

    auto pTexture = childActorInfo.pTexture_;

    glBindVertexArray(childActorInfo.vao_);
    
    //Upload Scene info to the Shader
    glUniformMatrix4fv( pShader->uniform_location("worldToCamMatrix"), 1, GL_FALSE, glm::value_ptr( worldToCamMat ) );
    glUniform4fv( pShader->uniform_location("ambientIntensity"), 1, glm::value_ptr(worldInfo.ambient_light().lightIntensity_) );
    glUniform3fv( pShader->uniform_location("dirToLight"), 1, glm::value_ptr(worldInfo.directional_light().lightDirection_) );
    glUniform4fv( pShader->uniform_location("lightIntensity"), 1, glm::value_ptr(worldInfo.directional_light().lightIntensity_) );
    
    glm::mat3 normToWorldMat3( modToWorldMat );
    
    //Upload info to the Shader
    glUniformMatrix4fv( pShader->uniform_location("modelToWorldMatrix"), 1, GL_FALSE, glm::value_ptr( modToWorldMat ) );
    glUniformMatrix3fv( pShader->uniform_location("normalToWorldMatrix"), 1, GL_FALSE, glm::value_ptr(normToWorldMat3) );

    if ( !childActorInfo.hasTexture_ ){
      glUniform4fv( pShader->uniform_location("diffuseColor"), 1, glm::value_ptr( childActorInfo.diffuseColor_ ) );
    }
    

    if ( childActorInfo.hasTexture_ ){
      ++count;
      //if(count == 6){
        glBindTexture( GL_TEXTURE_2D, pTexture->texture_id() );
        glBindSampler(pShader->texture_unit(), pTexture->get_sampler_id(4));
        glUniform1i( pShader->uniform_location("colorTexture"), pShader->texture_unit() );
      //}
    }
    
    glDrawArrays( GL_TRIANGLES, 0, childActorInfo.vertexCoordsSize_ );
    
    if ( childActorInfo.hasTexture_ ){
      glBindSampler(pShader->texture_unit(), 0);
	    glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    glBindVertexArray(0);
  };
  std::for_each(actorGroupRenderInfo.actorRenderInfos_.begin(), actorGroupRenderInfo.actorRenderInfos_.end(), render_actor);

  glUseProgram(0);

}
