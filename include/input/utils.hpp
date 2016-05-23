#ifndef BRIGHT_INPUT_UTILS_H
#define BRIGHT_INPUT_UTILS_H


#include <Windows.h>
#include <map>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <memory>

#include "input/globalstructs.hpp"
#include <vector>

namespace bright{

  namespace input{

  std::vector<std::shared_ptr<InputBindings>> parse_input_bindings(std::string fileName);

  std::string translate_key(short int scancode);
  void create_maps(std::map<int, std::string> &keyIndexToValue);
  std::string get_key_ascii_value(int valueToTranslate);

  }
}

#endif