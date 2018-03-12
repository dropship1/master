--libraries info
-glm library is header only
-catch(catch.h) is header only
-gl is headers and also needs to link to opengl32.dll
  We don't include this in the engine directories so that is uses whatever version the player has on their computer
-glew is headers and libraries
-openal, you need to get the sdk installer from the website, install it, then grab the win32 and win64 directories and copy them to our /lib directory and rename them to something like OpenAl-win32..


--doxy command
doxygen.exe Doxyfile
Doxyfile is a config file

--Cmake (We only build for 64bit, hence the Win64 in the below command
Make sure to build 'out-of-source' with cmake by doing:
cd master/build
 cmake.exe -G"Visual Studio 15 2017 Win64" ..
This will put everything into the build directory and you run from there.
If you want to disable the console/debug output for release mode for opengl that is in openglcontext.hpp and .ccp
you need to pass in the flag to cmake to turn it off (Its on by default) and this will undefine the preprocessor definition
 cmake.exe -DDebug_Opengl=OFF -G"Visual Studio 15 2017 Win64" ..