local besdkdir = os.getcwd();

function create_test(intype, fullnameoffiletesting, inlibdirs, inincludedirs, infiles, inlinks, inlinksD, inflags, intargetdir)
project_name = "Test_" .. fullnameoffiletesting;
print("    -" .. fullnameoffiletesting);
project(project_name)
  kind(intype);
  targetdir(intargetdir .. "/bin")
  language "c++"

  libdirs(inlibdirs);

  includedirs(inincludedirs);

  files(infiles);  
  
  configuration "windows"
    defines "WIN32"

  configuration "Debug"
    defines {"DEBUG", "_DEBUG", "MEMORY_DEBUGGING", "_VARIADIC_MAX=10"};
    flags{ "Symbols" };
    flags{ "Unicode" };
    flags{ inflags };
    if (intype == "WindowedApp") then
      flags{ "WinMain" }
    end
    objdir "Debug";
    targetname(project_name .. "D");
    links(inlinksD);

  configuration "Release"
    defines {"NDEBUG", "RELEASE", "_VARIADIC_MAX=10"};
    flags{ "Symbols" };
    flags{ "Unicode" };
    flags{ inflags };
    flags{ "OptimizeSpeed" };
    flags{ "NoFramePointer" };
    flags{ "ExtraWarnings" };
    flags{ "NoEditAndContinue" };
    if (intype == "WindowedApp") then
      flags{ "WinMain" }
    end
    objdir "Release";
    targetname(project_name);
    links(inlinks);

end


function create_game(intype, fullnameofgame, inlibdirs, inincludedirs, infiles, inlinks, inlinksD, inflags, intargetdir)
project_name = "Game_" .. fullnameofgame;
print("    -" .. fullnameofgame);
project(project_name)
  kind(intype);
  targetdir(intargetdir .. "/bin")
  language "c++"

  libdirs(inlibdirs);

  includedirs(inincludedirs);

  files(infiles);  
  
  configuration "windows"
    defines "WIN32"

  configuration "Debug"
    defines {"DEBUG", "_DEBUG", "MEMORY_DEBUGGING", "_VARIADIC_MAX=10"};
    flags{ "Symbols" };
    flags{ "Unicode" };
    flags{ inflags };
    if (intype == "WindowedApp") then
      flags{ "WinMain" }
    end
    objdir(intargetdir .. "/Debug");
    targetname(project_name .. "D");
    links(inlinksD);

  configuration "Release"
    defines {"NDEBUG", "RELEASE", "_VARIADIC_MAX=10"};
    flags{ "Symbols" };
    flags{ "Unicode" };
    flags{ inflags };
    flags{ "OptimizeSpeed" };
    flags{ "NoFramePointer" };
    flags{ "ExtraWarnings" };
    flags{ "NoEditAndContinue" };
    if (intype == "WindowedApp") then
      flags{ "WinMain" }
    end
    objdir(intargetdir .. "/Release");
    targetname(project_name);
    links(inlinks);

end



function create_editor(intype, fullnameofgame, inlibdirs, inincludedirs, infiles, inlinks, inlinksD, inflags, intargetdir)
project_name = "Editor_" .. fullnameofgame;
print("    -" .. fullnameofgame);
project(project_name)
  kind(intype);
  targetdir(intargetdir .. "/bin")
  language "c++"

  libdirs(inlibdirs);

  includedirs(inincludedirs);

  files(infiles);  
  
  configuration "windows"
    defines "WIN32"

  configuration "Debug"
    defines {"DEBUG", "_DEBUG", "MEMORY_DEBUGGING", "_VARIADIC_MAX=10"};
    flags{ "Symbols" };
    flags{ "Unicode" };
    flags{ inflags };
    if (intype == "WindowedApp") then
      flags{ "WinMain" }
    end
    objdir(intargetdir .. "/Debug");
    targetname(project_name .. "D");
    links(inlinksD);

  configuration "Release"
    defines {"NDEBUG", "RELEASE", "_VARIADIC_MAX=10"};
    flags{ "Symbols" };
    flags{ "Unicode" };
    flags{ inflags };
    flags{ "OptimizeSpeed" };
    flags{ "NoFramePointer" };
    flags{ "ExtraWarnings" };
    flags{ "NoEditAndContinue" };
    if (intype == "WindowedApp") then
      flags{ "WinMain" }
    end
    objdir(intargetdir .. "/Release");
    targetname(project_name);
    links(inlinks);

end



--######################### SERVER ################################################

print();
print("Creating Bright Server Solution");
solution "brightServer"
  configurations {"Debug", "Release"}
  defines {
        "_CRT_SECURE_NO_WARNINGS", 
        "_CRT_SECURE_NO_DEPRECATE", 
        "_SCL_SECURE_NO_WARNINGS"
    }


