#include "input/inputmanager.hpp"

using namespace bright::input;

InputManager::InputManager(){
  initialize_keys_map();
  keyboard_[Key::APOSTROPHE] = false;
  keyboard_[Key::COMMA] = false;
  keyboard_[Key::DASH] = false;
  keyboard_[Key::PERIOD] = false;
  keyboard_[Key::FORWARD_SLASH] = false;
  keyboard_[Key::ZERO] = false;
  keyboard_[Key::ONE] = false;
  keyboard_[Key::TWO] = false;
  keyboard_[Key::THREE] = false;
  keyboard_[Key::FOUR] = false;
  keyboard_[Key::FIVE] = false;
  keyboard_[Key::SIX] = false;
  keyboard_[Key::SEVEN] = false;
  keyboard_[Key::EIGHT] = false;
  keyboard_[Key::NINE] = false;
  keyboard_[Key::TEN] = false;
  keyboard_[Key::EQUAL_SIGN] = false;
  keyboard_[Key::A] = false;
  keyboard_[Key::ALT] = false;
  keyboard_[Key::APPLICATION] = false;
  keyboard_[Key::B] = false;
  keyboard_[Key::BACKSPACE] = false;
  keyboard_[Key::C] = false;
  keyboard_[Key::CAPS_LOCK] = false;
  keyboard_[Key::CTRL] = false;
  keyboard_[Key::D] = false;
  keyboard_[Key::DEL] = false;
  keyboard_[Key::DOWN] = false;
  keyboard_[Key::E] = false;
  keyboard_[Key::END] = false;
  keyboard_[Key::ENTER] = false;
  keyboard_[Key::ESC] = false;
  keyboard_[Key::F] = false;
  keyboard_[Key::F1] = false;
  keyboard_[Key::F10] = false;
  keyboard_[Key::F11] = false;
  keyboard_[Key::F12] = false;
  keyboard_[Key::F2] = false;
  keyboard_[Key::F3] = false;
  keyboard_[Key::F4] = false;
  keyboard_[Key::F5] = false;
  keyboard_[Key::F6] = false;
  keyboard_[Key::F7] = false;
  keyboard_[Key::F8] = false;
  keyboard_[Key::F9] = false;
  keyboard_[Key::G] = false;
  keyboard_[Key::H] = false;
  keyboard_[Key::HOME] = false;
  keyboard_[Key::I] = false;
  keyboard_[Key::INSERT] = false;
  keyboard_[Key::J] = false;
  keyboard_[Key::K] = false;
  keyboard_[Key::L] = false;
  keyboard_[Key::LEFT] = false;
  keyboard_[Key::LEFT_WINDOWS] = false;
  keyboard_[Key::M] = false;
  keyboard_[Key::N] = false;
  keyboard_[Key::NUM_STAR] = false;
  keyboard_[Key::NUM_PLUS] = false;
  keyboard_[Key::NUM_MINUS] = false;
  keyboard_[Key::NUM_FORWARD_SLASH] = false;
  keyboard_[Key::NUM_0] = false;
  keyboard_[Key::NUM_1] = false;
  keyboard_[Key::NUM_2] = false;
  keyboard_[Key::NUM_3] = false;
  keyboard_[Key::NUM_4] = false;
  keyboard_[Key::NUM_5] = false;
  keyboard_[Key::NUM_6] = false;
  keyboard_[Key::NUM_7] = false;
  keyboard_[Key::NUM_8] = false;
  keyboard_[Key::NUM_9] = false;
  keyboard_[Key::NUM_DEL] = false;
  keyboard_[Key::NUM_ENTER] = false;
  keyboard_[Key::NUM_LOCK] = false;
  keyboard_[Key::O] = false;
  keyboard_[Key::P] = false;
  keyboard_[Key::PAGE_DOWN] = false;
  keyboard_[Key::PAGE_UP] = false;
  keyboard_[Key::PAUSE] = false;
  keyboard_[Key::PRNT_SCRN] = false;
  keyboard_[Key::Q] = false;
  keyboard_[Key::R] = false;
  keyboard_[Key::RIGHT] = false;
  keyboard_[Key::RIGHT_ALT] = false;
  keyboard_[Key::RIGHT_CTRL] = false;
  keyboard_[Key::RIGHT_SHIFT] = false;
  keyboard_[Key::S] = false;
  keyboard_[Key::SCROLL_LOCK] = false;
  keyboard_[Key::SHIFT] = false;
  keyboard_[Key::SPACE] = false;
  keyboard_[Key::T] = false;
  keyboard_[Key::TAB] = false;
  keyboard_[Key::U] = false;
  keyboard_[Key::UP] = false;
  keyboard_[Key::V] = false;
  keyboard_[Key::W] = false;
  keyboard_[Key::X] = false;
  keyboard_[Key::Y] = false;
  keyboard_[Key::Z] = false;
  keyboard_[Key::OPEN_SQUARE_BRACKET] = false;
  keyboard_[Key::BACKSLASH] = false;
  keyboard_[Key::CLOSE_SQUARE_BRACKET] = false;
  keyboard_[Key::TILDE] = false;
}


