                                                    
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

#<RenderActor>
#name= the unique name of resource
#mesh= what mesh to use for this actor (This mesh will implicitly give the textures to use)
#shader= what shader to use
#cameratype= what type of camera? if not set then defaults to first person
#</Actor>

#Example
#<RenderActor>
#name=Box
#mesh=cube
#shader=PER_FRAG_VERT_LIGHT
#cameratype=1st
#</RenderActor>



#------------------------------
#Start
#------------------------------

<RenderActor>
name=Box
mesh=altair
shader=PER_FRAG_LIGHT_TEXTURE
cameratype=1st
</RenderActor>