print("  -Creating BeSdk Server Static Lib");
project("besdkServer")
  kind "StaticLib"
  targetdir "lib"
  language "c++"

  libdirs {"lib"}

  includedirs {
    besdkdir,
    "include",
    "boost-1.58.0",
  }

  files {
    "include/ai/*.hpp",
    "include/base/controlactor.hpp",
    "include/base/actorcontrolsresourcemanager.hpp",
    "include/base/actorcontrolcontroller.hpp",
    "include/physics/*.hpp",
    "include/network/asyncserver.hpp",
    "include/network/globalstructs.hpp",
    "include/network/networkmessage.hpp",
    "include/network/clientconnection.hpp",
    "include/network/clienthandler.hpp",
    "include/network/loginmessage.hpp",
    "include/network/loginresponsemessage.hpp",
    "include/network/updatemessage.hpp",
    "include/input/globalstructs.hpp",
    "include/input/commandmessage.hpp",
    "include/converters/aabbconverter.hpp",
    "include/utils/*.hpp",
    "source/ai/*.cpp",
    "source/base/controlactor.cpp",
    "source/base/actorcontrolsresourcemanager.cpp",
    "source/base/actorcontrolcontroller.cpp",
    "source/physics/*.cpp",
    "source/network/asyncserver.cpp",
    "source/network/networkmessage.cpp",
    "source/network/clientconnection.cpp",
    "source/network/clienthandler.cpp",
    "source/network/loginmessage.cpp",
    "source/network/loginresponsemessage.cpp",
    "source/network/updatemessage.cpp",
    "source/input/commandmessage.cpp",
    "source/converters/aabbconverter.cpp",
    "source/utils/*.cpp",
  };  
  
  configuration "windows"
    defines "WIN32"

  configuration "Debug"
    flags { "Unicode"};
    defines {"DEBUG", "_DEBUG", "MEMORY_DEBUGGING", "_VARIADIC_MAX=10"};
    objdir "Debug";
    flags "Symbols";
    targetname "besdkServerD";
    links {}

  configuration "Release"
    defines {"NDEBUG", "RELEASE", "_VARIADIC_MAX=10"};
    flags { "Unicode" };
    flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue"};
    objdir "Release";
    targetname "besdkServer"
    links {}

print();


print("  -Creating BeSdk Server Console based test");
create_test("ConsoleApp", "Network_AsyncServer", {"lib"}, {besdkdir, "include", "boost-1.58.0"}, {"test/network/asyncserver.cpp"}, {"besdkServer"}, {"besdkServerD"}, {}, "test/network");
create_test("ConsoleApp", "Network_ClientHandler", {"lib"}, {besdkdir, "include"}, {"test/network/clienthandler.cpp"}, {"besdkServer"}, {"besdkServerD"}, {}, "test/network");
create_test("ConsoleApp", "AI_AIManager", {"lib"}, { besdkdir, "include"}, {"test/ai/aimanager.cpp"}, {"besdkServer"}, {"besdkServerD"}, {}, "test/ai");
create_test("ConsoleApp", "Base_ActorControlsResourceManager", {"lib"}, { besdkdir, "include"}, {"test/base/actorcontrolsresourcemanager.cpp", "test/base/data/files.fl", "test/base/data/controlling.wrld"}, {"besdkServer"}, {"besdkServerD"}, {}, "test/base");
create_test("ConsoleApp", "Physics_AABB", {"lib"}, {besdkdir, "include"}, {"test/physics/aabb.cpp"}, {"besdkServer"}, {"besdkServerD"}, {}, "test/physics");
create_test("ConsoleApp", "Converters_AABBConverter", {"lib"}, { besdkdir, "include"}, {"test/converters/aabbconverter.cpp"}, {"besdkServer"}, {"besdkServerD"}, {}, "test/converters");
print();



print("  -Creating Games");
create_game("ConsoleApp", "ChasersServer", {"lib"}, {besdkdir, "include", "boost-1.58.0"}, {"games/chasers/source/chasersserver.cpp"}, {"besdkServer"}, {"besdkServerD"}, {}, "games/chasers");
print();







--######################### CLIENT ################################################

print();
print("Creating Bright Client Solution");
solution "brightClient"
  configurations {"Debug", "Release"}
  defines {
        "_CRT_SECURE_NO_WARNINGS", 
        "_CRT_SECURE_NO_DEPRECATE", 
        "_SCL_SECURE_NO_WARNINGS"
    }


