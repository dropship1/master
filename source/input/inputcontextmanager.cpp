#include "input/inputcontextmanager.hpp"
#include "input/keyboardevent.hpp"
#include "input/keyboardeventlistener.hpp"
#include "utils/utilities.hpp"

using namespace bright::input;

void InputContextManager::add_command(std::string commandName, std::string commandType) {
  commands_.insert(std::pair<std::string, std::string>(commandName, commandType));
}
void InputContextManager::add_control(std::string controlName, std::string commandName) {
  std::map<std::string, std::string>::iterator itr = commands_.find(commandName);
  if ( itr == commands_.end() ) {
    //std::cout << "ERROR: Tried to add control: " << controlName << " for command: " << commandName << " that isn't already in commands map." << std::endl;
    return;
  }
  controls_.insert(std::pair<std::string, std::string>(controlName, commandName));
}

void InputContextManager::initialize(std::string controlsContexts) {
  std::istringstream f(controlsContexts);
  std::string line;
  std::string currentContext = "";
  while (std::getline(f, line)) {
    line = bright::utils::trim(line);
    if (line.length() == 0) {
      //Ignore empty lines
    }
    else if (line.substr(0,1) == "#") {
      //Ignore comment
    }
    else if ( bright::utils::split(line, "=").size() == 2 ) {
      if (line.length() == 0) {
        //std::cout << "ERROR: Currect Contxt not set. Unable to assign control assignment: " << std::endl;
      }
      else {
        std::vector<std::string> split_line = bright::utils::split(line, "=");
        std::string commandName = split_line.at(0);
        std::string commandType = split_line.at(1);
        bright::utils::trim(commandName);
        bright::utils::trim(commandType);
        //std::cout << "DEBUG: controlName: " << commandName << " control_type: " << commandType << std::endl;
        add_command(commandName, commandType);
      }
    }
    else {
      //std::cout << "ERROR: Incorrect format for config line: " << line << std::endl;
    }
  } //END while
}

void InputContextManager::set_control_config(std::string controlConfig) {
  std::istringstream f(controlConfig);
  std::string line;
  while (std::getline(f, line)) {
    bright::utils::trim(line);
    if (line.length() == 0) {
      //Ignore empty lines
    }
    else if (line.substr(0,1) == "#") {
      //Ignore comment
    }
    else if ( bright::utils::split(line, "=").size() == 2 ) {
      if (line.length() == 0) {
        //std::cout << "ERROR: Currect Contxt not set. Unable to assign control assignment: " << std::endl;
      }
      else {
        std::vector<std::string> split_line = bright::utils::split(line, "=");
        std::string controlName = split_line.at(0);
        std::string commandName = split_line.at(1);
        bright::utils::trim(controlName);
        bright::utils::trim(commandName);
        //std::cout << "DEBUG: controlName: " << controlName << " commandName: " << commandName << std::endl;
        add_control(controlName, commandName);
      }
    }
  } // END while
}

std::string InputContextManager::control_to_command(std::string controlName) {
  std::map<std::string, std::string>::iterator itr = controls_.find(controlName);
  if ( itr == controls_.end() ) {
    return "";
  }
  return itr->second;
}

std::string InputContextManager::command_to_type(std::string commandName) {
  std::map<std::string, std::string>::iterator itr = commands_.find(commandName);
  if ( itr == commands_.end() ) {
    return "";
  }
  return itr->second;
}

