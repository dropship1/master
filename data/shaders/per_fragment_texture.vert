#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texture;

out vec3 vertexNormal;
out vec3 modelSpacePosition;
out vec3 texCoord;

uniform mat4 cameraToClipMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 worldToCamMatrix;
uniform mat3 normalToWorldMatrix;

void main(){

  gl_Position = cameraToClipMatrix * (worldToCamMatrix * (modelToWorldMatrix * position));
   
  vec3 normWorldSpace = normalize(normalToWorldMatrix * normal);

  texCoord = texture;
  vertexNormal = normWorldSpace;
  modelSpacePosition = vec3(position.xyz);

}