print("  -Creating BeSdk Client Static Lib");
project("besdkClient")
  kind "StaticLib"
  targetdir "lib"
  language "c++"

  libdirs {"lib", "lib/OpenAl-Win32/EFX-Util_MT", "lib/OpenAl-Win32/EFX-Util_MTDLL", "lib/OpenAl-Win32", "lib/OpenAl-Win64/EFX-Util_MT", "lib/OpenAl-Win64/EFX-Util_MTDLL", "lib/OpenAl-Win64"}

  includedirs {
    besdkdir,
    "glew-1.11.0/include",
    "include",
    "boost-1.58.0",
    "openal-1.1"
  }

  files {
    "include/base/*.hpp",
    "include/context/*.hpp",
    "include/graphics/*.hpp",
    "include/input/*.hpp",
    "include/network/*.hpp",
    "include/converters/*.hpp",
    "include/audio/*.hpp",
    "include/utils/*.hpp",
    "include/physics/*.hpp",
    "source/base/*.cpp",
    "source/context/*.cpp",
    "source/graphics/*.cpp",
    "source/input/*.cpp",
    "source/network/*.cpp",
    "source/audio/*.cpp",
    "source/converters/*.cpp",
    "source/utils/*.cpp",
    "source/physics/*.cpp",
  };  
  excludes { 
    "include/network/asyncserver.hpp",
    "include/network/clientconnection.hpp",
    "include/network/clienthandler.hpp",
    "source/network/clientconnection.cpp",
    "source/network/clientconnection.cpp",
    "source/network/clienthandler.cpp",
    "source/network/asyncserver.cpp",
  }
  
  configuration "windows"
    defines "WIN32"

  configuration "Debug"
    flags { "Unicode"};
    defines {"DEBUG", "_DEBUG", "MEMORY_DEBUGGING", "_VARIADIC_MAX=10"};
    objdir "Debug";
    flags "Symbols";
    targetname "besdkClientD";
    links {"glew32d", "opengl32"}

  configuration "Release"
    defines {"NDEBUG", "RELEASE", "_VARIADIC_MAX=10"};
    flags { "Unicode" };
    flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue"};
    objdir "Release";
    targetname "besdkClient"
    links {"glew32", "opengl32"}

print();


