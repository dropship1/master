#ifndef BRIGHT_GRAPHICS_TEXTURE_H
#define BRIGHT_GRAPHICS_TEXTURE_H

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
 * @brief The texture class
 * 
 * @details 
 * This is a simple representation of a texture program.
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class Texture{

public:

/** 
 * @brief Construct a new default texture.
 * @details
 */
  Texture();

  unsigned int texture_id();
  void texture_id(unsigned int tid);

  int num_samplers();
  void num_samplers(int num);

  bool success_status();
  void success_status(bool status);

  std::string status_string();
  void status_string(std::string statusString);

  std::vector<unsigned int>& samplers();
  void samplers(unsigned int sampleId);
  unsigned int get_sampler_id(int index);


private:

  unsigned int tid_;
  int numSamplers_;
  std::vector<unsigned int> samplers_;

  std::string statusString_;
  bool status_;

};

  }
}

#endif