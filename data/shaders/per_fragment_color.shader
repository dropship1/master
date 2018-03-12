//Vertex shader
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

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

out vec4 outputColor;

uniform vec4 diffuseColor;


void main(){

  outputColor =  diffuseColor;
  
}