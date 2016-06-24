#ifndef BRIGHT_BASE_MESH_H
#define BRIGHT_BASE_MESH_H

//Include glew before gl.h
#include "Gl/glew.h"
#include "Gl/wglew.h"
#include "glm/glm.hpp"
#include <memory>
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <algorithm>


namespace bright{

  namespace base{

class Mesh{

public:

  Mesh();

  std::vector<glm::vec4>& vertices();
  void vertices(glm::vec4 vertex);

  std::vector<glm::vec3>& normals();
  void normals(glm::vec3 normal);

  std::vector<glm::vec3>& textures();
  void textures(glm::vec3 texture);

  std::vector<GLushort>& vertex_elements();
  void vertex_elements(GLushort element);

  std::vector<GLushort>& normal_elements();
  void normal_elements(GLushort element);

  std::vector<GLushort>& texture_elements();
  void texture_elements(GLushort element);

  std::vector<glm::vec4>& vertex_faces();
  void vertex_faces(glm::vec4 vertexFace);

  std::vector<glm::vec3>& normal_faces();
  void normal_faces(glm::vec3 normalFace);

  std::vector<glm::vec3>& texture_faces();
  void texture_faces(glm::vec3 textureFace);

  std::vector<std::shared_ptr<Mesh>>& child_meshes();
  void child_meshes(std::shared_ptr<Mesh> pChildMesh);

  void count_faces(int count);
  int count_faces();

  void count_vertex_faces(int count);
  int count_vertex_faces();

  void count_normal_faces(int count);
  int count_normal_faces();

  void count_texture_faces(int count);
  int count_texture_faces();

  void count_vertices(int count);
  int count_vertices();

  void count_textures(int count);
  int count_textures();

  void count_normals(int count);
  int count_normals();

  void count_vertex_elements(int count);
  int count_vertex_elements();

  void count_normal_elements(int count);
  int count_normal_elements();

  void count_texture_elements(int count);
  int count_texture_elements();

  void count_children(int count);
  int count_children();

  void face_indices(unsigned int index);
  std::vector<unsigned int>& face_indices();

  void composite_name(std::string fileName);
  std::string composite_name();

  void is_root(bool value);
  bool is_root();

  void part_name(std::string partName);
  std::string part_name();

  void has_children(bool value);
  bool has_children();

  void material(std::string material);
  std::string material();

  void to_stream(std::ofstream &out);
  void from_stream(std::shared_ptr<std::ifstream> pIn);


private:

  bool hasChildren_;
  bool isRoot_;
  std::string compositeName_;
  std::string partName_;
  std::string material_;
  std::vector<glm::vec4> vertices_;
  std::vector<glm::vec3> normals_;
  std::vector<glm::vec3> textures_;
  std::vector<GLushort> vertexElements_;
  std::vector<GLushort> normalElements_;
  std::vector<GLushort> textureElements_;
  std::vector<glm::vec4> vertexFaces_;
  std::vector<glm::vec3> normalFaces_;
  std::vector<glm::vec3> textureFaces_;
  std::vector<std::shared_ptr<Mesh>> childMeshes_;
  std::vector<unsigned int> faceIndices_;
  int countV_;
  int countN_;
  int countT_;
  int countVE_;
  int countNE_;
  int countTE_;
  int countVF_;
  int countNF_;
  int countTF_;
  int countF_;
  int countChildren_;

};

  }

}

#endif