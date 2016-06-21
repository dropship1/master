#include "converters/meshconverter.hpp"

using namespace bright::converters;

MeshConverter::MeshConverter(std::shared_ptr<bright::utils::FileWorker> pFileWorker): pFileWorker_(pFileWorker){}

void MeshConverter::batch_convert_obj_and_dump_mesh_binary(std::string path, std::vector<std::string> files){  

  //Loop over all files
  auto handle_file = [&] (std::string file) {
    single_convert_obj_and_dump_mesh_binary(path, file);
  };

  std::for_each(files.begin(), files.end(), handle_file);
	
}

std::shared_ptr<bright::base::Mesh> MeshConverter::mesh(std::string meshCompositeName){
  return meshes_[meshCompositeName];
}

void MeshConverter::batch_read_obj_mesh_binary(){

  std::string meshList =  pFileWorker_->get_file_contents("mesh.res");
  std::stringstream in(meshList);
  std::string line; 
  std::string meshFile;
  std::vector<std::string> meshFileList;
  bool inMeshNode = false; 
  while (getline(in, line)){
    if (line.substr(0,1) == "]"){
      break;
    }
    if (inMeshNode){
      if(line.substr(0,5) == "file="){
        meshFile = line.substr(5);
      }
      else if(line.substr(0,7) == "</Mesh>"){
        meshFileList.push_back( meshFile );
        inMeshNode = false;
      }
    }
    else if (line.substr(0,6) == "<Mesh>"){ 
      inMeshNode = true;
    }
  }




  std::vector<std::shared_ptr<std::istringstream>> streams = pFileWorker_->get_files_streams(meshFileList);
  //Loop over all streams
  auto handle_stream = [&] (std::shared_ptr<std::istringstream> pStream) { 
    auto pMesh = single_read_obj_mesh_binary(pStream);
    meshes_[pMesh->composite_name()] = pMesh;
  };
  std::for_each(streams.begin(), streams.end(), handle_stream);

}

std::shared_ptr<bright::base::Mesh> MeshConverter::single_read_obj_mesh_binary(std::shared_ptr<std::istringstream> pStream){

  auto pMesh = std::make_shared<bright::base::Mesh>();
  pMesh->from_stream(*pStream);

  return pMesh;
}

void MeshConverter::single_convert_obj_and_dump_mesh_binary(std::string path, std::string file){  
	std::shared_ptr<Obj> obj = single_load_obj(path, file);
	std::shared_ptr<bright::base::Mesh> mesh = single_convert_obj_to_mesh(path, obj); 
	single_dump_mesh_binary(path, mesh);
}

std::shared_ptr<Obj> MeshConverter::single_load_obj(std::string path, std::string file){  

  int start, end;
  int dif;

  std::cout << "Loading " << file+".obj: " << std::endl << std::flush;
  auto obj = std::make_shared<Obj>();

  start = clock() / (CLOCKS_PER_SEC / 1000);

  obj->fileName_ = file;
  obj->load_obj_file(file+".obj", path);

  end = clock() / (CLOCKS_PER_SEC / 1000);
  dif = end - start;

  std::cout << "Loaded " << dif << " ms" << std::endl << std::flush;

  return obj;
}

std::shared_ptr<bright::base::Mesh> MeshConverter::single_convert_obj_to_mesh(std::string path, std::shared_ptr<Obj> obj){

  int start, end;
  int dif;

	std::cout << "Converting " << obj->fileName_ << ": " << std::endl << std::flush;

	auto mesh = std::make_shared<bright::base::Mesh>();

	start = clock() / (CLOCKS_PER_SEC / 1000);

	mesh = load_mesh_from_obj(obj);

	end = clock() / (CLOCKS_PER_SEC / 1000);
	dif = end - start;

	std::cout << "Converted " << dif << " ms" << std::endl << std::flush;
	
	return mesh;
}

