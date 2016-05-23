#include <iostream>
#include "physics/aabb.hpp"
#include <memory>
#include <algorithm>
#include <vector>
#include "glm/glm.hpp"
#include <string>

bool check_intersect(std::shared_ptr<bright::physics::AABB> pAABBPlayer,std::shared_ptr<bright::physics::AABB> pAABBMonster);
void current_information(std::shared_ptr<bright::physics::AABB> pAABBPlayer, std::string who);

int main(){

  std::shared_ptr<bright::physics::AABB> pCurrentAABB;

  auto pAABBPlayer  = std::make_shared<bright::physics::AABB>( glm::vec3(0.0f,0.0f,0.0f) );
  std::vector<glm::vec3> playerVertices;
  playerVertices.push_back( glm::vec3(0.5f, 2.5f, 0.5f) );
  playerVertices.push_back( glm::vec3(0.1f, 2.0f, 0.3f) );
  playerVertices.push_back( glm::vec3(0.6f, 2.0f, 0.6f) );
  

  auto pAABBMonster = std::make_shared<bright::physics::AABB>( glm::vec3(5.0f,0.0f,5.0f) );
  std::vector<glm::vec3> monsterVertices;
  monsterVertices.push_back( glm::vec3(0.5f, 2.5f, 0.5f) );
  monsterVertices.push_back( glm::vec3(0.1f, 2.0f, 0.3f) );
  monsterVertices.push_back( glm::vec3(0.6f, 2.0f, 0.6f) );

  auto verify_adding_vertices = [&] (glm::vec3 vertex) {
    std::cout << "Vertex added: " << vertex.x << " : " << vertex.y << " : " << vertex.z << std::endl << std::flush;
    pCurrentAABB->add(vertex);
    pCurrentAABB->finalize();

    glm::vec3 max = pCurrentAABB->max();
    glm::vec3 min = pCurrentAABB->min();
    std::cout << "max: "         << max.x << " : " << max.y << " : " << max.z << std::endl << std::flush;
    std::cout << "min: "         << min.x << " : " << min.y << " : " << min.z << std::endl << std::flush;
    std::cout << "width: "       << pCurrentAABB->width() << std::endl << std::flush;
    std::cout << "half_width: "  << pCurrentAABB->half_width() << std::endl << std::flush;
    std::cout << "height: "      << pCurrentAABB->height() << std::endl << std::flush;
    std::cout << "half_height: " << pCurrentAABB->half_height() << std::endl << std::flush;
    std::cout << "length: "      << pCurrentAABB->length() << std::endl << std::flush;
    std::cout << "half_length: " << pCurrentAABB->half_length() << std::endl << std::flush;
    std::cout << std::endl << std::flush;
  };

  std::cout << "Player: " << std::endl << std::flush;
  pCurrentAABB = pAABBPlayer;
  std::for_each(playerVertices.begin(), playerVertices.end(), verify_adding_vertices);

  pCurrentAABB = pAABBMonster;
  std::cout << "Monster: " << std::endl << std::flush;
  std::for_each(monsterVertices.begin(), monsterVertices.end(), verify_adding_vertices);

  std::vector<glm::vec3> xPositions;
  xPositions.push_back( glm::vec3(1.0f,0.0f,0.0f) );
  xPositions.push_back( glm::vec3(2.0f,0.0f,0.0f) );
  xPositions.push_back( glm::vec3(3.0f,0.0f,0.0f) );
  xPositions.push_back( glm::vec3(4.0f,0.0f,0.0f) );
  xPositions.push_back( glm::vec3(5.0f,0.0f,0.0f) );
  xPositions.push_back( glm::vec3(1.25f,0.0f,0.0f) );
  xPositions.push_back( glm::vec3(2.25f,0.0f,0.0f) );
  xPositions.push_back( glm::vec3(3.25f,0.0f,0.0f) );
  xPositions.push_back( glm::vec3(4.25f,0.0f,0.0f) );
  xPositions.push_back( glm::vec3(5.25f,0.0f,0.0f) );

  std::vector<glm::vec3> yPositions;
  yPositions.push_back( glm::vec3(0.0f,1.0f,0.0f) );
  yPositions.push_back( glm::vec3(0.0f,2.0f,0.0f) );
  yPositions.push_back( glm::vec3(0.0f,3.0f,0.0f) );
  yPositions.push_back( glm::vec3(0.0f,4.0f,0.0f) );
  yPositions.push_back( glm::vec3(0.0f,5.0f,0.0f) );
  yPositions.push_back( glm::vec3(0.0f,1.25f,0.0f) );
  yPositions.push_back( glm::vec3(0.0f,2.25f,0.0f) );
  yPositions.push_back( glm::vec3(0.0f,3.25f,0.0f) );
  yPositions.push_back( glm::vec3(0.0f,4.25f,0.0f) );
  yPositions.push_back( glm::vec3(0.0f,5.25f,0.0f) );

  std::vector<glm::vec3> zPositions;
  zPositions.push_back( glm::vec3(0.0f,0.0f,1.0f) );
  zPositions.push_back( glm::vec3(0.0f,0.0f,2.0f) );
  zPositions.push_back( glm::vec3(0.0f,0.0f,3.0f) );
  zPositions.push_back( glm::vec3(0.0f,0.0f,4.0f) );
  zPositions.push_back( glm::vec3(0.0f,0.0f,5.0f) );
  zPositions.push_back( glm::vec3(0.0f,0.0f,1.25f) );
  zPositions.push_back( glm::vec3(0.0f,0.0f,2.25f) );
  zPositions.push_back( glm::vec3(0.0f,0.0f,3.25f) );
  zPositions.push_back( glm::vec3(0.0f,0.0f,4.25f) );
  zPositions.push_back( glm::vec3(0.0f,0.0f,5.25f) );

  std::vector<glm::vec3> xzPositions;
  xzPositions.push_back( glm::vec3(1.0f,0.0f,1.0f) );
  xzPositions.push_back( glm::vec3(2.0f,0.0f,2.0f) );
  xzPositions.push_back( glm::vec3(3.0f,0.0f,3.0f) );
  xzPositions.push_back( glm::vec3(4.0f,0.0f,4.0f) );
  //(This is in the intersect function in AABB)
  //Here they will intersect since the halfLenth is 0.15 and the current max z on player is 4.71+0.15 = 4.86
  //and the min z on Monster is 5.0-0.15 = 4.85 so max will be -0.1 which is > 0 
  //and the current Max x on Player is 5.0+0.25 = 5.25 and Curr Min z on monster is 5.0-0.25 = 4.75 so max will be -0.5
  //and the current Max y on Player is 0.0+0.25 = 0.25 and Curr Min y on monster is 0.0-0.25 = -0.25 so max will be -0.5
  xzPositions.push_back( glm::vec3(5.0f,0.0f,4.71f) );

  auto check_intersection = [&] (glm::vec3 pos) {
    bool intersect = false;
    pAABBPlayer->move(pos);
    current_information(pAABBPlayer, "Player ");
    current_information(pAABBMonster, "Monster");
    intersect = check_intersect(pAABBPlayer, pAABBMonster);
    std::cout << std::endl << std::flush;
  };
  std::for_each(xPositions.begin(), xPositions.end(), check_intersection);
  std::for_each(yPositions.begin(), yPositions.end(), check_intersection);
  std::for_each(zPositions.begin(), zPositions.end(), check_intersection);
  std::for_each(xzPositions.begin(), xzPositions.end(), check_intersection);
 

  std::cout << "ctr+c to exit or x on the window" << std::endl;
  while (true){ }

  return 0;
}


bool check_intersect(std::shared_ptr<bright::physics::AABB> pAABBPlayer, std::shared_ptr<bright::physics::AABB> pAABBMonster){
  bool intersect = pAABBPlayer->intersect(pAABBMonster);
  std::cout << "intersect:? " << (intersect ? "Yes" : "No") << std::endl << std::flush;
  return intersect;
}

void current_information(std::shared_ptr<bright::physics::AABB> pAABB, std::string who){
  glm::vec3 currPos = pAABB->curr_pos();
  std::cout << who << ": " "currPos: " << currPos.x << " : " << currPos.y << " : " << currPos.z << " ";
  std::cout << "halfWidth: " << pAABB->half_width() << " ";
  std::cout << "halfHeight: " << pAABB->half_height() << " ";
  std::cout << "halfLength: " << pAABB->half_length() << " ";
  std::cout << std::endl << std::flush;
}