void InputManager::add_keyboard_event_listener(std::shared_ptr<KeyboardEventListener> pKeyboardEventListener){
  pKeyboardEventListeners_.push_back(pKeyboardEventListener);
}


void InputManager::add_mouse_event_listener(std::shared_ptr<MouseEventListener> pMouseEventListener){
  pMouseEventListeners_.push_back(pMouseEventListener);
}


void InputManager::add_raw_mouse_event_listener(std::shared_ptr<RawMouseEventListener> pRawMouseEventListener){
  pRawMouseEventListeners_.push_back(pRawMouseEventListener);
}


void InputManager::notify(){
  if ( !keyboardEvents_.empty() ){
    notify_keyboard_listeners();
  }
  if ( !mouseEvents_.empty() ){
    notify_mouse_listeners();
  }
  if ( !rawMouseEvents_.empty() ){
    notify_raw_mouse_listeners();
  }
}

void InputManager::update_mouse_listeners_position(float newXPos, float newYPos){
  auto mouseEvent = std::make_shared<MouseEvent>(MouseEventType::MOVEMENT, MouseButton::MB_UNDEF, newXPos, newYPos);
  mouseEvents_.push_back(mouseEvent);
}


void InputManager::notify_mouse_listeners(){

  std::shared_ptr<MouseEventListener> pCurrentMouseEventListener;

  auto notify = [&] (std::shared_ptr<MouseEvent> pMouseEvent) { 
    pCurrentMouseEventListener->on_mouse_event(pMouseEvent);
  };

  auto notify_mouse_listener = [&] (std::shared_ptr<MouseEventListener> pMouseEventListener) {
    pCurrentMouseEventListener = pMouseEventListener;
    std::for_each(this->mouseEvents_.begin(), this->mouseEvents_.end(), notify);
  };

  std::for_each(pMouseEventListeners_.begin(), pMouseEventListeners_.end(), notify_mouse_listener);

  mouseEvents_.clear();
}


void InputManager::update_keyboard_listeners(bool isDown, std::string asciiValue){

  std::transform(asciiValue.begin(), asciiValue.end(), asciiValue.begin(), toupper);
  Key key = KEYS_MAP_[asciiValue];

  if (keyboard_[key] == isDown){}
  else { 
    keyboard_[key] = isDown; 
    auto keyboardEvent = std::make_shared<KeyboardEvent>(isDown, asciiValue, key);
    keyboardEvents_.push_back(keyboardEvent);
  }
}


void InputManager::notify_keyboard_listeners(){

  std::shared_ptr<KeyboardEventListener> pCurrentKeyboardEventListener;

  auto notify = [&] (std::shared_ptr<KeyboardEvent> pKeyboardEvent) { 
    pCurrentKeyboardEventListener->on_keyboard_event(pKeyboardEvent);
  };

  auto notify_keyboard_listener = [&] (std::shared_ptr<KeyboardEventListener> pKeyboardEventListener) {
    pCurrentKeyboardEventListener = pKeyboardEventListener;
    std::for_each(this->keyboardEvents_.begin(), this->keyboardEvents_.end(), notify);
  };

  std::for_each(pKeyboardEventListeners_.begin(), pKeyboardEventListeners_.end(), notify_keyboard_listener);

  keyboardEvents_.clear();
}


void InputManager::update_raw_mouse_listeners_deltas(int xPos, int yPos){
  auto rawMouseEvent = std::make_shared<RawMouseEvent>(MouseEventType::MOVEMENT, MouseButton::MB_UNDEF, false, xPos, yPos, 0);
  rawMouseEvents_.push_back(rawMouseEvent);
}

