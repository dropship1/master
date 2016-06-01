#version 330

in vec3 vertexNormal;
in vec3 modelSpacePosition;

out vec4 outputColor;

uniform vec4 diffuseColor;


void main(){

  outputColor =  diffuseColor;
  
}