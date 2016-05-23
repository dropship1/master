#include "network/networkmessage.hpp"

using namespace bright::network;


NetworkMessage::NetworkMessage(std::string data, MessageType type): 
  data_(data), type_(type){
}

std::string NetworkMessage::data(){
  return data_;
}

MessageType NetworkMessage::type(){
  return type_;
}