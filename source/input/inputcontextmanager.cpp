#include "input/inputcontextmanager.hpp"
#include "input/keyboardevent.hpp"
#include "input/keyboardeventlistener.hpp"
#include "utils/utilities.hpp"

using namespace bright::input;

void InputContextManager::add_command(std::string command_name, std::string command_type) {
	commands_.insert(std::pair<std::string, std::string>(command_name, command_type));
}
void InputContextManager::add_control(std::string control_name, std::string command_name) {
	std::map<std::string, std::string>::iterator itr = commands_.find(command_name);
	if (itr == commands_.end()) {
		std::cout << "ERROR: Tried to add control: " << control_name << " for command: " << command_name << " that isn't already in commands map." << std::endl;
		return;
	}
	controls_.insert(std::pair<std::string, std::string>(control_name, command_name));
}

void InputContextManager::initialize(std::string controls_contexts) {
	std::istringstream f(controls_contexts);
	std::string line;
	std::string current_context = "";
	while (std::getline(f, line)) {
		line = bright::utils::trim(line);
		if (line.length() == 0) {
			// Ignore empty lines
		}
		else if (line.substr(0,1) == "#") {
			// Ignore comment
		}
		else if ( bright::utils::split(line, "=").size() == 2 ) {
			if (line.length() == 0) {
				std::cout << "ERROR: Currect Contxt not set. Unable to assign control assignment: " << std::endl;
			}
			else {
				std::vector<std::string> split_line = bright::utils::split(line, "=");
				std::string command_name = split_line.at(0);
				std::string command_type = split_line.at(1);
				bright::utils::trim(command_name);
				bright::utils::trim(command_type);

				std::cout << "DEBUG: control_name: " << command_name << " control_type: " << command_type << std::endl;
				add_command(command_name, command_type);
			}
		}
		else {
			std::cout << "ERROR: Incorrect format for config line: " << line << std::endl;
		}
	} // END while
}

void InputContextManager::set_control_config(std::string control_config) {
	std::istringstream f(control_config);
	std::string line;
	while (std::getline(f, line)) {
		bright::utils::trim(line);
		if (line.length() == 0) {
			// Ignore empty lines
		}
		else if (line.substr(0,1) == "#") {
			// Ignore comment
		}
		else if ( bright::utils::split(line, "=").size() == 2 ) {
			if (line.length() == 0) {
				std::cout << "ERROR: Currect Contxt not set. Unable to assign control assignment: " << std::endl;
			}
			else {
				std::vector<std::string> split_line = bright::utils::split(line, "=");
				std::string control_name = split_line.at(0);
				std::string command_name = split_line.at(1);
				bright::utils::trim(control_name);
				bright::utils::trim(command_name);

				std::cout << "DEBUG: control_name: " << control_name << " command_name: " << command_name << std::endl;
				add_control(control_name, command_name);
			}
		}
	} // END while
}

std::string InputContextManager::control_to_command(std::string control_name) {
	std::map<std::string, std::string>::iterator itr = controls_.find(control_name);
	if (itr == controls_.end()) {
		return "";
	}
	return itr->second;
}

std::string InputContextManager::command_to_type(std::string command_name) {
	std::map<std::string, std::string>::iterator itr = commands_.find(command_name);
	if (itr == commands_.end()) {
		return "";
	}
	return itr->second;
}

void bright::input::InputContextManager::on_keyboard_event(std::shared_ptr<bright::input::KeyboardEvent> pKeyboardInputEvent) {

	std::string control_name = pKeyboardInputEvent->key_string();
	std::string command_name = control_to_command(control_name);
	// Control is not assigned to a command
	if (command_name == "") {
		return;
	}
	std::string command_type = command_to_type(command_name);
	
	if (command_type == "KEY" ) {
		if (pKeyboardInputEvent->is_down()) {
			std::cout << "control_name: " << control_name << " command_name: " << command_name << " command_type: " << command_type << std::endl;
			auto command_info = std::make_shared<std::vector<std::string>>();
			command_info->push_back(command_name);
			command_info->push_back(command_type);
		    update_command_listeners(command_info);
		}
	}
	else if (command_type == "STATE" ) {
		std::string state_data = "STATE_OFF";
		if (pKeyboardInputEvent->is_down()) {
			state_data = "STATE_ON";
		}
		else if (!pKeyboardInputEvent->is_down()) {
			state_data = "STATE_OFF";
		}
		std::cout << "control_name: " << control_name << " command_name: " << command_name << " command_type: " << command_type << "state_data: " << state_data << std::endl;
			auto command_info = std::make_shared<std::vector<std::string>>();
			command_info->push_back(command_name);
			command_info->push_back(command_type);
			command_info->push_back(state_data);
		    update_command_listeners(command_info);
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
  std::cout << std::endl << std::flush;
}

//TODO: Move this to some kind of map
std::string InputContextManager::get_mouse_button_name(std::shared_ptr<bright::input::RawMouseEvent> pRawMouseInputEvent) {
	std::string button_name = "";
    if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::MB_UNDEF) { 
		button_name = "MOUSE_UNDEF";
	}
    else if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::LEFT) { 
		button_name = "MOUSE_LEFT";
	}
    else if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::RIGHT) { 
		button_name = "MOUSE_RIGHT";
	}
    else if ( pRawMouseInputEvent->raw_mouse_button() == bright::input::MouseButton::MIDDLE) { 
		button_name = "MOUSE_MIDDLE";
	}
    else if ( pRawMouseInputEvent->delta_rate_wheel() < 0 ) {
		button_name = "MOUSE_WHEEL_DOWN"; 
	}
    else if ( pRawMouseInputEvent->delta_rate_wheel() > 0 ) {
		button_name = "MOUSE_WHEEL_UP"; 
	}
    return button_name;
}


