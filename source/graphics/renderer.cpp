#include "graphics/renderer.hpp"

using namespace bright::graphics;

Renderer::Renderer(){
}


void Renderer::render(std::shared_ptr<bright::graphics::ActorGroupRenderInfo> pActorGroupRenderInfo, std::shared_ptr<WorldInfo> pWorldInfo){
  
  glm::mat4 worldToCamMat = pWorldInfo->world_to_cam_matrix( pActorGroupRenderInfo->cameraType_ );

  auto pShader = pActorGroupRenderInfo->pShader_;

  glUseProgram( pShader->program_id() );
  glActiveTexture( GL_TEXTURE0 + pShader->texture_unit() );
  //Currently everything is controlled from the root/top level root actor
  //No controll per actor (group) yet
  glm::mat4 modToWorldMat = pActorGroupRenderInfo->modToWorld_;
    int count = 0;
  auto render_actor = [&] (std::pair<std::string, std::shared_ptr<ActorRenderInfo>> pChildActorInfoPair) { 
  
    auto pChildActorInfo = pChildActorInfoPair.second;

    auto pTexture = pChildActorInfo->pTexture_;

    glBindVertexArray(pChildActorInfo->vao_);
    
    //Upload Scene info to the Shader
    glUniformMatrix4fv( pShader->uniform_location("worldToCamMatrix"), 1, GL_FALSE, glm::value_ptr( worldToCamMat ) );
    glUniform4fv( pShader->uniform_location("ambientIntensity"), 1, glm::value_ptr(pWorldInfo->ambient_light().lightIntensity_) );
    glUniform3fv( pShader->uniform_location("dirToLight"), 1, glm::value_ptr(pWorldInfo->directional_light().lightDirection_) );
    glUniform4fv( pShader->uniform_location("lightIntensity"), 1, glm::value_ptr(pWorldInfo->directional_light().lightIntensity_) );
    
    glm::mat3 normToWorldMat3( modToWorldMat );
    
    //Upload info to the Shader
    glUniformMatrix4fv( pShader->uniform_location("modelToWorldMatrix"), 1, GL_FALSE, glm::value_ptr( modToWorldMat ) );
    glUniformMatrix3fv( pShader->uniform_location("normalToWorldMatrix"), 1, GL_FALSE, glm::value_ptr(normToWorldMat3) );

    if ( !pChildActorInfo->hasTexture_ ){
      glUniform4fv( pShader->uniform_location("diffuseColor"), 1, glm::value_ptr( pChildActorInfo->diffuseColor_ ) );
    }
    

    if ( pChildActorInfo->hasTexture_ ){
      ++count;
      //if(count == 6){
        glBindTexture( GL_TEXTURE_2D, pTexture->texture_id() );
        glBindSampler(pShader->texture_unit(), pTexture->get_sampler_id(4));
        glUniform1i( pShader->uniform_location("colorTexture"), pShader->texture_unit() );
      //}
    }
    
    glDrawArrays( GL_TRIANGLES, 0, pChildActorInfo->vertexCoordsSize_ );
    
    if ( pChildActorInfo->hasTexture_ ){
      glBindSampler(pShader->texture_unit(), 0);
	    glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    glBindVertexArray(0);
  };
  std::for_each(pActorGroupRenderInfo->actorRenderInfos_.begin(), pActorGroupRenderInfo->actorRenderInfos_.end(), render_actor);

  glUseProgram(0);

}
