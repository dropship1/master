#include "converters/meshconverter.hpp"
#include <iostream>

Obj::Obj(std::shared_ptr<bright::utils::FileWorker> pFileWorker): pFileWorker_(pFileWorker) {}


void Obj::load_obj_file(std::string fileName){
        
  std::string resourcesConfig = pFileWorker_->get_file_contents(fileName);
  std::stringstream in(resourcesConfig);

  std::string line; 

  //Material handling
  std::string materialFileName;
  bool hasMaterialLib = false;

  while (getline(in, line)){
    if (line.substr(0,7) == "mtllib "){ materialFileName = line.substr(7); hasMaterialLib = true; break;}
    else if (line.substr(0,2) == "v "){ hasMaterialLib = false; break; }
  }

  if (hasMaterialLib){
    load_material_file(materialFileName);
  }

  //Vertex handling
  std::string groupData;
  bool atFaces = false;
  groupLibrary_ = std::make_shared<GroupLibrary>();
  bool needToAddGroupNameBack = false;
  bool justFinishedGroup = false;

  do {

    if (line.substr(0,18) == "g SideJet_Cube.003") {
      int i = 0;
      i = 25 * 2;
    }

    if ( line.substr(0,2) == "v " || line.substr(0,3) == "vt " || line.substr(0,3) == "vn "){ justFinishedGroup = false; }

    if (line.substr(0,2) == "g " && atFaces){ needToAddGroupNameBack = true; }
    else { 
      if (line.substr(0,2) == "l " || line.substr(0,2) == "s "){ 
        continue; 
      }
      else{ groupData += "\n"+line; }
    }

    if (line.substr(0,2) == "f "){
      atFaces = true;
    }
    else if (atFaces){
      if (justFinishedGroup) { exit(0); }
      atFaces = false;
      groupLibrary_->load_group(groupData);
      if (needToAddGroupNameBack) { 
        groupData = "\n"+line; 
        needToAddGroupNameBack = false; 
      }
      else{ groupData = ""; }
      justFinishedGroup = true;
    }

  }while (getline(in, line));

  int imdone = 0;
  imdone = 1 * 10;

}


std::shared_ptr<Material> Obj::get_material(std::string matName){
  return materialLibrary_->materials_[matName];
}


void Obj::load_material_file(std::string fileName){
 
  std::string resourcesConfig = pFileWorker_->get_file_contents(fileName);
  std::stringstream in(resourcesConfig);

  std::string line; 

  std::string materialData;
  bool nextMaterial = false;
  materialLibrary_ = std::make_shared<MaterialLibrary>();
  materialLibrary_->fileName_ = fileName;

  while (getline(in, line)){
    materialData += "\n"+line;
    if (line.substr(0,7) == "newmtl "){
      nextMaterial = true;
    }
    if (nextMaterial && line == ""){
      nextMaterial = false;
      materialLibrary_->load_material(materialData);
      materialData = "";
    }    
  }
  //Last material
  if (materialData != ""){
    materialLibrary_->load_material(materialData);
  }

}


void GroupLibrary::load_group(std::string groupData){

  //std::cout << groupData << std::endl;
  std::shared_ptr<Group> group = std::make_shared<Group>();
  std::string line; 
  std::string groupName;
  std::string::size_type sz;
  
  std::vector<std::string> splitGroupData = bright::utils::split(groupData, "\\n");

  //Loop
  auto handle_line = [&] (std::string line) { 
    if (line.substr(0,1) == "g"){
      std::istringstream s(line.substr(1));
      s >> group->name_;
    }
    else if (line.substr(0,2) == "f "){
      std::string faces(line.substr(2));
      faces = bright::utils::trim(faces);
      std::vector<std::string> splitFaces = bright::utils::split( faces, "(\/| )" );
      int index = 1;
      //Loop
      auto handle_face = [&] (std::string value) { 
        unsigned int indexVal = atoi(value.c_str()); 
        //The indices we get from the obj file are not "0" based, so we have to
        //minus 1 for each index we get
        --indexVal;
        if (index == 1){ group->vertexFaceElems_.push_back(indexVal); }
        if (index == 2){ group->textureFaceElems_.push_back(indexVal); }
        if (index == 3){ group->normalFaceElems_.push_back(indexVal);  index = 0;}
        ++index;
      };
      std::for_each(splitFaces.begin(), splitFaces.end(), handle_face);
    }
    else if (line.substr(0,2) == "v "){
      std::istringstream s(line.substr(2));
      glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
      vertices_.push_back(v);
    }
    else if (line.substr(0,2) == "vn"){
      std::istringstream s(line.substr(2));
      glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;
      normals_.push_back(v);
    }
    else if (line.substr(0,2) == "vt"){
      std::istringstream s(line.substr(2));
      glm::vec3 v; s >> v.x; s >> v.y;// s >> v.z;
      v.z = 0.0f;
      textures_.push_back(v);
    }
    else if (line.substr(0,7) == "usemtl "){
      group->material_ = line.substr(7);
    }
  };

  std::for_each(splitGroupData.begin(), splitGroupData.end(), handle_line);
  
  groups_.push_back(group);
  
}


