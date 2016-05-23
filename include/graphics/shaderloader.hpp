#ifndef BRIGHT_GRAPHICS_SHADER_LOADER_H
#define BRIGHT_GRAPHICS_SHADER_LOADER_H

#include <GL/glew.h>
#include <GL/wglew.h>
#include "glm/glm.hpp"

#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "graphics/globalstructs.hpp"


namespace bright{

  namespace graphics{

/**
 * @ingroup graphics
 *
 * @brief The shader loader class
 * 
 * @details 
 * This is where all shader loading functionality resides
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class ShaderLoader {

public:
  ShaderLoader();
  
  std::shared_ptr<Shader> load_single_shader_program(std::shared_ptr<ShaderConfig> shaderConfig);

private:  

  std::shared_ptr<Shader> create_shader_program(std::shared_ptr<ShaderConfig> shaderConfig);
  std::string text_file_read(std::string fileName);
  void validate_shader(GLuint shader, GLenum shaderType);
  void validate_program(GLuint program);
  void initialize_shader_program(std::shared_ptr<Shader> pShade, std::shared_ptr<ShaderConfig> shaderConfig);
  void initialize_fragment_shader_text(std::string fragmentShaderFileName);
  void initialize_vertex_shader_text(std::string vertexShaderFileName);


  std::string vertexShaderText_;
  std::string fragmentShaderText_;

};

  }
}

#endif