void bright::input::InputContextManager::on_keyboard_event(std::shared_ptr<bright::input::KeyboardEvent> pKeyboardInputEvent) {

  std::string controlName = pKeyboardInputEvent->key_string();
  std::string commandName = control_to_command(controlName);
  // Control is not assigned to a command
  if (commandName == "") {
    return;
  }
  std::string commandType = command_to_type(commandName);
  
  if (commandType == "KEY") {
    if (pKeyboardInputEvent->is_down()) {
      //std::cout << "controlName: " << controlName << " commandName: " << commandName << " commandType: " << commandType << std::endl;
      auto commandInfo = std::make_shared<std::vector<std::string>>();
      commandInfo->push_back(commandName);
      commandInfo->push_back(commandType);
      update_command_listeners(commandInfo);
    }
  }
  else if (commandType == "STATE") {
    std::string stateData = "STATE_OFF";
    if ( pKeyboardInputEvent->is_down() ) {
      stateData = "STATE_ON";
    }
    else if ( !pKeyboardInputEvent->is_down() ) {
      stateData = "STATE_OFF";
    }
    //std::cout << "controlName: " << controlName << " commandName: " << commandName << " commandType: " << commandType << "stateData: " << stateData << std::endl;
    auto commandInfo = std::make_shared<std::vector<std::string>>();
    commandInfo->push_back(commandName);
    commandInfo->push_back(commandType);
    commandInfo->push_back(stateData);
    update_command_listeners(commandInfo);
  }

  //std::cout << "Keyboard Listener:" << std::endl << std::flush;
  //std::cout << "keyString: " << pKeyboardInputEvent->key_string() << std::endl << std::flush;
  //std::cout << "keyEnum: " << (int)pKeyboardInputEvent->key_enum() << std::endl << std::flush;
  //std::cout << "upOrDown:? ";
  //if ( pKeyboardInputEvent->is_down() ){ 
  //  std::cout << "Down" << std::endl << std::flush; 
  //  if (pKeyboardInputEvent->key_enum() == Key::W){ 
  //    std::cout << "Come with me if you want to live!" << std::endl << std::flush;
  //  }
  //  //if (pKeyboardInputEvent->key_enum() == Key::F1){ 
  //  //  pContextManager->reshape_context(true,1920,1080);
  //  //}
  //  //else if (pKeyboardInputEvent->key_enum() == Key::F2){ 
  //  //  pContextManager->reshape_context(false,1280,768);
  //  //}

  //  //if (pKeyboardInputEvent->key_enum() == Key::F4){ 
  //  //  pContextManager->exit();
  //  //}
  //}
  //else {
  //  std::cout << "Up" << std::endl << std::flush;
  //  if (pKeyboardInputEvent->key_enum() == Key::W){ 
  //    std::cout << "Its not a tumor!" << std::endl << std::flush;
  //  }
  //}
  //std::cout << std::endl << std::flush;
}

//TODO: Move this to some kind of map
std::string InputContextManager::get_mouse_button_name(std::shared_ptr<bright::input::RawMouseEvent> pRawMouseInputEvent) {
  std::string buttonName = "";
  if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::MB_UNDEF ){ 
    buttonName = "MOUSE_UNDEF";
  }
  else if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::LEFT ){ 
    buttonName = "MOUSE_LEFT";
  }
  else if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::RIGHT ){ 
    buttonName = "MOUSE_RIGHT";
  }
  else if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::MIDDLE ){ 
    buttonName = "MOUSE_MIDDLE";
  }
  else if ( pRawMouseInputEvent->delta_rate_wheel() < 0 ) {
    buttonName = "MOUSE_WHEEL_DOWN"; 
  }
  else if ( pRawMouseInputEvent->delta_rate_wheel() > 0 ) {
    buttonName = "MOUSE_WHEEL_UP"; 
  }
  return buttonName;
}