void InputManager::update_raw_mouse_listeners_button(bool isDown, MouseButton button, short int data){

  if ( rawMouseButtons_[button] == isDown && (button != MouseButton::WHEEL) ){}
  else { 
    rawMouseButtons_[button] = isDown; 
    auto rawMouseEvent = std::make_shared<RawMouseEvent>(MouseEventType::BUTTON_CLICK, button, isDown, 0, 0, data);
    rawMouseEvents_.push_back(rawMouseEvent);
  }

}


void InputManager::notify_raw_mouse_listeners(){

  std::shared_ptr<RawMouseEventListener> pCurrentRawMouseEventListener;

  auto notify = [&] (std::shared_ptr<RawMouseEvent> rawMouseEvent) { 
    pCurrentRawMouseEventListener->on_raw_mouse_event(rawMouseEvent);
  };

  auto notify_raw_mouse_listener = [&] (std::shared_ptr<RawMouseEventListener> pRawMouseEventListener) {
    pCurrentRawMouseEventListener = pRawMouseEventListener;
    std::for_each(this->rawMouseEvents_.begin(), this->rawMouseEvents_.end(), notify);
  };

  std::for_each(pRawMouseEventListeners_.begin(), pRawMouseEventListeners_.end(), notify_raw_mouse_listener);


  rawMouseEvents_.clear();
}



