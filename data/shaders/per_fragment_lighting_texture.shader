//Vertex shader
#version 460 core

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

  //texCoord = vec2(position.xy);

  gl_Position = cameraToClipMatrix * (worldToCamMatrix * (modelToWorldMatrix * position));
   
  vec3 normWorldSpace = normalize(normalToWorldMatrix * normal);

  texCoord = texture;
  vertexNormal = normWorldSpace;
  modelSpacePosition = vec3(position.xyz);

}

//Fragment shader
#version 460 core

in vec3 vertexNormal;
in vec3 modelSpacePosition;
in vec3 texCoord;

out vec4 outputColor;

uniform vec3 dirToLight;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

uniform sampler2D colorTexture;

void main(){
  
  //So we can pass in our light at any distance and have it normalized
  //The dot product to get the cosAngIncidence requires to unit vectors
  vec3 normalizedDirToLight = normalize(dirToLight);
  
  float cosAngIncidence = dot(vertexNormal, normalizedDirToLight);
  cosAngIncidence = clamp(cosAngIncidence, 0, 1);
  
  vec4 diffuseColorFromTex = texture(colorTexture, texCoord.xy);

  outputColor =  (lightIntensity * diffuseColorFromTex * cosAngIncidence) + (diffuseColorFromTex * ambientIntensity);
  
}