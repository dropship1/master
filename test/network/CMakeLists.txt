
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX D)
endif()


cmake_minimum_required (VERSION 3.10.2)
project (brightNetworkAsyncServer)

message(STATUS "Creating brightNetworkAsyncServer Solution")

file(GLOB_RECURSE source_network_asyncserver_files "asyncserver.cpp")

message("${source_network_asyncserver_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/boost-1.58.0")

add_executable(Test_Network_AsyncServer ${source_network_asyncserver_files})
target_link_libraries(
Test_Network_AsyncServer	 
"${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkServerD.lib"
"${CMAKE_SOURCE_DIR}/boost-1.58.0/stage/lib/libboost_system-vc140-mt-gd-1_58.lib"
"${CMAKE_SOURCE_DIR}/boost-1.58.0/stage/lib/libboost_date_time-vc140-mt-gd-1_58.lib"
"${CMAKE_SOURCE_DIR}/boost-1.58.0/stage/lib/libboost_regex-vc140-mt-gd-1_58.lib"
"${CMAKE_SOURCE_DIR}/boost-1.58.0/stage/lib/libboost_thread-vc140-mt-gd-1_58.lib"
"${CMAKE_SOURCE_DIR}/boost-1.58.0/stage/lib/libboost_chrono-vc140-mt-gd-1_58.lib"
)

set_target_properties(Test_Network_AsyncServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
SOURCE_GROUP("source_network_asyncserver" FILES ${source_network_asyncserver_files})



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



project (brightNetworkClientHandler)

message(STATUS "Creating brightNetworkClientHandler Solution")

file(GLOB_RECURSE source_network_clienthandler_files "clienthandler.cpp")

message("${source_network_clienthandler_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")

add_executable(Test_Network_ClientHandler ${source_network_clienthandler_files})
target_link_libraries(Test_Network_ClientHandler "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkServerD.lib")

set_target_properties(Test_Network_ClientHandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
SOURCE_GROUP("source_network_clienthandler" FILES ${source_network_clienthandler_files})


project (brightNetworkServerHandler)

message(STATUS "Creating brightNetworkServerHandler Solution")

file(GLOB_RECURSE source_network_serverhandler_files "serverhandler.cpp")

message("${source_network_serverhandler_files}")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include")

add_executable(Test_Network_ServerHandler ${source_network_serverhandler_files})
target_link_libraries(Test_Network_ServerHandler "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}/besdkClientD.lib")

set_target_properties(Test_Network_ServerHandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin")
SOURCE_GROUP("source_network_serverhandler" FILES ${source_network_serverhandler_files})

