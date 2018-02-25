
cmake_minimum_required (VERSION 3.10.2)

#if(NOT CMAKE_DEBUG_POSTFIX)
#  set(CMAKE_DEBUG_POSTFIX D)
#endif()
IF(MSVC)
  add_definitions(/D_VARIADIC_MAX=10)
ENDIF()

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(functions) # defines some convenient functions


project (brightClient)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

message(STATUS "")
message(STATUS "")
message(STATUS "::::::::  Creating Bright Client Solution  ::::::::")
message(STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
message(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY: " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

file(GLOB_RECURSE include_base_files "${CMAKE_SOURCE_DIR}/include/base/*.hpp")
file(GLOB_RECURSE include_context_files "${CMAKE_SOURCE_DIR}/include/context/*.hpp")
file(GLOB_RECURSE include_graphics_files "${CMAKE_SOURCE_DIR}/include/graphics/*.hpp")
file(GLOB_RECURSE include_input_files "${CMAKE_SOURCE_DIR}/include/input/*.hpp")
file(GLOB_RECURSE include_network_files "${CMAKE_SOURCE_DIR}/include/network/*.hpp")
file(GLOB_RECURSE include_converters_files "${CMAKE_SOURCE_DIR}/include/converters/*.hpp")
file(GLOB_RECURSE include_audio_files "${CMAKE_SOURCE_DIR}/include/audio/*.hpp")
file(GLOB_RECURSE include_utils_files "${CMAKE_SOURCE_DIR}/include/utils/*.hpp")
file(GLOB_RECURSE include_windows_files "${CMAKE_SOURCE_DIR}/include/windows/*.hpp")

file(GLOB_RECURSE source_base_files "${CMAKE_SOURCE_DIR}/source/base/*.cpp")
file(GLOB_RECURSE source_context_files "${CMAKE_SOURCE_DIR}/source/context/*.cpp")
file(GLOB_RECURSE source_graphics_files "${CMAKE_SOURCE_DIR}/source/graphics/*.cpp")
file(GLOB_RECURSE source_input_files "${CMAKE_SOURCE_DIR}/source/input/*.cpp")
file(GLOB_RECURSE source_network_files "${CMAKE_SOURCE_DIR}/source/network/*.cpp")
file(GLOB_RECURSE source_converters_files "${CMAKE_SOURCE_DIR}/source/converters/*.cpp")
file(GLOB_RECURSE source_audio_files "${CMAKE_SOURCE_DIR}/source/audio/*.cpp")
file(GLOB_RECURSE source_utils_files "${CMAKE_SOURCE_DIR}/source/utils/*.cpp")
file(GLOB_RECURSE source_physics_files "${CMAKE_SOURCE_DIR}/source/physics/*.cpp")
file(GLOB_RECURSE source_windows_files "${CMAKE_SOURCE_DIR}/source/windows/*.cpp")

list(REMOVE_ITEM include_network_files 
"${CMAKE_SOURCE_DIR}/include/network/asyncserver.hpp" 
"${CMAKE_SOURCE_DIR}/include/network/clientconnection.hpp" 
"${CMAKE_SOURCE_DIR}/include/network/clienthandler.hpp" 
)
list(REMOVE_ITEM source_network_files 
"${CMAKE_SOURCE_DIR}/source/network/clientconnection.cpp" 
"${CMAKE_SOURCE_DIR}/source/network/clientconnection.cpp" 
"${CMAKE_SOURCE_DIR}/source/network/clienthandler.cpp" 
"${CMAKE_SOURCE_DIR}/source/network/asyncserver.cpp")

include_directories(${CMAKE_SOURCE_DIR} "include" "boost-1.58.0" "openal-1.1" "glew-1.11.0/include")
add_definitions(-DGLEW_STATIC=1)
add_library(
besdkClientX64
STATIC 
${include_base_files} 
${include_context_files} 
${include_graphics_files}
${include_input_files}
${include_network_files}
${include_converters_files}
${include_audio_files}
${include_utils_files}
${include_windows_files}
${source_base_files}
${source_context_files}
${source_graphics_files}
${source_input_files}
${source_network_files}
${source_converters_files}
${source_audio_files}
${source_utils_files}
${source_physics_files}
${source_windows_files}
)
set_target_properties(besdkClientX64 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")


SOURCE_GROUP("include_base" FILES ${include_base_files})
SOURCE_GROUP("include_context" FILES ${include_context_files})
SOURCE_GROUP("include_graphics" FILES ${include_graphics_files})
SOURCE_GROUP("include_input" FILES ${include_input_files})
SOURCE_GROUP("include_network" FILES ${include_network_files})
SOURCE_GROUP("include_converters" FILES ${include_converters_files})
SOURCE_GROUP("include_audio" FILES ${include_audio_files})
SOURCE_GROUP("include_utils" FILES ${include_utils_files})
SOURCE_GROUP("include_windows" FILES ${include_windows_files})

SOURCE_GROUP("source_base" FILES ${source_base_files})
SOURCE_GROUP("source_context" FILES ${source_context_files})
SOURCE_GROUP("source_graphics" FILES ${source_graphics_files})
SOURCE_GROUP("source_input" FILES ${source_input_files})
SOURCE_GROUP("source_network" FILES ${source_network_files})
SOURCE_GROUP("source_converters" FILES ${source_converters_files})
SOURCE_GROUP("source_audio" FILES ${source_audio_files})
SOURCE_GROUP("source_utils" FILES ${source_utils_files})
SOURCE_GROUP("source_physics" FILES ${source_physics_files})
SOURCE_GROUP("source_windows" FILES ${source_windows_files})








project (brightServer)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

message(STATUS "")
message(STATUS "")
message(STATUS "::::::::  Creating Bright Server Solution  ::::::::")
message(STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
message(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY: " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

file(GLOB_RECURSE include_ai_files "${CMAKE_SOURCE_DIR}/include/ai/*.hpp")
file(GLOB_RECURSE include_base_files 
"${CMAKE_SOURCE_DIR}/include/base/controlactor.hpp"
"${CMAKE_SOURCE_DIR}/include/base/actorcontrolsresourcemanager.hpp"
"${CMAKE_SOURCE_DIR}/include/base/actorcontrolcontroller.hpp"
)
file(GLOB_RECURSE include_physics_files "${CMAKE_SOURCE_DIR}/include/physics/*.hpp")
file(GLOB_RECURSE include_network_files 
"${CMAKE_SOURCE_DIR}/include/network/asyncserver.hpp"
"${CMAKE_SOURCE_DIR}/include/network/globalstructs.hpp"
"${CMAKE_SOURCE_DIR}/include/network/networkmessage.hpp"
"${CMAKE_SOURCE_DIR}/include/network/clientconnection.hpp"
"${CMAKE_SOURCE_DIR}/include/network/clienthandler.hpp"
"${CMAKE_SOURCE_DIR}/include/network/loginmessage.hpp"
"${CMAKE_SOURCE_DIR}/include/network/loginresponsemessage.hpp"
"${CMAKE_SOURCE_DIR}/include/network/loginresponsemessage.hpp"
"${CMAKE_SOURCE_DIR}/include/network/updatemessage.hpp"
)
file(GLOB_RECURSE include_input_files 
"${CMAKE_SOURCE_DIR}/include/input/globalstructs.hpp"
"${CMAKE_SOURCE_DIR}/include/input/commandmessage.hpp"
)
file(GLOB_RECURSE include_converters_files "${CMAKE_SOURCE_DIR}/include/converters/aabbconverter.hpp")
file(GLOB_RECURSE include_utils_files "${CMAKE_SOURCE_DIR}/include/utils/*.hpp")

file(GLOB_RECURSE source_ai_files "${CMAKE_SOURCE_DIR}/source/ai/*.cpp")
file(GLOB_RECURSE source_base_files 
"${CMAKE_SOURCE_DIR}/source/base/controlactor.cpp"
"${CMAKE_SOURCE_DIR}/source/base/actorcontrolsresourcemanager.cpp"
"${CMAKE_SOURCE_DIR}/source/base/actorcontrolcontroller.cpp"
)
file(GLOB_RECURSE source_physics_files "${CMAKE_SOURCE_DIR}/source/physics/*.cpp")
file(GLOB_RECURSE source_network_files 
"${CMAKE_SOURCE_DIR}/source/network/asyncserver.cpp"
"${CMAKE_SOURCE_DIR}/source/network/networkmessage.cpp"
"${CMAKE_SOURCE_DIR}/source/network/clientconnection.cpp"
"${CMAKE_SOURCE_DIR}/source/network/clienthandler.cpp"
"${CMAKE_SOURCE_DIR}/source/network/loginmessage.cpp"
"${CMAKE_SOURCE_DIR}/source/network/loginresponsemessage.cpp"
"${CMAKE_SOURCE_DIR}/source/network/updatemessage.cpp"
)

file(GLOB_RECURSE source_input_files "${CMAKE_SOURCE_DIR}/source/input/commandmessage.cpp")
file(GLOB_RECURSE source_converters_files "${CMAKE_SOURCE_DIR}/source/converters/aabbconverter.cpp")
file(GLOB_RECURSE source_utils_files "${CMAKE_SOURCE_DIR}/source/utils/*.cpp")

include_directories(${CMAKE_SOURCE_DIR} "include" "boost-1.58.0")

add_library(
besdkServerX64
STATIC 
${include_ai_files}
${include_base_files}
${include_physics_files}
${include_network_files}
${include_input_files}
${include_converters_files}
${include_utils_files}
${source_ai_files}
${source_base_files}
${source_physics_files}
${source_network_files}
${source_input_files}
${source_converters_files}
${source_utils_files}
)
set_target_properties(besdkServerX64 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")

SOURCE_GROUP("include_ai" FILES ${include_ai_files})
SOURCE_GROUP("include_base" FILES ${include_base_files})
SOURCE_GROUP("include_physics" FILES ${include_physics_files})
SOURCE_GROUP("include_network" FILES ${include_network_files})
SOURCE_GROUP("include_input" FILES ${include_input_files})
SOURCE_GROUP("include_converters" FILES ${include_converters_files})
SOURCE_GROUP("include_utils" FILES ${include_utils_files})
SOURCE_GROUP("source_ai" FILES ${source_ai_files})
SOURCE_GROUP("source_base" FILES ${source_base_files})
SOURCE_GROUP("source_physics" FILES ${source_physics_files})
SOURCE_GROUP("source_network" FILES ${source_network_files})
SOURCE_GROUP("source_input" FILES ${source_input_files})
SOURCE_GROUP("source_converters" FILES ${source_converters_files})
SOURCE_GROUP("source_utils" FILES ${source_utils_files})




add_subdirectory ("${CMAKE_SOURCE_DIR}/test")