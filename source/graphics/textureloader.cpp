#include "graphics/textureloader.hpp"

using namespace bright::graphics;

TextureLoader::TextureLoader(){
}

Texture TextureLoader::create_texture(TextureConfig& textureConfig){

  Texture texture;

  std::stringstream stream(textureConfig.fileContents_);
  auto pTexture2D = imageLoader_.load_single_dds_image(stream);

  GLuint tid;
  //Allocate a texture name
  glGenTextures(1, &tid);
  texture.texture_id(tid);

  glBindTexture(GL_TEXTURE_2D, tid);

  //Note
  //Technically, we do not have to use a sampler object. 
  //The parameters we use for samplers could have been set into the texture object directly with glTexParameter. 
  //Sampler objects have a lot of advantages over setting the value in the texture, and binding a sampler object overrides parameters set in the texture. 
  //There are still some parameters that must be in the texture object, and those are not overridden by the sampler object.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(pTexture2D->levels() - 1) );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);


  //The behavior of glTexStorage2D depends on the target parameter. 
  //When target is GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_RECTANGLE, GL_PROXY_TEXTURE_RECTANGLE or GL_PROXY_TEXTURE_CUBE_MAP, 
  //calling glTexStorage2D is equivalent, assuming no errors are generated, to executing the following pseudo-code:
  //for (i = 0; i < levels; i++) {
  //    glTexImage2D(target, i, internalformat, width, height, 0, format, type, NULL);
  //    width = max(1, (width / 2));
  //    height = max(1, (height / 2));
  //}
  glTexStorage2D( GL_TEXTURE_2D,
                  GLint( pTexture2D->levels() ),
                  GLenum( gli::internal_format( pTexture2D->format() ) ),
                  GLsizei( pTexture2D->dimensions().x ),
                  GLsizei( pTexture2D->dimensions().y ) );

  if( gli::is_compressed( pTexture2D->format() ) ){
    for( gli::texture2D::size_type Level = 0; Level < pTexture2D->levels(); ++Level ){
      glCompressedTexSubImage2D( GL_TEXTURE_2D,
                                 GLint(Level),
                                 0, 0,
                                 GLsizei( pTexture2D->operator[](Level).dimensions().x ),
                                 GLsizei( pTexture2D->operator[](Level).dimensions().y ),
                                 GLenum( gli::internal_format( pTexture2D->format() ) ),
                                 GLsizei( pTexture2D->operator[](Level).size() ),
                                 pTexture2D->operator[](Level).data() );
    }
  }
  else{
    for(gli::texture2D::size_type Level = 0; Level < pTexture2D->levels(); ++Level){
      glTexSubImage2D( GL_TEXTURE_2D,
                       GLint(Level),
                       0, 0,
                       GLsizei( pTexture2D->operator[](Level).dimensions().x ),
                       GLsizei( pTexture2D->operator[](Level).dimensions().y ),
                       GLenum( gli::external_format( pTexture2D->format() ) ),
                       GLenum( gli::type_format( pTexture2D->format() ) ),
                       pTexture2D->operator[](Level).data() );
    }
  } 

  glBindTexture(GL_TEXTURE_2D, 0);
  texture.num_samplers(6);
  create_samplers(texture);

  return texture;
}
  

void TextureLoader::create_samplers(Texture& texture){

  for (int n=0; n < texture.num_samplers(); ++n){
    GLuint sampler= 0;
    glGenSamplers(1, &sampler);
    texture.samplers(sampler);
  }
  
  
  for(int samplerIx = 0; samplerIx < texture.num_samplers(); ++samplerIx){
    glSamplerParameteri(texture.get_sampler_id(samplerIx), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(texture.get_sampler_id(samplerIx), GL_TEXTURE_WRAP_T, GL_REPEAT);
  }
  
  //Nearest
  glSamplerParameteri(texture.get_sampler_id(0), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glSamplerParameteri(texture.get_sampler_id(0), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  
  //Linear
  glSamplerParameteri(texture.get_sampler_id(1), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(texture.get_sampler_id(1), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  //Linear mipmap Nearest
  glSamplerParameteri(texture.get_sampler_id(2), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(texture.get_sampler_id(2), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  
  //Linear mipmap linear
  glSamplerParameteri(texture.get_sampler_id(3), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(texture.get_sampler_id(3), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  
  //Low anisotropic
  glSamplerParameteri(texture.get_sampler_id(4), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(texture.get_sampler_id(4), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glSamplerParameterf(texture.get_sampler_id(4), GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
  
  //Max anisotropic
  GLfloat maxAniso = 0.0f;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
  
  glSamplerParameteri(texture.get_sampler_id(5), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(texture.get_sampler_id(5), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glSamplerParameterf(texture.get_sampler_id(5), GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
  
}