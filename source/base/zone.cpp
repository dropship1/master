#include "base/zone.hpp"

using namespace bright::base;

Zone::Zone(glm::vec3 maxDimensions, glm::vec3 minDimensions){
  xMin_ = int(minDimensions.x);
  xMax_ = int(maxDimensions.x);
  
  zMin_ = int(minDimensions.z);
  zMax_ = int(maxDimensions.z);
  
  yMin_ = int(minDimensions.y);
  yMax_ = int(maxDimensions.y);
}


int Zone::x_min(){
  return xMin_;
}

int Zone::x_max(){
  return xMax_;
}

int Zone::y_min(){
  return yMin_;
}

int Zone::y_max(){
  return yMax_;
}

int Zone::z_min(){
  return zMin_;
}

int Zone::z_max(){
  return zMax_;
}

void Zone::x_min(int xMin){
  xMin_ = xMin;
}

void Zone::x_max(int xMax){
  xMax_ = xMax;
}

void Zone::y_min(int yMin){
  yMin_ = yMin;
}

void Zone::y_max(int yMax){
  yMax_ = yMax;
}

void Zone::z_min(int zMin){
  zMin_ = zMin;
}

void Zone::z_max(int zMax){
  zMax_ = zMax;
}


