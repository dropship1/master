#include "base/mesh.hpp"

using namespace bright::base;


Mesh::Mesh(): countV_(0), countN_(0), countT_(0), countVE_(0), countNE_(0), countTE_(0), 
              countVF_(0), countNF_(0), countTF_(0), countF_(0), countChildren_(0), hasChildren_(false), isRoot_(false),
              compositeName_("default_compositename"),  partName_("default_partname"),  material_("default.dds"){
}

std::vector<glm::vec4>& Mesh::vertices(){
  return vertices_;
}


void Mesh::vertices(glm::vec4 vertex){
  vertices_.push_back(vertex);
}


std::vector<glm::vec3>& Mesh::normals(){
  return normals_;
}


void Mesh::normals(glm::vec3 normal){
  normals_.push_back(normal);
}


std::vector<glm::vec3>& Mesh::textures(){
  return textures_;
}


void Mesh::textures(glm::vec3 texture){
  textures_.push_back(texture);
}


std::vector<GLushort>& Mesh::vertex_elements(){
  return vertexElements_;
}


void Mesh::vertex_elements(GLushort element){
  vertexElements_.push_back(element);
}


std::vector<GLushort>& Mesh::normal_elements(){
  return normalElements_;
}


void Mesh::normal_elements(GLushort element){
  normalElements_.push_back(element);
}


std::vector<GLushort>& Mesh::texture_elements(){
  return textureElements_;
}


void Mesh::texture_elements(GLushort element){
  textureElements_.push_back(element);
}


std::vector<glm::vec4>& Mesh::vertex_faces(){
  return vertexFaces_;
}


void Mesh::vertex_faces(glm::vec4 vertexFace){
  vertexFaces_.push_back(vertexFace);
}


std::vector<glm::vec3>& Mesh::normal_faces(){
  return normalFaces_;
}


void Mesh::normal_faces(glm::vec3 normalFace){
  normalFaces_.push_back(normalFace);
}


std::vector<glm::vec3>& Mesh::texture_faces(){
  return textureFaces_;
}


void Mesh::texture_faces(glm::vec3 textureFace){
  textureFaces_.push_back(textureFace);
}


std::vector<std::shared_ptr<Mesh>>& Mesh::child_meshes(){
  return childMeshes_;
}


void Mesh::child_meshes(std::shared_ptr<Mesh> pChildMesh){
  childMeshes_.push_back(pChildMesh);
}


void Mesh::count_faces(int count){
  countF_ = count;
}

int Mesh::count_faces(){
  return countF_;
}

void Mesh::count_vertex_faces(int count){
  countVF_ = count;
}

int Mesh::count_vertex_faces(){
  return countVF_;
}


void Mesh::count_normal_faces(int count){
  countNF_ = count;
}

int Mesh::count_normal_faces(){
  return countNF_;
}


void Mesh::count_texture_faces(int count){
  countTF_ = count;
}

int Mesh::count_texture_faces(){
  return countTF_;
}


void Mesh::count_vertices(int count){
  countV_ = count;
}

int Mesh::count_vertices(){
  return countV_;
}


void Mesh::count_textures(int count){
  countT_ = count;
}


int Mesh::count_textures(){
  return countT_;
}


void Mesh::count_normals(int count){
  countN_ = count;
}


int Mesh::count_normals(){
  return countN_;
}


void Mesh::count_vertex_elements(int count){
  countVE_ = count;
}


int Mesh::count_vertex_elements(){
  return countVE_;
}


void Mesh::count_normal_elements(int count){
  countNE_ = count;
}


int Mesh::count_normal_elements(){
  return countNE_;
}


void Mesh::count_texture_elements(int count){
  countTE_ = count;
}


int Mesh::count_texture_elements(){
  return countTE_;
}


void Mesh::count_children(int count){
  countChildren_ = count;
}


int Mesh::count_children(){
  return countChildren_;
}


void Mesh::face_indices(unsigned int index){
  faceIndices_.push_back(index);
}

std::vector<unsigned int>& Mesh::face_indices(){
  return faceIndices_;
}


void Mesh::composite_name(std::string fileName){
  compositeName_ = fileName;
}

std::string Mesh::composite_name(){
  return compositeName_;
}


void Mesh::is_root(bool value){
  isRoot_ = value;
}

bool Mesh::is_root(){
  return isRoot_;
}


void Mesh::part_name(std::string partName){
  partName_ = partName;
}

std::string Mesh::part_name(){
  return partName_;
}


void Mesh::material(std::string material){
  material_ = material;
}

std::string Mesh::material(){
  return material_;
}


