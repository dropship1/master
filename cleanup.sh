#!/bin/bash

find_and_remove_files () {
  find . -type f -name $1 -exec rm "{}" \+
}

find_and_print_files () {
  echo "## Removing all $1 files ##"
  echo ""
  find . -type f -name $1 -print
  echo ""
  echo "## ##"
  echo ""
}

find_and_do_all_for_files () {
  find_and_print_files $1
  find_and_remove_files $1
}


find_and_remove_directories () {
  find . -type d -name "$1" -exec rm -rf '{}' \;
}

find_and_print_directories () {
  echo "## Removing all $1 directories ##"
  echo ""
  find . -type d -name "$1" -print
  echo ""
  echo "## ##"
  echo ""
}

find_and_do_all_for_directories () {
  find_and_print_directories "$1"
  find_and_remove_directories "$1"
}


find_and_do_all_for_files '*.vcxproj.filters'
find_and_do_all_for_files '*.vcxproj'
find_and_do_all_for_files '*.sln'
find_and_do_all_for_files '*.sdf'
find_and_do_all_for_files '*.suo'
find_and_do_all_for_files '*.vcxproj.user'
find_and_do_all_for_files 'cmake_install.cmake'
find_and_do_all_for_files 'CMakeCache.txt'
find_and_do_all_for_directories '*.dir'
find_and_do_all_for_directories '*.vs'
find_and_do_all_for_directories 'CMakeFiles'
