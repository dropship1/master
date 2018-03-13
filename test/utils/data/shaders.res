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
file=pflt
perspective=cameraToClipMatrix
type=PER_FRAG_LIGHT_TEXTURE
textureuniformname=colorTexture
textureunit=0
samplerid=5
</Shader>

<Shader>
file=pflc
perspective=cameraToClipMatrix
type=PER_FRAG_LIGHT_COLOR
samplerid=5
</Shader>

]
#------------------------------
#END
#------------------------------
