void Mesh::has_children(bool value){
  hasChildren_ = value;
}


bool Mesh::has_children(){
  return hasChildren_;
}


void Mesh::to_stream(std::ofstream& out){

  out.write((char*)&hasChildren_,sizeof(bool));
  out.write((char*)&countChildren_,sizeof(int));
  out.write((char*)&isRoot_,sizeof(bool));
  out.write(compositeName_.c_str(),compositeName_.size());
  out.write("\0",sizeof(char)); // null end string for easier reading
  out.write(partName_.c_str(),partName_.size());
  out.write("\0",sizeof(char)); // null end string for easier reading
  out.write(material_.c_str(),material_.size());
  out.write("\0",sizeof(char)); // null end string for easier reading

  out.write((char*)&countVF_,sizeof(int));
  out.write((char*)&countNF_,sizeof(int));
  out.write((char*)&countTF_,sizeof(int));
  out.write((char*)&countF_,sizeof(int));
  out.write((char*)&countV_,sizeof(int));
  out.write((char*)&countN_,sizeof(int));
  out.write((char*)&countT_,sizeof(int));
  out.write((char*)&countVE_,sizeof(int));
  out.write((char*)&countNE_,sizeof(int));
  out.write((char*)&countTE_,sizeof(int));

  auto handle_vec4 = [&] (glm::vec4 v) {
    out.write((char*)&v.x,sizeof(float)); 
    out.write((char*)&v.y,sizeof(float)); 
    out.write((char*)&v.z,sizeof(float)); 
    out.write((char*)&v.w,sizeof(float)); 
  };
  
  auto handle_vec3 = [&] (glm::vec3 v) { 
    out.write((char*)&v.x,sizeof(float)); 
    out.write((char*)&v.y,sizeof(float)); 
    out.write((char*)&v.z,sizeof(float)); 
  };
  
  auto handle_glushort = [&] (GLushort s) { 
    out.write((char*)&s,sizeof(GLushort)); 
  };

  auto handle_indices = [&] (unsigned int i) { 
    out.write((char*)&i,sizeof(unsigned int)); 
  };  
  
  auto handle_children = [&] (std::shared_ptr<Mesh> pChild) { 
    pChild->to_stream(out);
  };

  std::for_each(vertices_.begin(), vertices_.end(), handle_vec4);
  std::for_each(normals_.begin(), normals_.end(), handle_vec3);
  std::for_each(textures_.begin(), textures_.end(), handle_vec3);
  
  std::for_each(vertexElements_.begin(), vertexElements_.end(), handle_glushort);
  std::for_each(normalElements_.begin(), normalElements_.end(), handle_glushort);
  std::for_each(textureElements_.begin(), textureElements_.end(), handle_glushort);

  std::for_each(vertexFaces_.begin(), vertexFaces_.end(), handle_vec4);
  std::for_each(normalFaces_.begin(), normalFaces_.end(), handle_vec3);
  std::for_each(textureFaces_.begin(), textureFaces_.end(), handle_vec3);
  
  std::for_each(faceIndices_.begin(), faceIndices_.end(), handle_indices);    
  
  std::for_each(childMeshes_.begin(), childMeshes_.end(), handle_children);
}

//void Mesh::to_stream(std::ostream &out){
//
//
//  out.write(reinterpret_cast<char *>(this->has_children()),sizeof(bool));
//  out << this->has_children() << std::endl;
//  out << this->count_children() << std::endl;
//  out << this->is_root() << std::endl;
//  out << this->composite_name() << std::endl;
//  out << this->part_name() << std::endl; 
//  out << this->material() << std::endl;
//
//  out << this->count_vertex_faces() << std::endl;
//  out << this->count_normal_faces() << std::endl;
//  out << this->count_texture_faces() << std::endl;
//  out << this->count_faces() << std::endl;
//  out << this->count_vertices() << std::endl;
//  out << this->count_normals() << std::endl;
//  out << this->count_textures() << std::endl;
//  out << this->count_vertex_elements() << std::endl;
//  out << this->count_normal_elements() << std::endl;
//  out << this->count_texture_elements() << std::endl;
//
//  auto handle_vec4 = [&] (glm::vec4 v) { 
//    out << v.x << std::endl;
//    out << v.y << std::endl;
//    out << v.z << std::endl;
//    out << v.w << std::endl;
//  };
//
//  auto handle_vec3 = [&] (glm::vec3 v) { 
//    out << v.x << std::endl;
//    out << v.y << std::endl;
//    out << v.z << std::endl;
//  };
//
//  auto handle_glushort = [&] (GLushort s) { 
//    out << s << std::endl;
//  };
//
//
//  auto handle_children = [&] (std::shared_ptr<Mesh> child) { 
//    child->to_stream(out);
//  };
//
//  auto handle_indices = [&] (unsigned int i) { 
//    out << i << std::endl;
//  };
//
//  std::for_each(this->vertices().begin(), this->vertices().end(), handle_vec4);
//  std::for_each(this->normals().begin(), this->normals().end(), handle_vec3);
//  std::for_each(this->textures().begin(), this->textures().end(), handle_vec3);
//
//  std::for_each(this->vertex_elements().begin(), this->vertex_elements().end(), handle_glushort);
//  std::for_each(this->normal_elements().begin(), this->normal_elements().end(), handle_glushort);
//  std::for_each(this->texture_elements().begin(), this->texture_elements().end(), handle_glushort);
//
//
//  std::for_each(this->vertex_faces().begin(), this->vertex_faces().end(), handle_vec4);
//  std::for_each(this->normal_faces().begin(), this->normal_faces().end(), handle_vec3);
//  std::for_each(this->texture_faces().begin(), this->texture_faces().end(), handle_vec3);
//
//  std::for_each(this->face_indices().begin(), this->face_indices().end(), handle_indices);    
//
//  std::for_each(this->child_meshes().begin(), this->child_meshes().end(), handle_children);
//}


