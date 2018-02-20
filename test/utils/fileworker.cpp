#include <iostream>
#include "utils/fileworker.hpp"
#include <memory>
#include <algorithm>
#include <vector>
#include <string>

int main(){

  auto pFileWorker = std::make_shared<bright::utils::FileWorker>("data/files.fl");
  pFileWorker->read_in_list_of_files();
  pFileWorker->create_lookup_map_of_files_content();

  //Test shortHand name (alias)
  std::string fileContents = pFileWorker->get_file_contents("shaders.res");
  std::cout << "data/shaders.res from string using alias to get" << std::endl << std::flush;
  std::cout << fileContents << std::endl << std::flush;
  std::cout << std::endl << std::flush;

  std::vector<std::string> files;
  files.push_back("pflt.v");
  std::vector<std::shared_ptr<std::istringstream>> streams = pFileWorker->get_files_streams(files);
  //Loop over all streams
  std::cout << "got per_fragment_lighting_texture.vert from stream" << std::endl << std::flush;
  auto handle_stream = [&] (std::shared_ptr<std::istringstream> pStream) { 
    std::string line;
    while(getline(*pStream,line)){
      std::cout << line << std::endl << std::flush;
    }
  };
  std::for_each(streams.begin(), streams.end(), handle_stream);


  std::cout << "ctr+c to exit or x on the window" << std::endl;
  while (true){ }

  return 0;
}