void MaterialLibrary::load_material(std::string materialData){

  std::shared_ptr<Material> material = std::make_shared<Material>();
  std::string line; 
  std::string materialName;
  
  std::vector<std::string> splitMaterialData = bright::utils::split( materialData, "\\n");
  std::string::size_type sz;   

  auto handle_material = [&] (std::string line) {  
    bright::utils::trim(line);
    if (line.substr(0,7) == "newmtl "){
      std::istringstream s(line.substr(7));
      s >> material->name_;
    }
    else if (line.substr(0,3) == "Ns "){
      std::string ns = line.substr(3);
      material->Ns_ =  std::stof (ns,&sz);
    }
    else if (line.substr(0,2) == "d "){
      std::string d = line.substr(2);
      material->d_ = std::stof(d,&sz);
    }
    else if (line.substr(0,6) == "illum "){
      std::string illum = line.substr(6);
      material->illum_ = std::stof(illum,&sz);
    }
    else if (line.substr(0,3) == "Kd "){ 
      std::string values(line.substr(3));
      bright::utils::trim(values);
      std::vector<std::string> splitValues = bright::utils::split( values, " " );
      int index = 1;
      float x, y, z;
      //Loop
      auto handle_kd = [&] (std::string value) { 
	      float Val = std::stof (value,&sz); 
        if (index == 1){ x = Val; }
        if (index == 2){ y = Val; }
        if (index == 3){ z = Val; }
        ++index;
      };
      std::for_each(splitValues.begin(), splitValues.end(), handle_kd);
      material->Kd_ = glm::vec3(x,y,z);
    }
    else if (line.substr(0,3) == "Ks "){ 
      std::string values(line.substr(3));
      bright::utils::trim(values);
      std::vector<std::string> splitValues = bright::utils::split( values, " " );
      int index = 1;
      float x, y, z;
      //Loop
      auto handle_ks = [&] (std::string value) { 
        float Val = std::stof(value,&sz); 
        if (index == 1){ x = Val; }
        if (index == 2){ y = Val; }
        if (index == 3){ z = Val; }
        ++index;
      };
      std::for_each(splitValues.begin(), splitValues.end(), handle_ks);
      material->Ks_ = glm::vec3(x,y,z);
    }
    else if (line.substr(0,3) == "Ka "){ 
      std::string values(line.substr(3));
      bright::utils::trim(values);
      std::vector<std::string> splitValues = bright::utils::split( values, " " );
      int index = 1;
      float x, y, z;
      //Loop
      auto handle_ka = [&] (std::string value) { 
        float Val = std::stof(value,&sz);
        if (index == 1){ x = Val; }
        if (index == 2){ y = Val; }
        if (index == 3){ z = Val; }
        ++index;
      };
      std::for_each(splitValues.begin(), splitValues.end(), handle_ka);
      material->Ka_ = glm::vec3(x,y,z);
    }
    else if (line.substr(0,7) == "map_Kd "){ 
      material->load_map(line);
    }
  };
  
  std::for_each(splitMaterialData.begin(), splitMaterialData.end(), handle_material);

  materials_[material->name_] = material;

}


void Material::load_map(std::string mapData){
  if (mapData.substr(0,7) == "map_Kd "){ 
    map_Kd_ = mapData.substr(7);
    hasMaps_ = true;
  }
  else if (mapData.substr(0,6) == "map_d "){ 
    map_d_ = mapData.substr(6);
    hasMaps_ = true;
  }

}
