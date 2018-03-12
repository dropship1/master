
cmake_minimum_required (VERSION 3.10.2)

create_test_project(
  "AsyncServer"
  "Network"
  "asyncserver.cpp"
  "${CMAKE_SOURCE_DIR}/include;${CMAKE_SOURCE_DIR}/boost_1_66_0"
  "executable" 
  "besdkServer;boost"
)


create_test_project(
  "ServerConnection"
  "Network"
  "serverconnection.cpp"
  "${CMAKE_SOURCE_DIR}/include;${CMAKE_SOURCE_DIR}/boost_1_66_0;${CMAKE_SOURCE_DIR}/glew-1.11.0/include"
  "executableWIN32" 
  "besdkClient;opengl;glew;boost"
)

#project (brightNetworkServerConnection)
#
#message(STATUS "Creating brightNetworkServerConnection Solution")
#
#file(GLOB_RECURSE source_network_serverconnection_files "serverconnection.cpp")
#
#message("${source_network_serverconnection_files}")
#
#include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/boost-1.58.0")
#
#add_executable(Test_Network_ServerConnection ${source_network_serverconnection_files})
#target_link_libraries(
#Test_Network_ServerConnection	 
#"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib"
#)
#
#set_target_properties(Test_Network_ServerConnection PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
#SOURCE_GROUP("source_network_serverconnection" FILES ${source_network_serverconnection_files})



#project (brightNetworkClientHandler)
#
#message(STATUS "Creating brightNetworkClientHandler Solution")
#
#file(GLOB_RECURSE source_network_clienthandler_files "clienthandler.cpp")
#
#message("${source_network_clienthandler_files}")
#
#include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")
#
#add_executable(Test_Network_ClientHandler ${source_network_clienthandler_files})
#target_link_libraries(Test_Network_ClientHandler "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkServerD.lib")
#
#set_target_properties(Test_Network_ClientHandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
#set_target_properties(Test_Network_ClientHandler PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
#SOURCE_GROUP("source_network_clienthandler" FILES ${source_network_clienthandler_files})
#
#
#project (brightNetworkServerHandler)
#
#message(STATUS "Creating brightNetworkServerHandler Solution")
#
#file(GLOB_RECURSE source_network_serverhandler_files "serverhandler.cpp")
#
#message("${source_network_serverhandler_files}")
#
#include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")
#
#add_executable(Test_Network_ServerHandler ${source_network_serverhandler_files})
#target_link_libraries(Test_Network_ServerHandler "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib")
#
#set_target_properties(Test_Network_ServerHandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
#set_target_properties(Test_Network_ServerHandler PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")
#SOURCE_GROUP("source_network_serverhandler" FILES ${source_network_serverhandler_files})


file (COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file (COPY "${CMAKE_SOURCE_DIR}/data/shaders" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)
file (COPY "${CMAKE_SOURCE_DIR}/data/textures" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)
file (COPY "${CMAKE_SOURCE_DIR}/data/meshes" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/data)