void InputManager::initialize_keys_map(){
   KEYS_MAP_["'"]            = Key::APOSTROPHE;         
   KEYS_MAP_[","]            = Key::COMMA;              
   KEYS_MAP_["-"]            = Key::DASH;               
   KEYS_MAP_["."]            = Key::PERIOD;             
   KEYS_MAP_["/"]            = Key::FORWARD_SLASH;      
   KEYS_MAP_["0"]            = Key::ZERO;               
   KEYS_MAP_["1"]            = Key::ONE;                
   KEYS_MAP_["2"]            = Key::TWO;                
   KEYS_MAP_["3"]            = Key::THREE;              
   KEYS_MAP_["4"]            = Key::FOUR;               
   KEYS_MAP_["5"]            = Key::FIVE;               
   KEYS_MAP_["6"]            = Key::SIX;                
   KEYS_MAP_["7"]            = Key::SEVEN;              
   KEYS_MAP_["8"]            = Key::EIGHT;              
   KEYS_MAP_["9"]            = Key::NINE;               
   KEYS_MAP_["10"]           = Key::TEN;                
   KEYS_MAP_["="]            = Key::EQUAL_SIGN;         
   KEYS_MAP_["A"]            = Key::A;                  
   KEYS_MAP_["ALT"]          = Key::ALT;                
   KEYS_MAP_["APPLICATION"]  = Key::APPLICATION;        
   KEYS_MAP_["B"]            = Key::B;                  
   KEYS_MAP_["BACKSPACE"]    = Key::BACKSPACE;          
   KEYS_MAP_["C"]            = Key::C;                  
   KEYS_MAP_["CAPS_LOCK"]    = Key::CAPS_LOCK;          
   KEYS_MAP_["CTRL"]         = Key::CTRL;               
   KEYS_MAP_["D"]            = Key::D;                  
   KEYS_MAP_["DELETE"]       = Key::DEL;             
   KEYS_MAP_["DOWN"]         = Key::DOWN;               
   KEYS_MAP_["E"]            = Key::E;                  
   KEYS_MAP_["END"]          = Key::END;                
   KEYS_MAP_["ENTER"]        = Key::ENTER;              
   KEYS_MAP_["ESC"]          = Key::ESC;                
   KEYS_MAP_["F"]            = Key::F;                  
   KEYS_MAP_["F1"]           = Key::F1;                 
   KEYS_MAP_["F10"]          = Key::F10;                
   KEYS_MAP_["F11"]          = Key::F11;                
   KEYS_MAP_["F12"]          = Key::F12;                
   KEYS_MAP_["F2"]           = Key::F2;                 
   KEYS_MAP_["F3"]           = Key::F3;                 
   KEYS_MAP_["F4"]           = Key::F4;                 
   KEYS_MAP_["F5"]           = Key::F5;                 
   KEYS_MAP_["F6"]           = Key::F6;                 
   KEYS_MAP_["F7"]           = Key::F7;                 
   KEYS_MAP_["F8"]           = Key::F8;                 
   KEYS_MAP_["F9"]           = Key::F9;                 
   KEYS_MAP_["G"]            = Key::G;                  
   KEYS_MAP_["H"]            = Key::H;                  
   KEYS_MAP_["HOME"]         = Key::HOME;               
   KEYS_MAP_["I"]            = Key::I;                  
   KEYS_MAP_["INSERT"]       = Key::INSERT;             
   KEYS_MAP_["J"]            = Key::J;                  
   KEYS_MAP_["K"]            = Key::K;                  
   KEYS_MAP_["L"]            = Key::L;                  
   KEYS_MAP_["LEFT"]         = Key::LEFT;               
   KEYS_MAP_["LEFT_WINDOWS"] = Key::LEFT_WINDOWS;       
   KEYS_MAP_["M"]            = Key::M;                  
   KEYS_MAP_["N"]            = Key::N;                  
   KEYS_MAP_["NUM *"]        = Key::NUM_STAR;           
   KEYS_MAP_["NUM +"]        = Key::NUM_PLUS;           
   KEYS_MAP_["NUM -"]        = Key::NUM_MINUS;          
   KEYS_MAP_["NUM /"]        = Key::NUM_FORWARD_SLASH;  
   KEYS_MAP_["NUM_0"]        = Key::NUM_0;              
   KEYS_MAP_["NUM_1"]        = Key::NUM_1;              
   KEYS_MAP_["NUM_2"]        = Key::NUM_2;              
   KEYS_MAP_["NUM_3"]        = Key::NUM_3;              
   KEYS_MAP_["NUM_4"]        = Key::NUM_4;              
   KEYS_MAP_["NUM_5"]        = Key::NUM_5;              
   KEYS_MAP_["NUM_6"]        = Key::NUM_6;              
   KEYS_MAP_["NUM_7"]        = Key::NUM_7;              
   KEYS_MAP_["NUM_8"]        = Key::NUM_8;              
   KEYS_MAP_["NUM_9"]        = Key::NUM_9;              
   KEYS_MAP_["NUM_DEL"]      = Key::NUM_DEL;            
   KEYS_MAP_["NUM_ENTER"]    = Key::NUM_ENTER;          
   KEYS_MAP_["NUM_LOCK"]     = Key::NUM_LOCK;           
   KEYS_MAP_["O"]            = Key::O;                  
   KEYS_MAP_["P"]            = Key::P;                  
   KEYS_MAP_["PAGE_DOWN"]    = Key::PAGE_DOWN;          
   KEYS_MAP_["PAGE_UP"]      = Key::PAGE_UP;            
   KEYS_MAP_["PAUSE"]        = Key::PAUSE;              
   KEYS_MAP_["PRNT_SCRN"]    = Key::PRNT_SCRN;          
   KEYS_MAP_["Q"]            = Key::Q;                  
   KEYS_MAP_["R"]            = Key::R;                  
   KEYS_MAP_["RIGHT"]        = Key::RIGHT;              
   KEYS_MAP_["RIGHT_ALT"]    = Key::RIGHT_ALT;          
   KEYS_MAP_["RIGHT_CTRL"]   = Key::RIGHT_CTRL;         
   KEYS_MAP_["RIGHT_SHIFT"]  = Key::RIGHT_SHIFT;        
   KEYS_MAP_["S"]            = Key::S;                  
   KEYS_MAP_["SCROLL_LOCK"]  = Key::SCROLL_LOCK;        
   KEYS_MAP_["SHIFT"]        = Key::SHIFT;              
   KEYS_MAP_["SPACE"]        = Key::SPACE;              
   KEYS_MAP_["T"]            = Key::T;                  
   KEYS_MAP_["TAB"]          = Key::TAB;                
   KEYS_MAP_["U"]            = Key::U;                  
   KEYS_MAP_["UP"]           = Key::UP;                 
   KEYS_MAP_["V"]            = Key::V;                  
   KEYS_MAP_["W"]            = Key::W;                  
   KEYS_MAP_["X"]            = Key::X;                  
   KEYS_MAP_["Y"]            = Key::Y;                  
   KEYS_MAP_["Z"]            = Key::Z;                  
   KEYS_MAP_["["]            = Key::OPEN_SQUARE_BRACKET;
   KEYS_MAP_["\\"]           = Key::BACKSLASH;          
   KEYS_MAP_["]"]            = Key::CLOSE_SQUARE_BRACKET;
   KEYS_MAP_["`"]            = Key::TILDE;              
}