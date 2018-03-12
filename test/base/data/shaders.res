                                                    
#------------------------------
#Explanation
#------------------------------

#This is a Comment   

#Format explained
#<Node>
#Key=Value
#...
#<Node>
#Key=Value
#...
#Example
#<Shader>
#file=per_frag_light.shader
#type=PER_FRAG_LIGHT
#perspective=cameraToClipMatrix

#Shader
#Supported Nodes
#<Shader>
#vertexshader=file name of the vertex shader
#fragmentshader=file name of the fragment shader
#type=type of shader, this allows to look up the shader when you want to apply it to an actor
#perspective=should we use perspective cliping for the camera? if so then this is the name of the uniform for that




#------------------------------
#Start
#------------------------------

<Shader>
file=pflt
perspective=cameraToClipMatrix
type=PER_FRAG_LIGHT_TEXTURE
textureunit=colorTexture
</Shader>

<Shader>
file=pflc
perspective=cameraToClipMatrix
type=PER_FRAG_LIGHT_COLOR
</Shader>

<Shader>
file=pft
perspective=cameraToClipMatrix
type=PER_FRAG_TEXTURE
</Shader>

<Shader>
file=pfc
perspective=cameraToClipMatrix
type=PER_FRAG_COLOR
</Shader>

]
#------------------------------
#END
#------------------------------
















