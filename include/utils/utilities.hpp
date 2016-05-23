#ifndef BRIGHT_UTILS_UTILITIES_H
#define BRIGHT_UTILS_UTILITIES_H

#include <map>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <memory>
#include <regex>
#include <functional>
#include <cctype>

#include <vector>

namespace bright{

  namespace utils{

  std::vector<std::string> split(std::string input, std::string regex); 
  std::string &ltrim(std::string &s);
  std::string &rtrim(std::string &s);
  std::string &trim(std::string &s);

  }
}

#endif