//Vertex shader
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texture;

out vec3 vertexNormal;

uniform mat4 cameraToClipMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 worldToCamMatrix;
uniform mat3 normalToWorldMatrix;

void main(){

  gl_Position = cameraToClipMatrix * (worldToCamMatrix * (modelToWorldMatrix * position));
   
  vec3 normWorldSpace = normalize(normalToWorldMatrix * normal);

  vertexNormal = normWorldSpace;

}


//Fragment shader
#version 330 core

in vec3 vertexNormal;
in vec3 modelSpacePosition;
in vec3 texCoord;

out vec4 outputColor;

uniform vec4 diffuseColor;
uniform vec3 dirToLight;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;


void main(){
  
  //So we can pass in our light at any distance and have it normalized
  //The dot product to get the cosAngIncidence requires to unit vectors
  vec3 normalizedDirToLight = normalize(dirToLight);
  
  float cosAngIncidence = dot(vertexNormal, normalizedDirToLight);
  cosAngIncidence = clamp(cosAngIncidence, 0, 1);

  outputColor =  (lightIntensity * diffuseColor * cosAngIncidence) + (diffuseColor * ambientIntensity);
  
}