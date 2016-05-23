#include "utils/fileworker.hpp"

using namespace bright::utils;


FileWorker::FileWorker(std::string fullPathAndNameOfConfigFile): fullPathAndNameOfConfigFile_(fullPathAndNameOfConfigFile){
}


//void FileWorker::read_in_global_binary_file_and_create_map(std::string fullPathAndName){
//
//  std::fstream in(fullPathAndName, std::ios::in | std::ios::binary);
//  if (!in) { 
//    std::cerr << "Bin file reader: Cannot open " << fullPathAndName << std::endl << std::flush; 
//    exit(1); 
//  }
//
//  std::string allFilesContent_ = from_stream(in);
//}
//
//
//void FileWorker::dump_all_files_data_to_global_binary(std::string fullPathAndName){
//
//  std::string data;
//
//  auto append_file_data_to_all_files_data = [&] (std::string fullPathAndFileName){
//    std::fstream in(fullPathAndFileName, std::ios::in);
//    if (!in) { 
//      std::cerr << "Bin file reader: Cannot open " << fullPathAndFileName << std::endl << std::flush; 
//      exit(1); 
//    }
//    
//    std::string fileContents = from_stream(in);
//    
//    fileContents = "START 1234567890"+fullPathAndFileName+fileContents+"END 1234567890"+fullPathAndFileName;
//    data += fileContents;
//  };
//  std::for_each(fullPathsAndfileNames_.begin(), fullPathsAndfileNames_.end(), append_file_data_to_all_files_data);
//
//
//  std::fstream output(fullPathAndName, std::ios::out | std::ios::trunc | std::ios::binary);
//  //std::fstream output(fullPathAndName, std::ios::out | std::ios::trunc);
//  to_stream(output, data);
//}


void FileWorker::to_stream(std::ostream &out, std::string outputData){
  out << outputData << std::endl;
}
 

std::string FileWorker::from_stream(std::fstream &in){
  std::string inputData;
  //std::string line; 
  //while (getline(in, line)){
  //  inputData += line + "\n";
  //}
  std::stringstream sstr;
  sstr << in.rdbuf();
  //inputData = inputData.substr(0, inputData.size()-1);
  inputData = sstr.str();
  return inputData;
}

std::vector<std::shared_ptr<std::istringstream>> FileWorker::get_files_streams(std::vector<std::string> files){

  std::vector<std::shared_ptr<std::istringstream>> streams;
  auto add_stream = [&] (std::string fileName){
    std::string contents = get_file_contents(fileName);
    std::shared_ptr<std::istringstream> stream = std::make_shared<std::istringstream>(contents);
    streams.push_back(stream);
  };
  std::for_each(files.begin(), files.end(), add_stream);

  return streams;
} 


void FileWorker::create_all_files_map(){

  auto find_file_data_and_save_to_map = [&] (std::pair<std::string, std::string> namePair){
    std::string shortName = namePair.first;
    std::string fileName = namePair.second;
    std::string startOfFileMarking("START 1234567890"+shortName);
    std::string endOfFileMarking("END 1234567890"+shortName);

    std::size_t startOfFileContents = allFilesContent_.find(startOfFileMarking) + startOfFileMarking.size();
    std::size_t endOfFileContents = allFilesContent_.find(endOfFileMarking);
    std::size_t sizeToTake = endOfFileContents - startOfFileContents;

    std::string fileData = allFilesContent_.substr(startOfFileContents, sizeToTake);
    allFilesContents_[shortName] = fileData;
  };
  std::for_each(fullPathsAndfileNames_.begin(), fullPathsAndfileNames_.end(), find_file_data_and_save_to_map);
  
}


std::string FileWorker::get_file_contents(std::string name){
  return allFilesContents_[name];
}

std::shared_ptr<std::stringstream> FileWorker::get_file_stringstream(std::string name){
  return allStringStreams_[name];
}


void FileWorker::add_file(std::string fullPathAndFileName){
  fullPathsAndfileNames_[fullPathAndFileName] = fullPathAndFileName;
}


void FileWorker::add_file(std::string fullPathAndFileName, std::string shortName){
  fullPathsAndfileNames_[shortName] = fullPathAndFileName;
}


void FileWorker::create_lookup_map_of_files_content(){

  auto find_file_data_and_save_to_map = [&] (std::pair<std::string, std::string> namePair){
    std::string alias = namePair.first;
    std::string fullPathAndFileName = namePair.second;
    std::ifstream in(fullPathAndFileName, std::ios::binary);
    if (!in) { 
      std::cerr << "FileWorker read_in_files_and_create_map: Cannot open " << fullPathAndFileName << std::endl << std::flush; 
      exit(1); 
    }

    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string fileData = buffer.str();//from_stream(in);
    in.close();
    allFilesContents_[alias] = fileData;
  };
  std::for_each(fullPathsAndfileNames_.begin(), fullPathsAndfileNames_.end(), find_file_data_and_save_to_map);
  

}


void FileWorker::config_file_path_and_name(std::string fullPathAndNameOfConfigFile){
  fullPathAndNameOfConfigFile_ = fullPathAndNameOfConfigFile;
}


void FileWorker::read_in_list_of_files(){

  fullPathsAndfileNames_.clear();

  std::ifstream in(fullPathAndNameOfConfigFile_, std::ios::in);
  if (!in) {
    std::cerr << "FileWorker read_in_list_of_files: Cannot open " << fullPathAndNameOfConfigFile_ << std::endl << std::flush; 
    exit(1);
  }

  std::string path;
  std::string alias;
  bool haveAlias = false;
  bool havePath = false;
  std::string line;
  while (getline(in, line)){
    if(line.substr(0,5) == "path="){
      path = line.substr(5);
      havePath = true;
    }
    else if(havePath){
      if(line.substr(0,6) == "alias="){
        haveAlias = true;
        alias = line.substr(6);
      }
      else if(line.substr(0,5) == "file="){
        std::string fileName = line.substr(5);
        std::string fullPathAndFileName = path+line.substr(5);
        if(!haveAlias){
          alias = fileName;
        }
        haveAlias = false;
        fullPathsAndfileNames_[alias] = fullPathAndFileName;
      }
    }
  }

}