void MeshConverter::single_dump_mesh_binary(std::string path, std::shared_ptr<bright::base::Mesh> mesh){
  int start, end;
  int dif;

  std::string fullPathAndName = path+"/"+mesh->composite_name()+".bin";

	std::cout << "Dumping " << mesh->composite_name() << ": " << std::endl << std::flush;
	start = clock() / (CLOCKS_PER_SEC / 1000);

  //Write the new mesh out to disk.
  std::ofstream output;
  output.open(fullPathAndName, std::ios::binary | std::ios::out | std::ios::trunc);

  //std::fstream output(fullPathAndName, std::ios::out | std::ios::trunc);
  mesh->to_stream(output);

	end = clock() / (CLOCKS_PER_SEC / 1000);
	dif = end - start;

  output.close();

	std::cout << "Dumped " << dif << " ms" << std::endl << std::flush;
}

std::shared_ptr<bright::base::Mesh> MeshConverter::load_mesh_from_obj(std::shared_ptr<Obj> obj){
 
  auto rootMesh = std::make_shared<bright::base::Mesh>();
  rootMesh->composite_name(obj->fileName_);
  rootMesh->is_root(true);
  rootMesh->part_name("Root");
  rootMesh->has_children(true);

  //Loop over each group(which is a mesh)
  int countChildren = 1;
  auto handle_group = [&] (std::shared_ptr<Group> g) { 
    auto mesh = std::make_shared<bright::base::Mesh>();
    rootMesh->child_meshes(mesh);
    rootMesh->count_children(countChildren);
    ++countChildren;
    mesh->composite_name(obj->fileName_);
    mesh->is_root(false);
    mesh->has_children(false);
    mesh->part_name(g->name_);
    calculate_vertex_faces(mesh, g, obj->groupLibrary_, 0, g->vertexFaceElems_.size());
    calculate_normal_faces(mesh, g, obj->groupLibrary_, 0, g->normalFaceElems_.size());
    calculate_texture_faces(mesh, g, obj->groupLibrary_, 0, g->textureFaceElems_.size());
    calculate_face_indices(mesh);
    //rootMesh->count_vertex_faces(rootMesh->count_vertex_faces()+mesh->count_vertex_faces());
    if (g->material_ != ""){
      std::shared_ptr<Material> mat = obj->get_material(g->material_);
      if (mat->hasMaps_){
        mesh->material(mat->map_Kd_);
      }
      else{
        mesh->material("default.dds");
      }
    }
    else{
      mesh->material("default.dds");
    }
  };

  std::for_each(obj->groupLibrary_->groups_.begin(), obj->groupLibrary_->groups_.end(), handle_group);
 
  //calculate_face_indices(rootMesh);
  return rootMesh;
}

void MeshConverter::calculate_vertex_faces(std::shared_ptr<bright::base::Mesh> mesh, std::shared_ptr<Group> g, std::shared_ptr<GroupLibrary> gl, int start, int end){
  int count = 0;
  for (int i = start; i < end; ++i){
    GLuint ve = g->vertexFaceElems_[i];
    mesh->vertex_faces( glm::vec4( gl->vertices_[ve].x, gl->vertices_[ve].y, gl->vertices_[ve].z, gl->vertices_[ve].w ) );
    ++count;
  }
  mesh->count_vertex_faces(count);
}

void MeshConverter::calculate_normal_faces(std::shared_ptr<bright::base::Mesh> mesh, std::shared_ptr<Group> g, std::shared_ptr<GroupLibrary> gl, int start, int end){
  int count = 0;
  for (int i = start; i < end; ++i){
    GLuint ve = g->normalFaceElems_[i];
    mesh->normal_faces( glm::vec3(gl->normals_[ve].x, gl->normals_[ve].y, gl->normals_[ve].z) );
    ++count;
  }
  mesh->count_normal_faces(count);
}

void MeshConverter::calculate_texture_faces(std::shared_ptr<bright::base::Mesh> mesh, std::shared_ptr<Group> g, std::shared_ptr<GroupLibrary> gl, int start, int end){
  int count = 0;
  for (int i = start; i < end; ++i){
    GLuint ve = g->textureFaceElems_[i];
    mesh->texture_faces( glm::vec3(gl->textures_[ve].x, gl->textures_[ve].y, gl->textures_[ve].z) );
    ++count;
  }
  mesh->count_texture_faces(count);
}

void MeshConverter::calculate_face_indices(std::shared_ptr<bright::base::Mesh> mesh){
  mesh->count_faces( mesh->count_vertex_faces() );
  for (int i = 0; i < mesh->count_faces(); ++i){
    mesh->face_indices(i);
  }
}