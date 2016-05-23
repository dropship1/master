#include "graphics/worldinfo.hpp"

using namespace bright::graphics;


WorldInfo::WorldInfo(){
}

glm::mat4 WorldInfo::world_to_cam_matrix(std::string cameraType){
  return worldToCameraMatrices_[cameraType];
}

void WorldInfo::world_to_cam_matrix(std::string cameraType, glm::mat4 worldToCamMatrix){
  worldToCameraMatrices_[cameraType] = worldToCamMatrix;
}


bright::graphics::Light& WorldInfo::directional_light(){
  return directionalLight_;
}


bright::graphics::Light& WorldInfo::ambient_light(){
  return ambientLight_;
}

void WorldInfo::directional_light(bright::graphics::Light directionalLight){
  directionalLight_ = directionalLight;
}


void WorldInfo::ambient_light(bright::graphics::Light ambientLight){
  ambientLight_ = ambientLight;
}


//void WorldInfo::parse_world(std::string fileName){
//
//  std::string Line = std::string();
//  std::string currentNode;
//  SWorldActorInfo stWorldActorInfo;
//
//  std::ifstream File( fileName.c_str() ); 
//  if (File.is_open()) { 
//    while (!File.eof()) { 
//
//      getline(File, Line);
//	    std::string type = Line.substr(0,1);
//	    
//	    if(type == "["){
//		    std::string sName = Line.substr(1);
//        stWorldActorInfo.m_sName = sName;
//        stWorldActorInfo.m_v3Pos = glm::vec3();
//        stWorldActorInfo.m_v3Rot = glm::vec3();
//        stWorldActorInfo.m_eRenderType = grim::base::ERenderType::QUESTION_MARK_CUBE;
//        stWorldActorInfo.bIsPlayer = false;
//	    }
//	    else if(type == "<"){
//		    currentNode = Line.substr(1,Line.length()-2);
//	    }
//	    else if (type == "]"){
//        m_mWorldActorInfo.push_back(stWorldActorInfo);
//	    }
//	    else if(type == "#"){}
//	    else{
//		    if (currentNode == "Position"){
//		  	  std::string sName = Line.substr(0,Line.find("="));
//		  	  std::string sValue = Line.substr(Line.find("=")+1);
//          float fValue = atof( sValue.c_str() );
//          if (sName.compare("X") == 0){ stWorldActorInfo.m_v3Pos.x = fValue; }
//          else if (sName.compare("Y") == 0){ stWorldActorInfo.m_v3Pos.y = fValue; }
//          else if (sName.compare("Z") == 0){ stWorldActorInfo.m_v3Pos.z = fValue; }
//		    }
//		    else if (currentNode == "Rotation"){
//		  	  std::string sName = Line.substr(0,Line.find("="));
//		  	  std::string sValue = Line.substr(Line.find("=")+1);
//          float fValue = atof( sValue.c_str() );
//          if (sName.compare("X") == 0){ stWorldActorInfo.m_v3Rot.x = fValue; }
//          else if (sName.compare("Y") == 0){ stWorldActorInfo.m_v3Rot.y = fValue; }
//          else if (sName.compare("Z") == 0){ stWorldActorInfo.m_v3Rot.z = fValue; }
//		    }
//		    else if (currentNode == "RenderInfo"){
//		  	  std::string sName = Line.substr(0,Line.find("="));
//		  	  std::string sValue = Line.substr(Line.find("=")+1);
//          float fValue = atof( sValue.c_str() );
//          if (sName.compare("Type") == 0){ 
//            if ( m_mRenderType.count(sValue) == 1 ){
//              stWorldActorInfo.m_eRenderType = m_mRenderType[sValue];
//            }
//          }
//		    }
//		    else if (currentNode == "Player"){
//		  	  std::string sName = Line.substr(0,Line.find("="));
//		  	  std::string sValue = Line.substr(Line.find("=")+1);
//          int iValue = atoi( sValue.c_str() );
//          if (sName.compare("Yes") == 0){
//            if(iValue == 1){ stWorldActorInfo.bIsPlayer = true; }
//            else{ stWorldActorInfo.bIsPlayer = false; }
//          }
//		    }
//	    }
//    }
//        
//    File.close(); 
//  }
//
//}
