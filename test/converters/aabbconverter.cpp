#include <iostream>
#include "converters/aabbconverter.hpp"
#include "utils/fileworker.hpp"
#include "physics/aabb.hpp"
#include <memory>
#include <algorithm>

int main(){

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("data/files.fl");
  auto pAABBConverter = std::make_shared<bright::converters::AABBConverter>(pFileWorker);

  std::vector<std::string> aabbFileNames;
  aabbFileNames.push_back("monster_jetpack_003");
  //If you're running this from the debuger/visual studio, then you need to specify the path to the 
  //meshes/obj directory starting from the "bright" directory, for example:
  //test/tools/meshes/obj
  //But if you're building this and creating the executable, which goes into the bin directory
  //in test/tools/bin then you need to specify the path as "../meshes/obj".
  pAABBConverter->batch_convert_obj_and_dump_aabb_binary("../../data/Meshes", "../../data/aabbs", aabbFileNames);
  //pAABBConverter->batch_convert_obj_and_dump_aabb_binary("data/meshes", "data/aabbs",  aabbFileNames);

  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();
  
  pAABBConverter->batch_read_obj_aabb_binary();
  auto pAABB = pAABBConverter->aabb_shr_ptr_copy("monster_jetpack_003");
  //aabbs = pAABBConverter->batch_read_obj_aabb_binary("test/tools/meshes/obj", aabbFileNames);
  
  //Loop over all aabbs (Add more testing later, this is pretty simple verification)
  std::cout << "AABB: " << std::endl << std::flush;
  std::cout << "name: " << pAABB->name() << std::endl << std::flush;
  std::cout << "height: " << pAABB->height() << std::endl << std::flush;
  std::cout << "width: " << pAABB->width() << std::endl << std::flush;
  std::cout << "length: " << pAABB->length() << std::endl << std::flush;
  std::cout << std::endl << std::flush;

  
  std::cout << "ctr+c to exit or x on the window" << std::endl;
  while (true){ }

  return 0;
}