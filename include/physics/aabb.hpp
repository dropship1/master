#ifndef BRIGHT_PHYSICS_AABB_H
#define BRIGHT_PHYSICS_AABB_H

#include <fstream>

#include "glm/glm.hpp"
#include <algorithm>
#include <memory>
#include <string>
#include <iostream>
#include <ostream>

namespace bright{

  namespace physics{


struct Box{

  Box(){
    left_ = 0.0f;
    right_ = 0.0f;
    
    top_ = 0.0f;
    bottom_ = 0.0f;
    
    front_ = 0.0f;
    back_ = 0.0f;
  }

  float left_;
  float right_;
  
  float top_;
  float bottom_;
  
  float front_;
  float back_;

  glm::vec3 pos_;

};

/**
 * @ingroup base
 *
 * @brief The Axially aligned bounding box class
 * 
 * @details 
 * Can't be rotated (versus an oriented bounding box)
 * faster and simpler aligned with the coordinate axes
 * Example Usage:
 * @code
 * @endcode
 *
 */
class AABB{

public:

/** 
 * @brief Construct a new default aabb.
 * @details
 */
  AABB(glm::vec3 initialPosition);   
  AABB();


  void add(glm::vec3 point);
  void finalize();

  glm::vec3 max();
  glm::vec3 min();

  glm::vec3 prev_pos();
  glm::vec3 curr_pos();

  void move(glm::vec3 newPosition);

  float width();
  float half_width();
  float height();
  float half_height();
  float length();
  float half_length();
  glm::vec3 half_dimensions();

  std::string name();
  void name(std::string name);

  void from_stream(std::istream &in);
  void to_stream(std::ostream &out);

/** 
 * @brief Test for intersection with another AABB
 * @param pOtherBB A shared pointer to an already created axially aligned bounding box.
 */  
  bool intersect(std::shared_ptr<AABB> pOtherBB);

  void copy_this(std::shared_ptr<AABB> otherAABB);

private:

  glm::vec3 min_;
  glm::vec3 max_;

  std::string name_;

  float halfWidth_;
  float halfHeight_;
  float halfLength_;

  float width_;
  float height_;
  float length_;

  glm::vec3 halfDimensions_;

  Box lastBox_;

  glm::vec3 prevPos_;
  glm::vec3 currPos_;

};


  }
}

#endif