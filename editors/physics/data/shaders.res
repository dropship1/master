                                                    
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
#vertexshader=per_frag_light.vert
#fragmentshader=per_frag_light.frag
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
vertexshader=pflt.v
fragmentshader=pflt.f
perspective=cameraToClipMatrix
type=PER_FRAG_LIGHT_TEXTURE
textureunit=colorTexture
</Shader>

<Shader>
vertexshader=pflc.v
fragmentshader=pflc.f
perspective=cameraToClipMatrix
type=PER_FRAG_LIGHT_COLOR
</Shader>

<Shader>
vertexshader=pft.v
fragmentshader=pft.f
perspective=cameraToClipMatrix
type=PER_FRAG_TEXTURE
textureunit=colorTexture
</Shader>

<Shader>
vertexshader=pfc.v
fragmentshader=pfc.f
perspective=cameraToClipMatrix
type=PER_FRAG_COLOR
</Shader>

]
#------------------------------
#END
#------------------------------
