void Mesh::from_stream(std::shared_ptr<std::ifstream> pIn){

  pIn->read((char*)&hasChildren_,sizeof(bool)); 
  pIn->read((char*)&countChildren_,sizeof(int)); 
  pIn->read((char*)&isRoot_,sizeof(bool)); 
  std::getline(*pIn,compositeName_,'\0'); //(remember we null ternimated in binary)
  std::getline(*pIn,partName_,'\0'); //(remember we null ternimated in binary)
  std::getline(*pIn,material_,'\0'); //(remember we null ternimated in binary)

  pIn->read((char*)&countVF_,sizeof(int)); 

  pIn->read((char*)&countNF_,sizeof(int)); 

  pIn->read((char*)&countTF_,sizeof(int)); 

  pIn->read((char*)&countF_,sizeof(int)); 

  pIn->read((char*)&countV_,sizeof(int)); 

  pIn->read((char*)&countN_,sizeof(int)); 

  pIn->read((char*)&countT_,sizeof(int)); 

  pIn->read((char*)&countVE_,sizeof(int)); 

  pIn->read((char*)&countNE_,sizeof(int)); 

  pIn->read((char*)&countTE_,sizeof(int)); 

  for (int n=countV_; n>0; --n) {
    glm::vec4 v;
    pIn->read((char*)&v.x,sizeof(float)); 
    pIn->read((char*)&v.y,sizeof(float)); 
    pIn->read((char*)&v.z,sizeof(float)); 
    pIn->read((char*)&v.w,sizeof(float)); 
    vertices_.push_back(v);
  }
  
  
  for (int n=countN_; n>0; --n) {
    glm::vec3 v;
    pIn->read((char*)&v.x,sizeof(float)); 
    pIn->read((char*)&v.y,sizeof(float)); 
    pIn->read((char*)&v.z,sizeof(float)); 
    normals_.push_back(v);
  }
  
  for (int n=countT_; n>0; --n) {
    glm::vec3 v;
    pIn->read((char*)&v.x,sizeof(float)); 
    pIn->read((char*)&v.y,sizeof(float)); 
    pIn->read((char*)&v.z,sizeof(float)); 
    textures_.push_back(v);
  }
  
  for (int n=countVE_; n>0; --n) {
    GLushort v;
    pIn->read((char*)&v,sizeof(v)); 
    vertexElements_.push_back(v);
  }
  
  
  for (int n=countNE_; n>0; --n) {
    GLushort v;
    pIn->read((char*)&v,sizeof(v)); 
    normalElements_.push_back(v);
  }
  
  for (int n=countTE_; n>0; --n) {
    GLushort v;
    pIn->read((char*)&v,sizeof(v)); 
    textureElements_.push_back(v);
  }
  
  for (int n=countVF_; n>0; --n) {
    glm::vec4 v;
    pIn->read((char*)&v.x,sizeof(float)); 
    pIn->read((char*)&v.y,sizeof(float)); 
    pIn->read((char*)&v.z,sizeof(float)); 
    pIn->read((char*)&v.w,sizeof(float)); 
    vertexFaces_.push_back(v);
  }
  
  
  for (int n=countNF_; n>0; --n) {
    glm::vec3 v;
    pIn->read((char*)&v.x,sizeof(v.x)); 
    pIn->read((char*)&v.y,sizeof(v.y)); 
    pIn->read((char*)&v.z,sizeof(v.z)); 
    normalFaces_.push_back(v);
  }
  
  for (int n=countTF_; n>0; --n) {
    glm::vec3 v;
    pIn->read((char*)&v.x,sizeof(v.x)); 
    pIn->read((char*)&v.y,sizeof(v.y)); 
    pIn->read((char*)&v.z,sizeof(v.z)); 
    textureFaces_.push_back(v);
  }
  
  for (int n=countF_; n>0; --n) {
    unsigned int v;
    pIn->read((char*)&v,sizeof(v)); 
    faceIndices_.push_back(v);
  }
  
  
  for (int n=countChildren_; n>0; --n) {
    auto pChild = std::make_shared<Mesh>();
    pChild->from_stream(pIn);
    childMeshes_.push_back(pChild);
  }

}

