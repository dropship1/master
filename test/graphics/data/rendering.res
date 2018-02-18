                                                    
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

#<Actor>
#name=Jake
#mesh=monster_jetpack_003
#shader=PER_FRAG_LIGHT_TEXTURE
#player=true
#cameratype=1st
#</Actor>

<RenderActor>
NAME=Jake
MESH=monster_jetpack_003
SHADER=PER_FRAG_TEXTURE
CAMERATYPE=1st
</RenderActor>