#ifndef BRIGHT_TOOLS_OBJ_H
#define BRIGHT_TOOLS_OBJ_H

#include "base/mesh.hpp"
#include "utils/utilities.hpp"
#include "utils/fileworker.hpp"

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <algorithm>

#include <glm/glm.hpp> 

#include <GL/glew.h>
#include <GL/wglew.h>

class Group{
public:
  //Lets reserve (Preallocate memory) here to speed things up
  Group() {
    vertexFaceElems_.reserve(100000);
    normalFaceElems_.reserve(100000);
    textureFaceElems_.reserve(100000);
  }
  std::string name_;
  std::string material_;
  std::vector<unsigned int> vertexFaceElems_;
  std::vector<unsigned int> normalFaceElems_;
  std::vector<unsigned int> textureFaceElems_;
};

class GroupLibrary{
public:
  //Lets reserve (Preallocate memory) here to speed things up
  GroupLibrary() {
    vertices_.reserve(100000);
    normals_.reserve(100000);
    textures_.reserve(100000);
  }

  std::vector<std::shared_ptr<Group>> groups_;
  std::vector<glm::vec4> vertices_;
  std::vector<glm::vec3> normals_;
  std::vector<glm::vec3> textures_;
  void load_group(std::string groupData);
};

class Material{
public:
  Material(){hasMaps_ = false;}
  std::string name_;
	float Ns_;
	float d_;
	float illum_;
	glm::vec3 Kd_;
	glm::vec3 Ks_;
	glm::vec3 Ka_;
  bool hasMaps_;
	std::string map_Kd_;
	std::string map_d_;
  void load_map(std::string mapData);
};

class MaterialLibrary{
public:
  std::string fileName_;
  std::map<std::string, std::shared_ptr<Material>> materials_;
  void load_material(std::string materialData);
};


/**
 * @ingroup tools
 *
 * @brief The obj class
 * 
 * @details 
 * This is a representation of the .obj(WaveFront) file format
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class Obj{
public:
  Obj(std::shared_ptr<bright::utils::FileWorker> pFileWorker);
  
  void load_obj_file(std::string fileName);
  void load_material_file(std::string fileName);
  std::shared_ptr<Material> get_material(std::string name);
  std::string fileName_;
  std::shared_ptr<GroupLibrary> groupLibrary_;
  std::shared_ptr<MaterialLibrary> materialLibrary_;

  std::shared_ptr<bright::utils::FileWorker> pFileWorker_;
  
};


#endif