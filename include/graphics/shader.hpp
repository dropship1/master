#ifndef BRIGHT_GRAPHICS_SHADER_H
#define BRIGHT_GRAPHICS_SHADER_H

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <map>

namespace bright{

  namespace graphics{

/**
 * @ingroup graphics
 *
 * @brief The shader class
 * 
 * @details 
 * This is a simple representation of a shader program.
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class Shader{

public:

/** 
 * @brief Construct a new default shader.
 * @details
 */
  Shader();

  unsigned int program_id();
  void program_id(unsigned int pid);

  unsigned int fragment_id();
  void fragment_id(unsigned int fid);

  unsigned int vertex_id();
  void vertex_id(unsigned int vid);

  bool success_status();
  void success_status(bool status);

  std::string status_string();
  void status_string(std::string statusString);

  int texture_unit();
  void texture_unit(int textureUnit);

  int sampler_id();
  void sampler_id(int samplerId);


/** 
 * @brief This allows for getting a location of any named uniform that this shader may have
 *
 * @details
 *
 * @param name The name of the uniform location you want to retrieve
 * @return The location of the uniform
 *
 */
  int uniform_location(std::string name);


/** 
 * @brief This allows for setting the location of any named uniform that this shader should have
 *
 * @details
 * Some uniform name examples:
 *   -color
 *   -modelToWorldMatrix
 *   -worldToCamMatrix
 *   -cameraToClipMatrix
 *   -normalToWorldMatrix
 *   -dirToLight
 *   -lightIntensity
 *   -ambientIntensity
 *   -currentlySelected
 *   -colorTexture
 *
 * @param location The location of the uniform
 * @param name The name of the uniform
 *
 */
  void uniform_location(int location, std::string name);



private:

  unsigned int pid_;
  unsigned int fid_;
  unsigned int vid_;

  int textureUnit_;
  int samplerId_;

  std::map<std::string,int> uniformLocations_;

  std::string id_;
  std::string statusString_;
  bool status_;

};

  }
}

#endif