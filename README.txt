--libraries info
-glm library is header only
-catch(catch.h) is header only
-gl is headers and also needs to link to opengl32.dll
  We don't include this in the engine directories so that is uses whatever version the player has on their computer
-glew is headers and libraries
-openal, you need to get the sdk installer from the website, install it, then grab the win32 and win63 directories and copy them to our /lib directory and rename them to something like OpenAl-win32..

--premake commands
-older command: 'premake4.exe --os=windows --platform=x64 vs2010'
-newer command: 'premake5.exe --os=windows vs2012'


--doxy command
doxygen.exe Doxyfile
Doxyfile is a config file

--Cmake
Make sure to build 'out-of-source' with cmake by doing:
cd master/build
cmake.exe ..
This will put everything into the build directory and you run from there.