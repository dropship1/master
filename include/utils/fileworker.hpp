#ifndef BRIGHT_UTILS_FILE_WORKER_H
#define BRIGHT_UTILS_FILE_WORKER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>

namespace bright{

  namespace utils{

/**
 * @ingroup utils
 *
 * @brief The file worker class
 * 
 * @details 
 * This works with files (reads,writes,all different types)
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */
class FileWorker {

public:
  FileWorker(std::string fullPathAndNameOfConfigFile);
  
  //void read_in_global_binary_file_and_create_map(std::string fullPathAndName);
  //void dump_all_files_data_to_global_binary(std::string fullPathAndName);

  //void read_in_files_and_create_map();

  void read_in_list_of_files();
  void create_lookup_map_of_files_content();

  void config_file_path_and_name(std::string fullPathAndNameOfConfigFile);
  std::string get_file_contents(std::string name);
  std::vector<std::shared_ptr<std::istringstream>> FileWorker::get_files_streams(std::vector<std::string> files);
  void add_file(std::string fullPathAndFileName);
  void add_file(std::string fullPathAndFileName, std::string shortName);
  std::shared_ptr<std::stringstream> get_file_stringstream(std::string name);


private:
  void to_stream(std::ostream &out, std::string outputData);
  std::string from_stream(std::fstream &in);

  void create_all_files_map();

  std::string allFilesContent_;
  std::map<std::string, std::shared_ptr<std::istringstream>> allIStringStream_;
  std::map<std::string, std::shared_ptr<std::stringstream>> allStringStreams_;
  std::string fullPathAndNameOfConfigFile_;
  std::map<std::string, std::string> allFilesContents_;
  std::map<std::string, std::string> fullPathsAndfileNames_;


};

  }
}

#endif