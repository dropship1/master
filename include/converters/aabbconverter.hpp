#ifndef BRIGHT_CONVERTERS_AABB_CONVERTER_H
#define BRIGHT_CONVERTERS_AABB_CONVERTER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <time.h>
#include <algorithm>
#include <glm/glm.hpp> 
#include "utils/fileworker.hpp"
#include "physics/aabb.hpp"

namespace bright{

  namespace converters{

/**
 * @ingroup converters
 *
 * @brief The aabb converter class
 * 
 * @details 
 * ...
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class AABBConverter {
public:

  AABBConverter(std::shared_ptr<bright::utils::FileWorker> pFileWorker);

  void batch_convert_obj_and_dump_aabb_binary(std::string outputPath);
  void single_convert_obj_and_dump_aabb_binary(std::string outputPath, std::string file);
  void single_read_in_aabb_binary(std::string path, std::string file);
  void batch_read_obj_aabb_binary();
  bright::physics::AABB single_read_obj_aabb_binary(std::shared_ptr<std::istringstream> pStream); 
  std::shared_ptr<bright::physics::AABB> aabb_shr_ptr_copy(std::string aabbName);
  bright::physics::AABB aabb_stack_copy(std::string aabbName);

private:

  void create_aabb_from_vertices(std::vector<glm::vec3>& vertices, std::string name, bright::physics::AABB& aabb);
  void single_load_obj_vertices(std::vector<glm::vec3>& vertices, std::string file);
  void single_convert_vertices_to_aabb(std::vector<glm::vec3>& vertices, std::string aabbName, bright::physics::AABB& aabb);
  void single_dump_aabb_binary(std::string path, bright::physics::AABB& aabb);

  std::map<std::string, bright::physics::AABB> aabbs_;

  std::shared_ptr<bright::utils::FileWorker> pFileWorker_;

};

  }
}  

#endif