#include <iostream>
#include "converters/meshconverter.hpp"
#include "utils/fileworker.hpp"
#include "base/mesh.hpp"
#include <memory>
#include <algorithm>

int main(){

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("data/files.fl");
  auto pMeshConverter = std::make_shared<bright::converters::MeshConverter>(pFileWorker);

  std::vector<std::string> meshFileNames;
  meshFileNames.push_back("altair");

  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();
  
  pMeshConverter->batch_read_obj_mesh_binary();
  auto pMesh = pMeshConverter->mesh("altair");
  
  //Loop over all meshes (Add more testing later, this is pretty simple verification)
  std::cout << "Root Mesh: " << std::endl << std::flush;
  std::cout << "composite name: " << pMesh->composite_name() << std::endl << std::flush;
  std::cout << "count_children: " << pMesh->count_children() << std::endl << std::flush;
  std::cout << "count_faces: " << pMesh->count_faces() << std::endl << std::flush;
  std::cout << "count_normals: " << pMesh->count_normals() << std::endl << std::flush;
  std::cout << "count_textures: " << pMesh->count_textures() << std::endl << std::flush;
  std::cout << std::endl << std::flush;
  
  auto print_mesh = [&](std::shared_ptr<bright::base::Mesh> pChildMesh) {
    std::cout << "Child Mesh: " << std::endl << std::flush;
    std::cout << "composite name: " << pChildMesh->composite_name() << std::endl << std::flush;
    std::cout << "part_name: " << pChildMesh->part_name() << std::endl << std::flush;
    std::cout << "count_children: " << pChildMesh->count_children() << std::endl << std::flush;
    std::cout << "count_faces: " << pChildMesh->count_faces() << std::endl << std::flush;
    std::cout << "count_normals: " << pChildMesh->count_normals() << std::endl << std::flush;
    std::cout << "count_textures: " << pChildMesh->count_textures() << std::endl << std::flush;
    std::cout << std::endl << std::flush;
  };
  std::for_each(pMesh->child_meshes().begin(), pMesh->child_meshes().end(), print_mesh);
  
  std::cout << "ctr+c to exit or x on the window" << std::endl;
  while (true){ }

  return 0;
}