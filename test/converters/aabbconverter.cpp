#include <iostream>
#include "converters/aabbconverter.hpp"
#include "utils/fileworker.hpp"
#include "physics/aabb.hpp"
#include <memory>
#include <algorithm>

int main(){

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("data/files.fl");
  auto pAABBConverter = std::make_shared<bright::converters::AABBConverter>(pFileWorker);

  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();

  std::vector<std::string> aabbFileNames;
  aabbFileNames.push_back("monster_jetpack_003");

  pAABBConverter->batch_convert_obj_and_dump_aabb_binary("data/aabbs");
  
  pAABBConverter->batch_read_obj_aabb_binary();
  auto pAABB = pAABBConverter->aabb_shr_ptr_copy("monster_jetpack_003");

  //Loop over all aabbs (Add more testing later, this is pretty simple verification)
  std::cout << "AABB: " << std::endl << std::flush;
  std::cout << "name: " << pAABB->name() << std::endl << std::flush;
  std::cout << "height: " << pAABB->height() << std::endl << std::flush;
  std::cout << "width: " << pAABB->width() << std::endl << std::flush;
  std::cout << "length: " << pAABB->length() << std::endl << std::flush;
  std::cout << "maxX: " << pAABB->max().x << std::endl << std::flush;
  std::cout << "maxY: " << pAABB->max().y << std::endl << std::flush;
  std::cout << "maxZ: " << pAABB->max().z << std::endl << std::flush;
  std::cout << "minX: " << pAABB->min().x << std::endl << std::flush;
  std::cout << "minY: " << pAABB->min().y << std::endl << std::flush;
  std::cout << "minZ: " << pAABB->min().z << std::endl << std::flush;
  std::cout << std::endl << std::flush;
  
  
  std::cout << "ctr+c to exit or x on the window" << std::endl;
  while (true){ }

  return 0;
}