Use the vc10 project to build this.
Just update to vc2012.

Make sure your directory structure looks like this.
bright/glew-1.11.0/include

you really just need to extract the libs from lib dir and dlls from the bin dir.. and place them in bright/lib
and bright/bin respectively. The only files that are needed as reference are glew-1.11.0/include glew-1.11.0/src