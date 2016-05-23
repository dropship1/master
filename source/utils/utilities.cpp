#include "utils/utilities.hpp"

namespace bright{

  namespace utils{

std::vector<std::string> split(std::string input, std::string regexstr) {
  
  trim(input);
  
  std::regex regex(regexstr);
  std::sregex_token_iterator regexit = std::sregex_token_iterator(input.begin(), input.end(), regex, -1);
  std::sregex_token_iterator endit = std::sregex_token_iterator();

  std::vector<std::string> splitdata;

  std::copy( regexit, endit, back_inserter(splitdata) ); 

  return splitdata;
}


std::string& ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}


std::string& rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}


std::string& trim(std::string &s) {
  return ltrim(rtrim(s));
}


  }

}




