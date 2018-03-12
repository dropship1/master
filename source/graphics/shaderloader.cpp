#include "graphics/shaderloader.hpp"

using namespace bright::graphics;


ShaderLoader::ShaderLoader(){
}


void ShaderLoader::validate_shader(GLuint shaderId, GLenum shaderType){

  GLint status;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE){
    GLint infoLogLength;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    
    GLchar *infoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shaderId, infoLogLength, NULL, infoLog);
    
    const char *type = NULL;
    switch(shaderType){
      case GL_VERTEX_SHADER: type = "vertex"; break;
      case GL_FRAGMENT_SHADER: type = "fragment"; break;
      case GL_GEOMETRY_SHADER: type = "geometry"; break;
    }
    delete[] infoLog;
  }
}


void ShaderLoader::validate_program(GLuint programId){ 

  //Get OpenGL to try validating the program
  glValidateProgram(programId);
  
  GLint status;
  glGetProgramiv(programId, GL_VALIDATE_STATUS, &status);   
  if (status == GL_FALSE){
    int doWhat = 0;
  }

  glGetProgramiv (programId, GL_LINK_STATUS, &status);
  if (status == GL_FALSE){
    GLint infoLogLength;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
   
    GLchar *infoLog = new GLchar[infoLogLength + 1];
    glGetProgramInfoLog(programId, infoLogLength, NULL, infoLog);
    std::string text(infoLog);
    delete[] infoLog;
  }      
}


Shader ShaderLoader::load_single_shader_program(ShaderConfig& shaderConfig){

  auto shader = create_shader_program(shaderConfig);
  if ( shader.success_status() ){
    initialize_shader_program(shader, shaderConfig);
    return shader;
  }
  else{
    //handle error
    return shader;
  }
  
}


void ShaderLoader::initialize_shader_program(Shader& shader, ShaderConfig& shaderConfig){

  std::stringstream vertexShaderFileStream(vertexShaderText_);
  for (std::string line; std::getline(vertexShaderFileStream, line); ){
    if (line.substr(0,8) == "uniform "){ 
      std::string trimedLine = line.substr(8);
      std::size_t nextSpacePos = trimedLine.find(" ");
      std::size_t semicolonPos = trimedLine.find(";");
      std::size_t stringSize = semicolonPos- nextSpacePos;
      std::string uniformLocationName = trimedLine.substr(nextSpacePos+1, stringSize);
      //trim simicolon off the end
      uniformLocationName = uniformLocationName.substr(0,uniformLocationName.size()-1);
      GLint location = glGetUniformLocation(shader.program_id(), uniformLocationName.c_str());
      shader.uniform_location(location, uniformLocationName);
    }
  }

  std::stringstream fragmentShaderFileStream(fragmentShaderText_);
  for (std::string line; std::getline(fragmentShaderFileStream, line); ){
    if (line.substr(0,8) == "uniform "){ 
      std::string trimedLine = line.substr(8);
      std::size_t nextSpacePos = trimedLine.find(" ");
      std::size_t semicolonPos = trimedLine.find(";");
      std::size_t stringSize = semicolonPos - nextSpacePos;
      std::string uniformLocationName = trimedLine.substr(nextSpacePos+1, stringSize);
      //trim simicolon off the end
      uniformLocationName = uniformLocationName.substr(0,uniformLocationName.size()-1);
      GLint location = glGetUniformLocation(shader.program_id(), uniformLocationName.c_str());
      shader.uniform_location(location, uniformLocationName);
    }
  }

  if (shaderConfig.usePerpective_){
    //http://web.archive.org/web/20140208131757/http://www.arcsynthesis.org/gltut/Positioning/Tut04%20Aspect%20of%20the%20World.html#d0e4422
    //http://stackoverflow.com/questions/15457210/opengl-es-objects-away-from-view-centre-are-stretched
    //Load up the CameraToClipMatrix, this is our perspective adjustment of our camera(transforming our camera into clip space), 
    //so its the same *most* the time
    //FOV can be dynamically set to implement "zoom" (multply by a zoom factor)
    float fov = 45.0f;
    //TODO: need to set aspect ratio as screen size changes. (Unless for starts, we just assume only ever fullscreen and
    //certain ratios)
    //float apectRatio = (16.0f / 9.0f);
    float apectRatio = (1264 / 729);
    float zNear = 0.25f; 
    float zFar = 200000.0f;
    glm::mat4 persMatrix = glm::perspective(fov, apectRatio, zNear, zFar);
    glUseProgram( shader.program_id() );
    unsigned int camToClipLocation = shader.uniform_location(shaderConfig.camToClipLocationName_);
    glUniformMatrix4fv(camToClipLocation, 1, GL_FALSE, glm::value_ptr(persMatrix));
    glUseProgram(0);
  }

  if (shaderConfig.hasTextures_){
    shader.texture_unit(1);
    glUseProgram(shader.program_id() );
    glUniform1i(shader.uniform_location(shaderConfig.textureUniformName_), shader.texture_unit());
    glUseProgram(0);
  }

}


Shader ShaderLoader::create_shader_program(ShaderConfig& shaderConfig){
  
  Shader shader;

  //Create a vertex shader
  GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  shader.vertex_id(vertexShaderId);
  
  //Create a fragment shader
  GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  shader.fragment_id(fragmentShaderId);
  
  vertexShaderText_ = shaderConfig.vertexFileContents_;
  fragmentShaderText_ = shaderConfig.fragmentFileContents_;
   
  const char *vertexText = vertexShaderText_.c_str();
  const char *fragmentText = fragmentShaderText_.c_str();

  //If either the vertex or fragment shader wouldn't load
  if (vertexText == NULL || fragmentText == NULL) {
	  shader.success_status(false);
	  shader.status_string("Either vertex shader or fragment shader file data not found.");
    return shader;
  }

  glShaderSource(vertexShaderId, 1, &vertexText, NULL); 

  glCompileShader(vertexShaderId); 

  validate_shader(vertexShaderId, GL_VERTEX_SHADER); 

  glShaderSource(fragmentShaderId, 1, &fragmentText, NULL); 

  glCompileShader(fragmentShaderId); 

  validate_shader(fragmentShaderId, GL_FRAGMENT_SHADER);

  //Create a GLSL program
  GLuint shaderProgramId = glCreateProgram();
  shader.program_id(shaderProgramId);
  
  //Attach a vertex shader to the program
  glAttachShader(shaderProgramId, vertexShaderId);

  //Attach the fragment shader to the program    
  glAttachShader(shaderProgramId, fragmentShaderId); 

  //Link the vertex and fragment shaders in the program
  glLinkProgram(shaderProgramId); 

  //Validate the shader program
  validate_program(shaderProgramId);
  
  return shader;
}