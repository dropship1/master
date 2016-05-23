#include "input/utils.hpp"

namespace bright{

  namespace input{

std::vector<std::shared_ptr<InputBindings>> parse_input_bindings(std::string fileName){
  std::vector<std::shared_ptr<InputBindings>> inputBindings;

  //SInputBindings* pBinding;
  //std::string sLine = std::string();
  //std::string sCurrentDevice;
  //
  //std::ifstream File(sFileName.c_str()); 
  //if (File.is_open()) { 
  //  while (!File.eof()) { 
  //
  //  getline(File, sLine);
  //  std::string type = sLine.substr(0,1);
  //  
  //  if(type == "["){
  //    pBinding = new SInputBindings();
  //    std::string name = sLine.substr(1);
  //    pBinding->sActorName = name;
  //  }
  //  else if(type == "<"){
  //    sCurrentDevice = sLine.substr(1,sLine.length()-2);
  //  }
  //  else if (type == "]"){
  //    vcInputBindings.push_back(pBinding);
  //  }
  //  else if(type == "#"){}
  //  else{
  //    if (sCurrentDevice == "Keyboard"){
  //      std::string sName = sLine.substr(0,sLine.find("="));
  //      std::string sValue = sLine.substr(sLine.find("=")+1);
  //      pBinding->stKeyboard.mBindings[sName] = sValue;
  //    }
  //    //else if (sCurrentDevice == "Hydra"){
  //    //  std::string name = sLine.substr(0,sLine.find("="));
  //    //  std::string sValue = sLine.substr(sLine.find("=")+1);
  //    //  pBinding->HydraBindings.mBindings[name] = sValue;
  //    //}
  //    else if (sCurrentDevice == "RawMouse"){
  //      std::string sName = sLine.substr(0,sLine.find("="));
  //      std::string sValue = sLine.substr(sLine.find("=")+1);
  //      pBinding->stRawMouse.mBindings[sName] = sValue;
  //    }
  //  }
  //  }
  //      
  //  File.close(); 
  //}

  return inputBindings;
}

std::string translate_key(short int scancode){
  std::cout<< "HERECode:? " << scancode << std::endl << std::flush;
  static int mapsCreated = 0;
  static std::map<int, std::string> keyIndexToValue;
  if (mapsCreated == 0){
    create_maps(keyIndexToValue);
  }
  mapsCreated = 1;
  int index = scancode;
  std::cout<<  "HERECode2:? " << index << std::endl << std::flush;
  return keyIndexToValue[index];
}

void create_maps(std::map<int, std::string> &keyIndexToValue){
  for (int i = 0; i < 256; ++i){
    std::string asciiValue = get_key_ascii_value(i);
    keyIndexToValue[i] = asciiValue;
    //std::cout<<  i << " : " << asciiValue << std::endl << std::flush;
  }
}

std::string get_key_ascii_value(int valueToTranslate){
  std::string s1;
  s1 = (char)valueToTranslate;
  std::cout << s1 << std::endl << std::flush;
  //char c = static_cast<char>(valueToTranslate);
  //char *ct = c;
  ////char c = valueToTranslate;
  ////std::cout<<  c << std::endl << std::flush;
  ////std::stringstream ss;
  ////std::string s;
  ////ss << c;
  ////ss >> s;
  //std::string s(1, c);
  return s1;
}

  }

}




