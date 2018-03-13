#include "graphics/shader.hpp"

using namespace bright::graphics;

Shader::Shader(): status_(true), statusString_(""){
}


unsigned int Shader::program_id(){
  return pid_;
}


void Shader::program_id(unsigned int pid){
  pid_ = pid;
}


unsigned int Shader::fragment_id(){
  return fid_;
}


void Shader::fragment_id(unsigned int fid){
  fid_ = fid;
}


unsigned int Shader::vertex_id(){
  return vid_;
}


void Shader::vertex_id(unsigned int vid){
  vid_ = vid;
}


int Shader::uniform_location(std::string name){
  if(uniformLocations_.find(name) == uniformLocations_.end()) {
    return -1;
  }
  return uniformLocations_[name];
}


void Shader::uniform_location(int location, std::string name){
  uniformLocations_[name] = location;
}

bool Shader::success_status(){
  return status_;
}

int Shader::sampler_id() {
  return samplerId_;
}


void Shader::sampler_id(int samplerId) {
  samplerId_ = samplerId;
}

void Shader::success_status(bool status){
  status_ = status;
}

std::string Shader::status_string(){
  return statusString_;
}


void Shader::status_string(std::string statusString){
  statusString_ = statusString;
}


int Shader::texture_unit(){
  return textureUnit_;
}


void Shader::texture_unit(int textureUnit){
  textureUnit_ = textureUnit;
}





