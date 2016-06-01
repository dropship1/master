#version 330

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