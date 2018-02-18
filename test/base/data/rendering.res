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
NAME=Lucy
MESH=altair
SHADER=PER_FRAG_TEXTURE
CAMERATYPE=1st
</RenderActor>

<RenderActor>
NAME=Kane
MESH=altair
SHADER=PER_FRAG_TEXTURE
CAMERATYPE=1st
</RenderActor>

<RenderActor>
NAME=Zombie_001
MESH=altair
SHADER=PER_FRAG_TEXTURE
CAMERATYPE=1st
</RenderActor>