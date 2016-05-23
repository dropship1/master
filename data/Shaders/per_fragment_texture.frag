#version 330

in vec3 vertexNormal;
in vec3 modelSpacePosition;
in vec3 texCoord;

out vec4 outputColor;

uniform sampler2D colorTexture;

void main(){
  
  outputColor =  texture(colorTexture, texCoord.xy);
  
}