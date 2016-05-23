#ifndef BRIGHT_INPUT_GLOBAL_STRUCTS_H
#define BRIGHT_INPUT_GLOBAL_STRUCTS_H

#include <map>
#include <vector>
#include <string>

namespace bright{
  
    namespace input{ 


/**
 * @ingroup input
 *
 * @brief input helper structs
 * 
 * @details 
 * Structs to help with the input handling
 * Example Usage:
 * @code
 * @endcode
 *
 */  
enum class MouseEventType {ME_UNDEF, BUTTON_CLICK, MOVEMENT};
enum class MouseButton {MB_UNDEF, LEFT, RIGHT, MIDDLE, WHEEL};	
enum class Key {
  APOSTROPHE,             //'
  COMMA,                  //, 
  DASH,                   //-
  PERIOD,                 //.
  FORWARD_SLASH,          ///
  ZERO,                   //0
  ONE,                    //1
  TWO,                    //2
  THREE,                  //3
  FOUR,                   //4
  FIVE,                   //5
  SIX,                    //6
  SEVEN,                  //7
  EIGHT,                  //8
  NINE,                   //9
  TEN,                    //;
  EQUAL_SIGN,             //=
  A,                      //A
  ALT,                    //ALT
  APPLICATION,            //APPLICATION
  B,                      //B
  BACKSPACE,              //BACKSPACE
  C,                      //C
  CAPS_LOCK,              //CAPS_LOCK
  CTRL,                   //CTRL
  D,                      //D
  DEL,                    //DELETE
  DOWN,                   //DOWN
  E,                      //E
  END,                    //END
  ENTER,                  //ENTER
  ESC,                    //ESC
  F,                      //F
  F1,                     //F1
  F10,                    //F10
  F11,                    //F11
  F12,                    //F12
  F2,                     //F2
  F3,                     //F3
  F4,                     //F4
  F5,                     //F5
  F6,                     //F6
  F7,                     //F7
  F8,                     //F8
  F9,                     //F9
  G,                      //G
  H,                      //H
  HOME,                   //HOME
  I,                      //I
  INSERT,                 //INSERT
  J,                      //J
  K,                      //K
  L,                      //L
  LEFT,                   //LEFT
  LEFT_WINDOWS,           //LEFT_WINDOWS
  M,                      //M
  N,                      //N
  NUM_STAR,               //NUM *
  NUM_PLUS,               //NUM +
  NUM_MINUS,              //NUM - 
  NUM_FORWARD_SLASH,      //NUM /
  NUM_0,                  //NUM_0
  NUM_1,                  //NUM_1
  NUM_2,                  //NUM_2
  NUM_3,                  //NUM_3
  NUM_4,                  //NUM_4
  NUM_5,                  //NUM_5
  NUM_6,                  //NUM_6
  NUM_7,                  //NUM_7
  NUM_8,                  //NUM_8
  NUM_9,                  //NUM_9
  NUM_DEL,                //NUM_DEL
  NUM_ENTER,              //NUM_ENTER
  NUM_LOCK,               //NUM_LOCK
  O,                      //O
  P,                      //P
  PAGE_DOWN,              //PAGE_DOWN
  PAGE_UP,                //PAGE_UP
  PAUSE,                  //PAUSE
  PRNT_SCRN,              //PRNT_SCRN
  Q,                      //Q
  R,                      //R
  RIGHT,                  //RIGHT
  RIGHT_ALT,              //RIGHT_ALT
  RIGHT_CTRL,             //RIGHT_CTRL
  RIGHT_SHIFT,            //RIGHT_SHIFT
  S,                      //S
  SCROLL_LOCK,            //SCROLL_LOCK
  SHIFT,                  //SHIFT
  SPACE,                  //SPACE
  T,                      //T
  TAB,                    //TAB
  U,                      //U
  UP,                     //UP
  V,                      //V
  W,                      //W
  X,                      //X
  Y,                      //Y
  Z,                      //Z
  OPEN_SQUARE_BRACKET,    //[
  BACKSLASH,              /*\*/
  CLOSE_SQUARE_BRACKET,   //]
  TILDE                   //`
};


//struct Key{
//  bool isDown_;
//  std::string name_;
//  int index_;
//};

struct RawMouse{
  int deltaRateChangeX_;
  int deltaRateChangeY_;
  int deltaRateChangeWheel_;

  bool leftButtonDown_;
  bool rightButtonDown_;
  bool middleButtonDown_;
};

struct Deltas{
  float x_;
  float y_;
};


struct Mouse{
  float xPos_;
  float yPos_;
  float oldXPos_;
  float oldYPos_;
  bool leftButtonDown_;
  bool rightButtonDown_;
};


struct ControlState{
  ControlState(): moveForward_(false),moveBackward_(false),moveLeft_(false),moveRight_(false){}
  bool moveForward_;
  bool moveBackward_;
  bool moveLeft_;
  bool moveRight_;
  std::vector<std::string> rotations_;
};

struct Bindings{
  std::map<std::string, std::string> bindings_;
};

struct InputBindings{
  Bindings keyboard_;
  Bindings rawMouse_;
  std::string actorName_;
  int actorId_;
};

  }
}

#endif
