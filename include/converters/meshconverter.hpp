#ifndef BRIGHT_CONVERTERS_MESH_CONVERTER_H
#define BRIGHT_CONVERTERS_MESH_CONVERTER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <time.h>
#include <algorithm>
#include <glm/glm.hpp> 
#include "utils/fileworker.hpp"
#include "obj.hpp"
#include "base/mesh.hpp"

namespace bright{

  namespace converters{

/**
 * @ingroup tools
 *
 * @brief The mesh converter class
 * 
 * @details 
 * This is where all mesh creation(reading from binaries) and model file format convertion to meshes functionality resides
 * Currently only support for .obj(WaveFront) file format
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class MeshConverter {
public:

  MeshConverter(std::shared_ptr<bright::utils::FileWorker> pFileWorker);

  void batch_convert_obj_and_dump_mesh_binary(std::string outputPath);
  void single_convert_obj_and_dump_mesh_binary(std::string outputPath, std::string file);
  void single_read_in_mesh_binary(std::string path, std::string file);
  void batch_read_obj_mesh_binary();
  std::shared_ptr<bright::base::Mesh> single_read_obj_mesh_binary(std::shared_ptr<std::ifstream> pStream); 
  std::shared_ptr<bright::base::Mesh> mesh(std::string meshCompositeName);

private:

  std::shared_ptr<bright::base::Mesh> load_mesh_from_obj(std::shared_ptr<Obj> obj);
  void calculate_vertex_faces(std::shared_ptr<bright::base::Mesh> pMesh, std::shared_ptr<Group> g,  std::shared_ptr<GroupLibrary> gl, int start, int end);
  void calculate_normal_faces(std::shared_ptr<bright::base::Mesh> pMesh, std::shared_ptr<Group> g,  std::shared_ptr<GroupLibrary> gl, int start, int end);
  void calculate_texture_faces(std::shared_ptr<bright::base::Mesh> pMesh, std::shared_ptr<Group> g, std::shared_ptr<GroupLibrary> gl, int start, int end);
  void calculate_face_indices(std::shared_ptr<bright::base::Mesh> pMesh);
  std::shared_ptr<Obj> single_load_obj(std::string file);
  std::shared_ptr<bright::base::Mesh> single_convert_obj_to_mesh(std::shared_ptr<Obj> obj); 
  void single_dump_mesh_binary(std::string outputPath, std::shared_ptr<bright::base::Mesh> mesh);

  std::map<std::string, std::shared_ptr<bright::base::Mesh>> meshes_;

  std::shared_ptr<bright::utils::FileWorker> pFileWorker_;

};

  }
}  

#endif