//void Mesh::from_stream(std::istream &in){
//
//
//  bool hasChildren;
//  in >> hasChildren;
//  this->has_children(hasChildren);
//
//  int countChildren;
//  in >> countChildren;
//  this->count_children(countChildren);
//
//  bool isRoot;
//  in >> isRoot;
//  this->is_root(isRoot);
//  
//  std::string compositeName;
//  in >> compositeName;
//  this->composite_name(compositeName);
//  
//  std::string partName;
//  in >> partName;
//  this->part_name(partName);
//
//  std::string material;
//  in >> material;
//  this->material(material);
//
//
//  int cvf;
//  in >> cvf;
//  this->count_vertex_faces(cvf);
//
//  int cnf;
//  in >> cnf;
//  this->count_normal_faces(cnf);
//
//  int ctf;
//  in >> ctf;
//  this->count_texture_faces(ctf);
//
//  int cf;
//  in >> cf;
//  this->count_faces(cf);
//
//  int cv;
//  in >> cv;
//  this->count_vertices(cv);
//
//  int cn;
//  in >> cn;
//  this->count_normals(cn);
//
//  int ct;
//  in >> ct;
//  this->count_textures(ct);
//
//
//  int cve;
//  in >> cve;
//  this->count_vertex_elements(cve);
//
//  int cne;
//  in >> cne;
//  this->count_normal_elements(cne);
//
//  int cte;
//  in >> cte;
//  this->count_texture_elements(cte);
//
//  for (int n=this->count_vertices(); n>0; --n) {
//    glm::vec4 v;
//    in >> v.x;
//    in >> v.y;
//    in >> v.z;
//    in >> v.w;
//    this->vertices(v);
//  }
//
//
//  for (int n=this->count_normals(); n>0; --n) {
//    glm::vec3 v;
//    in >> v.x;
//    in >> v.y;
//    in >> v.z;
//    this->normals(v);
//  }
//
//  for (int n=this->count_textures(); n>0; --n) {
//    glm::vec3 v;
//    in >> v.x;
//    in >> v.y;
//    in >> v.z;
//    this->textures(v);
//  }
//
//
//  for (int n=this->count_vertex_faces(); n>0; --n) {
//    glm::vec4 v;
//    in >> v.x;
//    in >> v.y;
//    in >> v.z;
//    in >> v.w;
//    this->vertex_faces(v);
//  }
//
//
//  for (int n=this->count_normal_faces(); n>0; --n) {
//    glm::vec3 v;
//    in >> v.x;
//    in >> v.y;
//    in >> v.z;
//    this->normal_faces(v);
//  }
//
//  for (int n=this->count_texture_faces(); n>0; --n) {
//    glm::vec3 v;
//    in >> v.x;
//    in >> v.y;
//    in >> v.z;
//    this->texture_faces(v);
//  }
//
//
//  for (int n=this->count_vertex_elements(); n>0; --n) {
//    GLushort v;
//    in >> v;
//    this->vertex_elements(v);
//  }
//
//
//  for (int n=this->count_normal_elements(); n>0; --n) {
//    GLushort v;
//    in >> v;
//    this->normal_elements(v);
//  }
//
//  for (int n=this->count_texture_elements(); n>0; --n) {
//    GLushort v;
//    in >> v;
//    this->texture_elements(v);
//  }
//
//  for (int n=this->count_faces(); n>0; --n) {
//    unsigned int v;
//    in >> v;
//    this->face_indices(v);
//  }
//
//
//  for (int n=this->count_children(); n>0; --n) {
//    auto child = std::make_shared<Mesh>();
//    child->from_stream(in);
//    this->child_meshes(child);
//  }
//
//}



