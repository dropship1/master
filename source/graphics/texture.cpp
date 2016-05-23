#include "graphics/texture.hpp"

using namespace bright::graphics;

Texture::Texture(): status_(true), statusString_(""), numSamplers_(0){
}


unsigned int Texture::texture_id(){
  return tid_;
}


void Texture::texture_id(unsigned int tid){
  tid_ = tid;
}

void Texture::samplers(unsigned int sampleId){
  samplers_.push_back(sampleId);
}


int Texture::num_samplers(){
  return numSamplers_;
}


void Texture::num_samplers(int num){
  numSamplers_ = num;
}


std::vector<unsigned int>& Texture::samplers(){
  return samplers_;
}

unsigned int Texture::get_sampler_id(int index){
  return samplers_[index];
}


bool Texture::success_status(){
  return status_;
}


void Texture::success_status(bool status){
  status_ = status;
}

std::string Texture::status_string(){
  return statusString_;
}


void Texture::status_string(std::string statusString){
  statusString_ = statusString;
}