print("  -Creating BeSdk Console based test");
create_test("ConsoleApp", "Converters_MeshConverter", {"lib"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/converters/meshconverter.cpp", "test/converters/meshes/obj/*.*"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/converters");
create_test("ConsoleApp", "Graphics_ImageLoader", {"lib"}, {besdkdir, "include"}, {"test/graphics/imageloader.cpp", "test/graphics/data/*.dds"}, {"besdkClient"}, {"besdkClientD"}, {}, "test/graphics");
create_test("ConsoleApp", "Utils_FileWorker", {"lib"}, {besdkdir, "include"}, {"test/utils/fileworker.cpp", "test/utils/data/*.*"}, {"besdkClient"}, {"besdkClientD"}, {}, "test/utils");
create_test("ConsoleApp", "Network_ServerHandler", {"lib"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/network/serverhandler.cpp"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/network");
print();

print("  -Creating BeSdk Window based test");
create_test("WindowedApp", "Graphics_ShaderLoader", {"lib"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/graphics/shaderloader.cpp", "test/graphics/data/*.vert", "test/graphics/data/*.frag"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/graphics");
create_test("WindowedApp", "Graphics_TextureLoader", {"lib"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/graphics/textureloader.cpp", "test/graphics/data/*.dds"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/graphics");
create_test("WindowedApp", "Graphics_LoadersManager", {"lib"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/graphics/loadersmanager.cpp", "test/graphics/data/*.*"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/graphics");
create_test("WindowedApp", "Graphics_Renderer", {"lib"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/graphics/renderer.cpp", "test/graphics/data/*.*"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/graphics");
create_test("WindowedApp", "Graphics_RendererAndTextures", {"lib"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/graphics/rendererandtextures.cpp", "test/graphics/data/*.*"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/graphics");
create_test("WindowedApp", "Graphics_RendererAndActorRenderingResourceManager", {"lib"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/graphics/rendererandactorrenderingresourcemanager.cpp", "test/graphics/data/*.*"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/graphics");
create_test("WindowedApp", "Context_Context", {"lib"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/context/context.cpp"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/context");
create_test("WindowedApp", "Context_ContextManager_And_OpenglContext", {"lib"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/context/contextmanager_and_openglcontext.cpp"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/context");

create_test("WindowedApp", "Input_InputManager", {"lib"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/input/inputmanager.cpp"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/input");

create_test("WindowedApp", "Audio_SoundManager", {"lib", "lib/OpenAl-Win32/EFX-Util_MT", "lib/OpenAl-Win32/EFX-Util_MTDLL", "lib/OpenAl-Win32", "lib/OpenAl-Win64/EFX-Util_MT", "lib/OpenAl-Win64/EFX-Util_MTDLL", "lib/OpenAl-Win64"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/audio/soundmanager.cpp"}, {"besdkClient", "OpenAL32", "EFX-Util", "glew32", "opengl32"}, {"besdkClientD", "OpenAL32", "EFX-Util", "glew32d", "opengl32"}, {}, "test/audio");
create_test("WindowedApp", "Network_ServerConnection", {"lib"}, {besdkdir, "glew-1.11.0/include", "include", "boost-1.58.0"}, {"test/network/serverconnection.cpp", "test/network/data/*.*"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/network");

create_test("WindowedApp", "Input_InputContextManager", {"lib"}, {besdkdir, "glew-1.11.0/include", "include", "boost-1.58.0"}, {"test/input/inputcontextmanager.cpp", "test/input/data/controls_config.cfg", "test/input/data/controls_contexts.cfg", "test/input/data/filelist"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/input");
create_test("WindowedApp", "Base_ActorRenderingResourceManager", {"lib"}, {besdkdir, "glew-1.11.0/include", "include"}, {"test/base/actorrenderingresourcemanager.cpp", "test/base/data/*.*"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "test/base");
print();



print("  -Creating Games");
create_game("WindowedApp", "Chasers", {"lib"}, {besdkdir, "glew-1.11.0/include", "include", "boost-1.58.0", "games/chasers/include/"}, {"games/chasers/source/chasers.cpp", "games/chasers/source/gameengine.cpp", "games/chasers/include/*.hpp", "games/chasers/data/*.*"}, {"besdkClient", "glew32", "opengl32"}, {"besdkClientD", "glew32d", "opengl32"}, {}, "games/chasers");
print();

--######################### EDITOR ################################################

print();
print("Creating Bright Editor Solution");
solution "brightEditor"
  configurations {"Debug", "Release"}
  defines {
        "_CRT_SECURE_NO_WARNINGS", 
        "_CRT_SECURE_NO_DEPRECATE", 
        "_SCL_SECURE_NO_WARNINGS"
    }

print("  -Creating BeSdk Editor Static Lib");
project("besdkEditor")
  kind "StaticLib"
  targetdir "lib"
  language "c++"

  libdirs {"lib", "lib/OpenAl-Win32/EFX-Util_MT", "lib/OpenAl-Win32/EFX-Util_MTDLL", "lib/OpenAl-Win32", "lib/OpenAl-Win64/EFX-Util_MT", "lib/OpenAl-Win64/EFX-Util_MTDLL", "lib/OpenAl-Win64"}

  includedirs {
    besdkdir,
    "glew-1.11.0/include",
    "include",
    "boost-1.58.0",
    "openal-1.1"
  }

  files {
    "include/base/*.hpp",
    "include/context/*.hpp",
    "include/graphics/*.hpp",
    "include/input/*.hpp",
    "include/network/*.hpp",
    "include/converters/*.hpp",
    "include/audio/*.hpp",
    "include/utils/*.hpp",
    "include/physics/*.hpp",
    "source/base/*.cpp",
    "source/context/*.cpp",
    "source/graphics/*.cpp",
    "source/input/*.cpp",
    "source/network/*.cpp",
    "source/audio/*.cpp",
    "source/converters/*.cpp",
    "source/utils/*.cpp",
    "source/physics/*.cpp",
  };
  
  configuration "windows"
    defines "WIN32"

  configuration "Debug"
    flags { "Unicode"};
    defines {"DEBUG", "_DEBUG", "MEMORY_DEBUGGING", "_VARIADIC_MAX=10"};
    objdir "Debug";
    flags "Symbols";
    targetname "besdkEditorD";
    links {"glew32d", "opengl32"}

  configuration "Release"
    defines {"NDEBUG", "RELEASE", "_VARIADIC_MAX=10"};
    flags { "Unicode" };
    flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue"};
    objdir "Release";
    targetname "besdkEditor"
    links {"glew32", "opengl32"}

print();



print("  -Creating Editors");
create_editor("WindowedApp", "PhysicsTester", {"lib"}, {besdkdir, "glew-1.11.0/include", "include", "boost-1.58.0", "editors/physics/include/"}, {"editors/physics/source/physics.cpp", "editors/physics/source/gameengine.cpp", "editors/physics/include/*.hpp", "editors/physics/data/*.*"}, {"besdkEditor", "glew32", "opengl32"}, {"besdkEditorD", "glew32d", "opengl32"}, {}, "editors/physics");
print();