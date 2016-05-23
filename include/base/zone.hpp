#ifndef BRIGHT_BASE_ZONE_H
#define BRIGHT_BASE_ZONE_H

#include "glm/glm.hpp";

namespace bright{

  namespace base{

class Zone{

public:
  Zone(glm::vec3 maxDimensions, glm::vec3 minDimensions);

  int x_min();
  int z_min();
  int y_min();
  int x_max();
  int z_max();
  int y_max();
  void x_min(int xMin);
  void z_min(int zMin);
  void y_min(int yMin);
  void x_max(int xMax);
  void z_max(int zMax);
  void y_max(int yMax);

private:
  int xMin_;
  int xMax_;

  int zMin_;
  int zMax_;

  int yMin_;
  int yMax_;
  int uniqueId_;

};


  }
}

#endif