void InputContextManager::on_raw_mouse_event(std::shared_ptr<bright::input::RawMouseEvent> pRawMouseInputEvent){

  if ( pRawMouseInputEvent->raw_mouse_event_type() == bright::input::MouseEventType::BUTTON_CLICK ){
    std::string controlName = get_mouse_button_name(pRawMouseInputEvent);
    std::string commandName = control_to_command(controlName);
    if (commandName != "") {
      std::string commandType = command_to_type(commandName);
      if (commandType == "KEY" ) {
        if ( pRawMouseInputEvent->is_raw_mouse_button_down() ) {
          //std::cout << "controlName: " << controlName << " commandName: " << commandName << " commandType: " << commandType << std::endl;
          auto commandInfo = std::make_shared<std::vector<std::string>>();
          commandInfo->push_back(commandName);
          commandInfo->push_back(commandType);
          update_command_listeners(commandInfo);
        }
      }
      else if (commandType == "STATE") {
        std::string stateData = "STATE_OFF";
        if ( pRawMouseInputEvent->is_raw_mouse_button_down() ) {
          stateData = "STATE_ON";
        }
        //std::cout << "controlName: " << controlName << " commandName: " << commandName << " commandType: " << commandType << "stateData: " << stateData << std::endl;
        auto commandInfo = std::make_shared<std::vector<std::string>>();
        commandInfo->push_back(commandName);
        commandInfo->push_back(commandType);
        commandInfo->push_back(stateData);
        update_command_listeners(commandInfo);
      }
    }
  }

  //std::cout << "RawMouse Listener:" << std::endl << std::flush;
  //std::cout << "type: ";
  //if ( pRawMouseInputEvent->raw_mouse_event_type() == bright::input::MouseEventType::BUTTON_CLICK){ 
    //std::cout << "1 : BUTTON_CLICK" << std::endl << std::flush; 
   // std::cout << "button: ";
    //if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::MB_UNDEF){ std::cout << "0 : MB_UNDEF" << std::endl << std::flush; }
    ////if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::LEFT){ std::cout << "1 : LEFT" << std::endl << std::flush; }
    //if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::RIGHT){ std::cout << "2 : RIGHT" << std::endl << std::flush; }
    //if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::MIDDLE){ std::cout << "3 : MIDDLE" << std::endl << std::flush; }
    //if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::WHEEL){ std::cout << "4 : WHEEL" << std::endl << std::flush; }
    //std::cout << "upOrDown?: ";
   // if ( pRawMouseInputEvent->is_raw_mouse_button_down() ){ 
    //std::cout << "Down" << std::endl << std::flush; 
  //}
   // else { 
    //std::cout << "Up" << std::endl << std::flush; 
  //}
   // std::cout << "deltaWheel: " << pRawMouseInputEvent->delta_rate_wheel() << std::endl << std::flush;
 // }


  if ( pRawMouseInputEvent->raw_mouse_event_type() == bright::input::MouseEventType::MOVEMENT){ 
    std::string controlName = "";
    int deltaX = pRawMouseInputEvent->delta_rate_x();
    int deltaY = pRawMouseInputEvent->delta_rate_y();
      //std::cout << "2 : MOVEMENT" << std::endl << std::flush; 
      //std::cout << "deltaX: " << deltaX << std::endl << std::flush;
      //std::cout << "deltaY: " << deltaY << std::endl << std::flush;

    if (deltaX != 0) {
      //TODO: refactor duplicate code
      controlName = "MOUSE_X_AXIS";
      std::string commandName = control_to_command(controlName);
      // If control is assigned to control, send it out
      if (commandName != "") {
        std::string command_type = command_to_type(commandName);
          //std::cout << "controlName: " << controlName << " commandName: " << commandName << " command_type: " << command_type << std::endl;
        auto commandInfo = std::make_shared<std::vector<std::string>>();
        commandInfo->push_back(commandName);
        commandInfo->push_back(command_type);
        commandInfo->push_back(std::to_string(deltaX));
        update_command_listeners(commandInfo);
      }
    }
    if (deltaY != 0) {
      //TODO: refactor duplicate code
      controlName = "MOUSE_Y_AXIS";
      std::string commandName = control_to_command(controlName);
      // If control is assigned to control, send it out
      if (commandName != "") {
        std::string command_type = command_to_type(commandName);
        //std::cout << "controlName: " << controlName << " commandName: " << commandName << " command_type: " << command_type << std::endl;
        auto commandInfo = std::make_shared<std::vector<std::string>>();
        commandInfo->push_back(commandName);
        commandInfo->push_back(command_type);
        commandInfo->push_back(std::to_string(deltaY));
        update_command_listeners(commandInfo);
      }
    }
  }
 //std::cout << std::endl << std::flush;
}

void InputContextManager::on_mouse_event(std::shared_ptr<bright::input::MouseEvent> pMouseInputEvent){

  //std::cout << "Mouse Listener:" << std::endl << std::flush;
  //std::cout << "xPos: " << pMouseInputEvent->x_pos() << std::endl << std::flush;
  //std::cout << "yPos: " << pMouseInputEvent->y_pos() << std::endl << std::flush;
  //std::cout << std::endl << std::flush;

}

void InputContextManager::add_command_event_listener(boost::shared_ptr<CommandEventListener> pCommandEventListener){
  pCommandEventListeners_.push_back(pCommandEventListener);
}

void InputContextManager::notify(){
  if ( !commandEvents_.empty() ){
    notify_command_listeners();
  }
}

void InputContextManager::notify_command_listeners(){

  boost::shared_ptr<CommandEventListener> pCurrentCommandEventListener;

  auto notify = [&] (std::shared_ptr<CommandEvent> pCommandEvent) { 
    pCurrentCommandEventListener->on_command_event(pCommandEvent);
  };

  auto notify_command_listener = [&] (boost::shared_ptr<CommandEventListener> pCommandEventListener) {
    pCurrentCommandEventListener = pCommandEventListener;
    std::for_each(this->commandEvents_.begin(), this->commandEvents_.end(), notify);
  };
  std::for_each(pCommandEventListeners_.begin(), pCommandEventListeners_.end(), notify_command_listener);

  commandEvents_.clear();
}

void InputContextManager::update_command_listeners(std::shared_ptr<std::vector<std::string>>  commandInfo) {
  auto commandEvent = std::make_shared<CommandEvent>(commandInfo);
  commandEvents_.push_back(commandEvent);
}