--libraries info
-glm library is header only
-catch(catch.h) is header only
-gl is headers and also needs to link to opengl32.dll
  We don't include this in the engine directories so that is uses whatever version the player has on their computer
-glew is headers and libraries


--premake commands
-older command: 'premake4.exe --os=windows --platform=x64 vs2010'
-newer command: 'premake5.exe --os=windows vs2012'


--doxy command
doxygen.exe Doxyfile
Doxyfile is a config file