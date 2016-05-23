#include <iostream>
#include "converters/meshconverter.hpp"
#include "utils/fileworker.hpp"
#include "base/mesh.hpp"
#include <memory>
#include <algorithm>

int main(){

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("test/converters/data/filelist");
  auto pMeshConverter = std::make_shared<bright::converters::MeshConverter>(pFileWorker);

  std::vector<std::string> meshFileNames;
  meshFileNames.push_back("monster_jetpack_003");
  //If you're running this from the debuger/visual studio, then you need to specify the path to the 
  //meshes/obj directory starting from the "bright" directory, for example:
  //test/tools/meshes/obj
  //But if you're building this and creating the executable, which goes into the bin directory
  //in test/tools/bin then you need to specify the path as "../meshes/obj".
  //pMeshConverter->batch_convert_obj_and_dump_mesh_binary("../../data/meshes", meshFileNames);
  pMeshConverter->batch_convert_obj_and_dump_mesh_binary("data/meshes", meshFileNames);

  std::vector<std::shared_ptr<bright::base::Mesh>> meshes;
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();
  
  pMeshConverter->batch_read_obj_mesh_binary();
  auto pMesh = pMeshConverter->mesh("monster_jetpack_003");
  //meshes = pMeshConverter->batch_read_obj_mesh_binary("test/tools/meshes/obj", meshFileNames);
  
  //Loop over all meshes (Add more testing later, this is pretty simple verification)
  std::cout << "Root Mesh: " << std::endl << std::flush;
  std::cout << "composite name: " << pMesh->composite_name() << std::endl << std::flush;
  std::cout << "count_children: " << pMesh->count_children() << std::endl << std::flush;
  std::cout << "count_faces: " << pMesh->count_faces() << std::endl << std::flush;
  std::cout << "count_normals: " << pMesh->count_normals() << std::endl << std::flush;
  std::cout << "count_textures: " << pMesh->count_textures() << std::endl << std::flush;
  std::cout << std::endl << std::flush;

  
  std::cout << "ctr+c to exit or x on the window" << std::endl;
  while (true){ }

  return 0;
}