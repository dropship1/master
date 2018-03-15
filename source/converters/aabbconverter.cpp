#include "converters/aabbconverter.hpp"

using namespace bright::converters;

AABBConverter::AABBConverter(std::shared_ptr<bright::utils::FileWorker> pFileWorker): pFileWorker_(pFileWorker){}

void AABBConverter::batch_convert_obj_and_dump_aabb_binary(std::string outputPath){  

  std::string aabbList = pFileWorker_->get_file_contents("aabb_conversions.res");
  std::stringstream in(aabbList);
  std::string line;
  std::string aabbFile;
  std::vector<std::string> aabbFileList;
  while (getline(in, line)) {
    if (line.substr(0, 1) == "]") {
      break;
    }
    if (line.substr(0, 5) == "file=") {
      aabbFile = line.substr(5);
      aabbFileList.push_back(aabbFile);
    }
  }

  //Loop over all files
  auto handle_file = [&] (std::string file) {
    single_convert_obj_and_dump_aabb_binary(outputPath, file);
  };

  std::for_each(aabbFileList.begin(), aabbFileList.end(), handle_file);
	
}


bright::physics::AABB AABBConverter::aabb_stack_copy(std::string aabbName){

  auto aabbToCopy = aabbs_[aabbName];

  bright::physics::AABB aabb;
  aabb.copy_this( aabbToCopy );

  return aabb;

}


std::shared_ptr<bright::physics::AABB> AABBConverter::aabb_shr_ptr_copy(std::string aabbName){

  auto aabbToCopy = aabbs_[aabbName];

  auto aabb = std::make_shared<bright::physics::AABB>();
  aabb->copy_this( aabbToCopy );

  return aabb;

}

void AABBConverter::batch_read_obj_aabb_binary(){

  std::string aabbList =  pFileWorker_->get_file_contents("aabb.res");
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
    auto aabb = single_read_obj_aabb_binary(pStream);
    aabbs_[aabb.name()] = aabb;
  };
  std::for_each(streams.begin(), streams.end(), handle_stream);

}

bright::physics::AABB AABBConverter::single_read_obj_aabb_binary(std::shared_ptr<std::istringstream> pStream){

  bright::physics::AABB aabb;
  aabb.from_stream(*pStream);

  return aabb;
}

void AABBConverter::single_convert_obj_and_dump_aabb_binary(std::string outputPath, std::string file){  
  std::vector<glm::vec3> vertices;
  //Preallocation to help performance
  vertices.reserve(100000);
  single_load_obj_vertices(vertices, file);
  bright::physics::AABB aabb;
	single_convert_vertices_to_aabb(vertices, file, aabb);
	single_dump_aabb_binary(outputPath, aabb);
}

void AABBConverter::single_load_obj_vertices(std::vector<glm::vec3>& vertices, std::string file){

  int start, end;
  int dif;

  //std::cout << "Loading " << file << std::endl << std::flush;

  start = clock() / (CLOCKS_PER_SEC / 1000);

  std::string meshList = pFileWorker_->get_file_contents(file);
  std::stringstream in(meshList);

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
}

void AABBConverter::single_convert_vertices_to_aabb(std::vector<glm::vec3>& vertices, std::string aabbName, bright::physics::AABB& aabb){

  int start, end;
  int dif;

	//std::cout << "Converting Vertices into aabb: " << aabbName << ": " << std::endl << std::flush;

	start = clock() / (CLOCKS_PER_SEC / 1000);

	create_aabb_from_vertices(vertices, aabbName, aabb);

	end = clock() / (CLOCKS_PER_SEC / 1000);
	dif = end - start;

	//std::cout << "Converted " << dif << " ms" << std::endl << std::flush;
}

void AABBConverter::single_dump_aabb_binary(std::string outputPath, bright::physics::AABB& aabb){
  int start, end;
  int dif;

  std::string fullPathAndName = outputPath+"/"+aabb.name()+".bin";

	//std::cout << "Dumping " << aabb->name() << ": " << std::endl << std::flush;
	start = clock() / (CLOCKS_PER_SEC / 1000);

  //Write the new aabb out to disk.
  std::fstream output(fullPathAndName, std::ios::out | std::ios::trunc | std::ios::binary);
  //std::fstream output(fullPathAndName, std::ios::out | std::ios::trunc);
  aabb.to_stream(output);

	end = clock() / (CLOCKS_PER_SEC / 1000);
	dif = end - start;

  output.close();

	//std::cout << "Dumped " << dif << " ms" << std::endl << std::flush;
}

void AABBConverter::create_aabb_from_vertices(std::vector<glm::vec3>& vertices, std::string aabbName, bright::physics::AABB& aabb){

  //Cut off the obj extension from the name
  int pos = aabbName.find(".");
  std::string newName = aabbName.substr(0, pos);
  aabb.name(newName);

  auto handler = [&] (glm::vec3 vertex) { 
    aabb.add(vertex);
  };
  std::for_each(vertices.begin(), vertices.end(), handler);

  aabb.finalize();

}