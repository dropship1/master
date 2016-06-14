#include "base/renderactor.hpp"

using namespace bright::base;

RenderActor::RenderActor(){
}

std::string RenderActor::mesh(){
  return mesh_;
}


void RenderActor::mesh(std::string mesh){
  mesh_ = mesh;
}

std::string RenderActor::shader(){
  return shader_;
}


void RenderActor::shader(std::string shader){
  shader_ = shader;
}


std::string RenderActor::name(){
  return name_;
}


void RenderActor::name(std::string name){
  name_ = name;
}


std::string RenderActor::camera_type(){
  return cameraType_;
}


void RenderActor::camera_type(std::string cameraType){
  cameraType_ = cameraType;
}