void InputContextManager::on_raw_mouse_event(std::shared_ptr<bright::input::RawMouseEvent> pRawMouseInputEvent){

    if ( pRawMouseInputEvent->raw_mouse_event_type() == bright::input::MouseEventType::BUTTON_CLICK){
		std::string control_name = get_mouse_button_name(pRawMouseInputEvent);
		std::string command_name = control_to_command(control_name);
		if (command_name != "") {
			std::string command_type = command_to_type(command_name);
			if (command_type == "KEY" ) {
				if (pRawMouseInputEvent->is_raw_mouse_button_down()) {
					std::cout << "control_name: " << control_name << " command_name: " << command_name << " command_type: " << command_type << std::endl;
					auto command_info = std::make_shared<std::vector<std::string>>();
					command_info->push_back(command_name);
					command_info->push_back(command_type);
					update_command_listeners(command_info);
				}
			}
			else if (command_type == "STATE" ) {
				std::string state_data = "STATE_OFF";
				if (pRawMouseInputEvent->is_raw_mouse_button_down()) {
					state_data = "STATE_ON";
				}
				std::cout << "control_name: " << control_name << " command_name: " << command_name << " command_type: " << command_type << "state_data: " << state_data << std::endl;
				auto command_info = std::make_shared<std::vector<std::string>>();
				command_info->push_back(command_name);
				command_info->push_back(command_type);
				command_info->push_back(state_data);
				update_command_listeners(command_info);
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
	std::string control_name = "";
	int delta_x = pRawMouseInputEvent->delta_rate_x();
	int delta_y = pRawMouseInputEvent->delta_rate_y();
    //std::cout << "2 : MOVEMENT" << std::endl << std::flush; 
    //std::cout << "deltaX: " << delta_x << std::endl << std::flush;
    //std::cout << "deltaY: " << delta_y << std::endl << std::flush;

	if (delta_x != 0) {
	        //TODO: refactor duplicate code
		    control_name = "MOUSE_X_AXIS";
	        std::string command_name = control_to_command(control_name);
			// If control is assigned to control, send it out
			if (command_name != "") {
				std::string command_type = command_to_type(command_name);
  				//std::cout << "control_name: " << control_name << " command_name: " << command_name << " command_type: " << command_type << std::endl;
				auto command_info = std::make_shared<std::vector<std::string>>();
				command_info->push_back(command_name);
				command_info->push_back(command_type);
				command_info->push_back(std::to_string(delta_x));
				update_command_listeners(command_info);
			}
	}
	if (delta_y != 0) {
	        //TODO: refactor duplicate code
		    control_name = "MOUSE_Y_AXIS";
	        std::string command_name = control_to_command(control_name);
			// If control is assigned to control, send it out
			if (command_name != "") {
				std::string command_type = command_to_type(command_name);
  				//std::cout << "control_name: " << control_name << " command_name: " << command_name << " command_type: " << command_type << std::endl;
				auto command_info = std::make_shared<std::vector<std::string>>();
				command_info->push_back(command_name);
				command_info->push_back(command_type);
				command_info->push_back(std::to_string(delta_y));
				update_command_listeners(command_info);
			}
	}
  }
 // std::cout << std::endl << std::flush;
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

void InputContextManager::update_command_listeners(std::shared_ptr<std::vector<std::string>>  command_info) {
	auto commandEvent = std::make_shared<CommandEvent>(command_info);
	commandEvents_.push_back(commandEvent);
}