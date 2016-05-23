#ifndef BRIGHT_INPUT_INPUT_CONTEXT_MANAGER_H
#define BRIGHT_INPUT_INPUT_CONTEXT_MANAGER_H

#include <iostream>

#include "globalstructs.hpp"

#include "input/keyboardevent.hpp"
#include "input/mouseevent.hpp"
#include "input/rawmouseevent.hpp"
#include "input/commandevent.hpp"

#include "input/keyboardeventlistener.hpp"
#include "input/mouseeventlistener.hpp"
#include "input/rawmouseeventlistener.hpp"
#include "input/commandeventlistener.hpp"

#include <algorithm>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>


namespace bright{
  
    namespace input{

/**
 * @ingroup input
 *
 * @brief The input context manager class
 * 
 * @details 
 * This manages all input contexts, converts control input to commands
 * Currently supports keyboard/mouse/rawmouse
 * Example Usage:
 * 
 * @todo
 * Add code usage example
 *
 */

class InputContextManager : public KeyboardEventListener, public RawMouseEventListener, public MouseEventListener {
public:
	InputContextManager() {
	// From KeyboardListener
        shiftDown_ = false; // TODO: Set this at right place
        capsLockDown_ = false; // TODO: Set this at right place
	}
	void initialize(std::string controls_contexts);
	//void load_config(std::string path);
	void add_command(std::string command_name, std::string command_type);
	void add_control(std::string control_name, std::string command_name);
	void set_control_config(std::string control_config);
	std::string control_to_command(std::string control_name);
	std::string command_to_type(std::string command_name);

	void on_keyboard_event(std::shared_ptr<KeyboardEvent> pKeyboardInputEvent);
    std::string get_mouse_button_name(std::shared_ptr<bright::input::RawMouseEvent> pRawMouseInputEvent);
    void on_raw_mouse_event(std::shared_ptr<bright::input::RawMouseEvent> pRawMouseInputEvent);
    void on_mouse_event(std::shared_ptr<bright::input::MouseEvent> pMouseInputEvent);

    void notify();

    void add_command_event_listener(boost::shared_ptr<CommandEventListener> pCommandEventListener);
	void update_command_listeners(std::shared_ptr<std::vector<std::string>>  command_info);

private:
	void notify_command_listeners();
	std::map<std::string, std::string> commands_;  // Command Name, Control Type
	std::map<std::string, std::string> controls_;  // Control Name, Command Name
    std::vector<boost::shared_ptr<CommandEventListener>> pCommandEventListeners_;
    std::vector<std::shared_ptr<CommandEvent>> commandEvents_;

	bool shiftDown_;
    bool capsLockDown_;
};

  } // end namespace input
} // end namespace bright

#endif