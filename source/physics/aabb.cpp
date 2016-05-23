#include "physics/aabb.hpp"

using namespace bright::physics;


AABB::AABB(){ 
  glm::vec3 initialPosition = glm::vec3(1.0f, 1.0f, 1.0f);
  const float fBigNumber = 1000000000000;

  min_.x = fBigNumber;
  min_.y = fBigNumber;
  min_.z = fBigNumber;

  max_.x = -fBigNumber;
  max_.y = -fBigNumber;
  max_.z = -fBigNumber;

  halfDimensions_ = glm::vec3(1.0f,1.0f,1.0f);

  currPos_ = initialPosition;
  prevPos_ = initialPosition;
}


AABB::AABB(glm::vec3 initialPosition){ 
  const float fBigNumber = 1000000000000;

  min_.x = fBigNumber;
  min_.y = fBigNumber;
  min_.z = fBigNumber;

  max_.x = -fBigNumber;
  max_.y = -fBigNumber;
  max_.z = -fBigNumber;

  halfDimensions_ = glm::vec3(1.0f,1.0f,1.0f);

  currPos_ = initialPosition;
  prevPos_ = initialPosition;
}

void AABB::finalize(){

  width_ = abs(max_.x - min_.x);
  halfWidth_ = width_*0.5f;

  height_ = abs(max_.y - min_.y);
  halfHeight_ = height_*0.5f;

  length_ = abs(max_.z - min_.z);
  halfLength_ = length_*0.5f;

  halfDimensions_ = glm::vec3(halfWidth_, halfHeight_, halfLength_);
}


glm::vec3 AABB::max(){ 
  return max_;
}

glm::vec3 AABB::min(){ 
  return min_;
}


glm::vec3 AABB::half_dimensions(){ 
  return halfDimensions_;
}

glm::vec3 AABB::curr_pos(){ 
  return currPos_;
}

glm::vec3 AABB::prev_pos(){
  return prevPos_;
}

void AABB::move(glm::vec3 newPosition){ 
  prevPos_ = currPos_;
  currPos_ = newPosition;
}


float AABB::width(){
   return width_;
}

float AABB::half_width(){
   return halfWidth_;
}

float AABB::height(){
   return height_;
}

float AABB::half_height(){
   return halfHeight_;
}

float AABB::length(){
   return length_;
}

float AABB::half_length(){
   return halfLength_;
}


bool AABB::intersect(std::shared_ptr<AABB> pOtherBB){

  bool intersect = false;

  glm::vec3 otherMax = pOtherBB->curr_pos() + pOtherBB->half_dimensions();
  glm::vec3 otherMin = pOtherBB->curr_pos() - pOtherBB->half_dimensions();

  glm::vec3 thisMax = currPos_ + halfDimensions_;
  glm::vec3 thisMin = currPos_ - halfDimensions_;
  
  glm::vec3 distanceLeft = otherMin - thisMax;
  glm::vec3 distanceRight =  thisMin - otherMax;
  glm::vec3 distances = glm::max(distanceLeft, distanceRight);
  float max = distances.x;
  if (distances.y > max){ max = distances.y; }
  if (distances.z > max){ max = distances.z; }
  
  if(max < 0){
    intersect = true;
  }

  return intersect;
}


void AABB::add(glm::vec3 point){

  if (point.x < min_.x){ min_.x = point.x; }
  if (point.x > max_.x){ max_.x = point.x; }
  if (point.y < min_.y){ min_.y = point.y; }
  if (point.y > max_.y){ max_.y = point.y; }
  if (point.z < min_.z){ min_.z = point.z; }
  if (point.z > max_.z){ max_.z = point.z; }

}



std::string AABB::name(){
  return name_;
}


void AABB::name(std::string name){
  name_ = name;
}


void AABB::from_stream(std::istream &in){

  in >> min_.x;
  in >> min_.y;
  in >> min_.z;

  in >> max_.x;
  in >> max_.y;
  in >> max_.z;

  in >> name_;

  in >> halfWidth_;
  in >> halfHeight_;
  in >> halfLength_;

  in >> width_;
  in >> height_;
  in >> length_;

  in >> halfDimensions_.x;
  in >> halfDimensions_.y;
  in >> halfDimensions_.z;

  in >> prevPos_.x;
  in >> prevPos_.y;
  in >> prevPos_.z;

  in >> currPos_.x;
  in >> currPos_.y;
  in >> currPos_.z;

}


void AABB::to_stream(std::ostream &out){

  out << min_.x << std::endl;
  out << min_.y << std::endl;
  out << min_.z << std::endl;

  out << max_.x << std::endl;
  out << max_.y << std::endl;
  out << max_.z << std::endl;

  out << name_ << std::endl;

  out << halfWidth_ << std::endl;
  out << halfHeight_ << std::endl;
  out << halfLength_ << std::endl;

  out << width_ << std::endl;
  out << height_ << std::endl;
  out << length_ << std::endl;

  out << halfDimensions_.x << std::endl;
  out << halfDimensions_.y << std::endl;
  out << halfDimensions_.z << std::endl;

  out << prevPos_.x << std::endl;
  out << prevPos_.y << std::endl;
  out << prevPos_.z << std::endl;

  out << currPos_.x << std::endl;
  out << currPos_.y << std::endl;
  out << currPos_.z << std::endl;

}


void AABB::copy_this(std::shared_ptr<AABB> otherAABB){
  min_ = otherAABB->min();

  max_ = otherAABB->max();

  name_ = otherAABB->name();

  halfWidth_  = otherAABB->half_width();
  halfHeight_ = otherAABB->half_height();
  halfLength_ = otherAABB->half_length();

  width_  = otherAABB->width();
  height_ = otherAABB->height();
  length_ = otherAABB->length();

  halfDimensions_ = otherAABB->half_dimensions();

  prevPos_ = otherAABB->prev_pos();

  currPos_ = otherAABB->curr_pos();

}