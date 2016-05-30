#include "converters/aabbconverter.hpp"

using namespace bright::converters;

AABBConverter::AABBConverter(std::shared_ptr<bright::utils::FileWorker> pFileWorker): pFileWorker_(pFileWorker){}

void AABBConverter::batch_convert_obj_and_dump_aabb_binary(std::string path, std::string outputPath, std::vector<std::string> files){  

  //Loop over all files
  auto handle_file = [&] (std::string file) {
    single_convert_obj_and_dump_aabb_binary(path, outputPath, file);
  };

  std::for_each(files.begin(), files.end(), handle_file);
	
}

std::shared_ptr<bright::physics::AABB> AABBConverter::aabb(std::string aabbName){

  auto aabbToCopy = aabbs_[aabbName];

  auto aabb = std::make_shared<bright::physics::AABB>();
  aabb->copy_this( aabbToCopy );

  return aabb;

}

void AABBConverter::batch_read_obj_aabb_binary(){

  std::string aabbList =  pFileWorker_->get_file_contents("aabblist");
  std::stringstream in(aabbList);
  std::string line; 
  std::string aabbFile;
  std::vector<std::string> aabbFileList;
  while (getline(in, line)){
    if (line.substr(0,1) == "]" ){
      break;
    }
    if(line.substr(0,5) == "file="){
      aabbFile = line.substr(5);
      aabbFileList.push_back( aabbFile );
    }
  }

  std::vector<std::shared_ptr<std::istringstream>> streams = pFileWorker_->get_files_streams(aabbFileList);
  //Loop over all streams
  auto handle_stream = [&] (std::shared_ptr<std::istringstream> pStream) { 
    auto pAABB = single_read_obj_aabb_binary(pStream);
    aabbs_[pAABB->name()] = pAABB;
  };
  std::for_each(streams.begin(), streams.end(), handle_stream);

}

std::shared_ptr<bright::physics::AABB> AABBConverter::single_read_obj_aabb_binary(std::shared_ptr<std::istringstream> pStream){

  auto pAABB = std::make_shared<bright::physics::AABB>();
  pAABB->from_stream(*pStream);

  return pAABB;
}

void AABBConverter::single_convert_obj_and_dump_aabb_binary(std::string path, std::string outputPath, std::string file){  
  std::vector<glm::vec3> vertices = single_load_obj_verties(path, file);
	std::shared_ptr<bright::physics::AABB> aabb = single_convert_obj_to_aabb(outputPath, vertices, file); 
	single_dump_aabb_binary(outputPath, aabb);
}

std::vector<glm::vec3> AABBConverter::single_load_obj_verties(std::string path, std::string file){  

  int start, end;
  int dif;

  std::vector<glm::vec3> vertices;

  //std::cout << "Loading " << file+".obj: " << std::endl << std::flush;

  start = clock() / (CLOCKS_PER_SEC / 1000);



  std::string fullPathAndName = path+"/"+file+".obj";

  std::ifstream in(fullPathAndName, std::ios::in);
  if (!in) { 
    //std::cerr << "AABB Obj file reader: Cannot open " << fullPathAndName << std::endl << std::flush; 
    exit(1); 
  }

  std::string line; 
  while (getline(in, line)){
    if (line.substr(0,2) == "v "){ 
      std::istringstream s(line.substr(2));
      glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;
      vertices.push_back(v);
    }
  }

  end = clock() / (CLOCKS_PER_SEC / 1000);
  dif = end - start;

  //std::cout << "Loaded " << dif << " ms" << std::endl << std::flush;

  return vertices;
}

std::shared_ptr<bright::physics::AABB> AABBConverter::single_convert_obj_to_aabb(std::string path, std::vector<glm::vec3>& vertices, std::string fileName){

  int start, end;
  int dif;

	//std::cout << "Converting " << fileName << ": " << std::endl << std::flush;

	start = clock() / (CLOCKS_PER_SEC / 1000);

	auto aabb = create_aabb_from_vertices(vertices, fileName);

	end = clock() / (CLOCKS_PER_SEC / 1000);
	dif = end - start;

	//std::cout << "Converted " << dif << " ms" << std::endl << std::flush;
	
	return aabb;
}

void AABBConverter::single_dump_aabb_binary(std::string path, std::shared_ptr<bright::physics::AABB> aabb){
  int start, end;
  int dif;

  std::string fullPathAndName = path+"/"+aabb->name()+".bin";

	//std::cout << "Dumping " << aabb->name() << ": " << std::endl << std::flush;
	start = clock() / (CLOCKS_PER_SEC / 1000);

  //Write the new aabb out to disk.
  std::fstream output(fullPathAndName, std::ios::out | std::ios::trunc | std::ios::binary);
  //std::fstream output(fullPathAndName, std::ios::out | std::ios::trunc);
  aabb->to_stream(output);

	end = clock() / (CLOCKS_PER_SEC / 1000);
	dif = end - start;

  output.close();

	//std::cout << "Dumped " << dif << " ms" << std::endl << std::flush;
}

std::shared_ptr<bright::physics::AABB> AABBConverter::create_aabb_from_vertices(std::vector<glm::vec3>& vertices, std::string name){

  auto aabb = std::make_shared<bright::physics::AABB>(); 
  aabb->name(name);

  auto handler = [&] (glm::vec3 vertex) { 
    aabb->add(vertex);
  };
  std::for_each(vertices.begin(), vertices.end(), handler);

  aabb->finalize();

  return aabb;
}