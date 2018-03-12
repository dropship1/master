function(create_test_project projectName subdirName sourceFileslist includes type libraries)

  set(projectFullName "brightTest${projectName}")
  project(${projectFullName})
  message(STATUS "")
  message(STATUS "")
  message(STATUS "::::::::  Creating ${projectFullName} Solution  ::::::::")
  string(TOLOWER ${projectName} lcProjectName)

  message("projectName: ${projectName}")
  message("subdirName: ${subdirName}")
  message("sourceFileslist: ${sourceFileslist}")
  message("includes: ${includes}")
  message("type: ${type}")
  message("libraries: ${libraries}")

  set(source_files_list_name source_${lcProjectName}_files)
  file(GLOB_RECURSE  sourceFiles ${sourceFileslist})
  
  message("${source_files_list_name}")

  include_directories(${includes})

  set(target "Test_${subdirName}_${projectName}")
  message("target: ${target}")
  if(${type} MATCHES "executableWIN32")
    add_executable("${target}" WIN32 ${sourceFiles})
  elseif(${type} MATCHES "executable")
    add_executable("${target}" ${sourceFiles})
  endif()

  if ("besdkServer" IN_LIST libraries)
    target_link_libraries(${target} debug "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/besdkServerX64.lib")
    target_link_libraries(${target} optimized "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release/besdkServerX64.lib")
  elseif ("besdkClient" IN_LIST libraries)
    target_link_libraries(${target} debug "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/besdkClientX64.lib")
    target_link_libraries(${target} optimized "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release/besdkClientX64.lib")
  endif()


  #If the test project needs to use glew
  if ("glew" IN_LIST libraries)
    target_link_libraries(${target} debug "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/vendor/glew/Debug/x64/glew32sd.lib")
    target_link_libraries(${target} optimized "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/vendor/glew/Release/x64/glew32s.lib")
  endif()

  #If the test project needs to use opengl
  if ("opengl" IN_LIST libraries)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${target} ${OPENGL_LIBRARIES})
  endif()

  #If the test project needs to use boost
  if ("boost" IN_LIST libraries)
    message(STATUS "Adding boost libraries: ")
    message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
    target_link_libraries(${target} debug "${CMAKE_SOURCE_DIR}/boost_1_66_0/lib64-msvc-14.1/libboost_system-vc141-mt-gd-x64-1_66.lib")
    target_link_libraries(${target} debug "${CMAKE_SOURCE_DIR}/boost_1_66_0/lib64-msvc-14.1/libboost_date_time-vc141-mt-gd-x64-1_66.lib")
    target_link_libraries(${target} debug "${CMAKE_SOURCE_DIR}/boost_1_66_0/lib64-msvc-14.1/libboost_regex-vc141-mt-gd-x64-1_66.lib")
    target_link_libraries(${target} debug "${CMAKE_SOURCE_DIR}/boost_1_66_0/lib64-msvc-14.1/libboost_thread-vc141-mt-gd-x64-1_66.lib")
    target_link_libraries(${target} debug "${CMAKE_SOURCE_DIR}/boost_1_66_0/lib64-msvc-14.1/libboost_chrono-vc141-mt-gd-x64-1_66.lib")
    target_link_libraries(${target} optimized "${CMAKE_SOURCE_DIR}/boost_1_66_0/lib64-msvc-14.1/libboost_system-vc141-mt-x64-1_66.lib")
    target_link_libraries(${target} optimized "${CMAKE_SOURCE_DIR}/boost_1_66_0/lib64-msvc-14.1/libboost_date_time-vc141-mt-x64-1_66.lib")
    target_link_libraries(${target} optimized "${CMAKE_SOURCE_DIR}/boost_1_66_0/lib64-msvc-14.1/libboost_regex-vc141-mt-x64-1_66.lib")
    target_link_libraries(${target} optimized "${CMAKE_SOURCE_DIR}/boost_1_66_0/lib64-msvc-14.1/libboost_thread-vc141-mt-x64-1_66.lib")
    target_link_libraries(${target} optimized "${CMAKE_SOURCE_DIR}/boost_1_66_0/lib64-msvc-14.1/libboost_chrono-vc141-mt-x64-1_66.lib")
  endif()


  #If the test project needs to use openal
  if ("openal" IN_LIST libraries)
    target_link_libraries(${target} "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/vendor/OpenAL/Win64/OpenAL32.lib")
    target_link_libraries(${target} "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/vendor/OpenAL/Win64/EFX-Util_MT/EFX-Util.lib")
  endif()

  set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "bin")
  set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin/Debug")
  set_target_properties(${target} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "bin")

  SOURCE_GROUP(${source_files_list_name} FILES ${sourceFiles})

endfunction(create_test_project)