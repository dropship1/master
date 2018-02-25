#include "windows/entrypointeventhandler.hpp"

using namespace bright::windows;

std::shared_ptr<EntryPointEventHandler> EntryPointEventHandler::pEntryPointEventHandler_ = std::make_shared<EntryPointEventHandler>();

EntryPointEventHandler::EntryPointEventHandler(): 
  pContextManager_(std::make_shared<bright::context::ContextManager>()),
  pInputManager_(std::make_shared<bright::input::InputManager>()) {
}

LRESULT EntryPointEventHandler::handle_event(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam){
  // Route Windows messages to game engine member functions
  switch (msg) {
  case WM_CREATE: {
    return 0;
  }

  case WM_SETFOCUS: {
    return 0;
  }

  case WM_KILLFOCUS: {
    return 0;
  }

  case WM_SIZE: {
    pContextManager_->reshape_rendering_context_viewport(LOWORD(lParam), HIWORD(lParam));
    return 0;
  }

  case WM_DESTROY: {
    PostQuitMessage(0);
    return 0;
  }

  case WM_QUIT: {
    return 0;
  }

  case WM_INPUT: {
    UINT dwSize;

    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,
      sizeof(RAWINPUTHEADER));
    LPBYTE lpb = new BYTE[dwSize];
    if (lpb == NULL)
    {
      return 0;
    }

    if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
      sizeof(RAWINPUTHEADER)) != dwSize)
      OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

    RAWINPUT* raw = (RAWINPUT*)lpb;

    if (raw->header.dwType == RIM_TYPEKEYBOARD) {
      //https://molecularmusings.wordpress.com/2011/09/05/properly-handling-keyboard-input/
      unsigned int scanCode = raw->data.keyboard.MakeCode;
      unsigned int virtualKey = raw->data.keyboard.VKey;
      unsigned int flags = raw->data.keyboard.Flags;

      if (virtualKey == 255) {
        // discard "fake keys" which are part of an escaped sequence
        return 0;
      }
      else if (virtualKey == VK_SHIFT) {
        // correct left-hand / right-hand SHIFT
        virtualKey = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
      }
      else if (virtualKey == VK_NUMLOCK) {
        // correct PAUSE/BREAK and NUM LOCK silliness, and set the extended bit
        scanCode = (MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC) | 0x100);
      }
      // e0 and e1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK.
      // see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
      const bool isE0 = ((flags & RI_KEY_E0) != 0);
      const bool isE1 = ((flags & RI_KEY_E1) != 0);

      if (isE1) {
        // for escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey.
        // however, MapVirtualKey is unable to map VK_PAUSE (this is a known bug), hence we map that by hand.
        if (virtualKey == VK_PAUSE) {
          scanCode = 0x45;
        }
        else {
          scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);
        }
      }

      switch (virtualKey) {
        // right-hand CONTROL and ALT have their e0 bit set
      case VK_CONTROL:
        if (isE0)
          virtualKey = WinKeys::RIGHT_CONTROL;
        else
          virtualKey = WinKeys::LEFT_CONTROL;
        break;

      case VK_MENU:
        if (isE0)
          virtualKey = WinKeys::RIGHT_ALT;
        else
          virtualKey = WinKeys::LEFT_ALT;
        break;

        // NUMPAD ENTER has its e0 bit set
      case VK_RETURN:
        if (isE0)
          virtualKey = WinKeys::NUMPAD_ENTER;
        break;

        // the standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always have their e0 bit set, but the
        // corresponding keys on the NUMPAD will not.
      case VK_INSERT:
        if (!isE0)
          virtualKey = WinKeys::NUMPAD_0;
        break;

      case VK_DELETE:
        if (!isE0)
          virtualKey = WinKeys::NUMPAD_DECIMAL;
        break;

      case VK_HOME:
        if (!isE0)
          virtualKey = WinKeys::NUMPAD_7;
        break;

      case VK_END:
        if (!isE0)
          virtualKey = WinKeys::NUMPAD_1;
        break;

      case VK_PRIOR:
        if (!isE0)
          virtualKey = WinKeys::NUMPAD_9;
        break;

      case VK_NEXT:
        if (!isE0)
          virtualKey = WinKeys::NUMPAD_3;
        break;

        // the standard arrow keys will always have their e0 bit set, but the
        // corresponding keys on the NUMPAD will not.
      case VK_LEFT:
        if (!isE0)
          virtualKey = WinKeys::NUMPAD_4;
        break;

      case VK_RIGHT:
        if (!isE0)
          virtualKey = WinKeys::NUMPAD_6;
        break;

      case VK_UP:
        if (!isE0)
          virtualKey = WinKeys::NUMPAD_8;
        break;

      case VK_DOWN:
        if (!isE0)
          virtualKey = WinKeys::NUMPAD_2;
        break;

        // NUMPAD 5 doesn't have its e0 bit set
      case VK_CLEAR:
        if (!isE0)
          virtualKey = WinKeys::NUMPAD_5;
        break;
      }

      // a key can either produce a "make" or "break" scancode. this is used to differentiate between down-presses and releases
      // see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
      const bool wasUp = ((flags & RI_KEY_BREAK) != 0);

      // getting a human-readable string
      UINT keyc = (scanCode << 16) | (isE0 << 24);
      char buffer[512] = {};
      GetKeyNameTextA((LONG)keyc, buffer, 512);
      std::string key(buffer);
      bool isDown = !wasUp;
      pInputManager_->update_keyboard_listeners(isDown, key);
    }
    else if (raw->header.dwType == RIM_TYPEMOUSE) {
      short int data = 0;
      int stop = 0;

      //int xPosRelative = raw->data.mouse.lLastX;
      //int yPosRelative = raw->data.mouse.lLastY;
      //pInputManager->update_raw_mouse_listeners_deltas(xPosRelative, yPosRelative);
      //if (raw->data.mouse.usFlags & (1 << MOUSE_ATTRIBUTES_CHANGED)) {
      //  std::cout << "MOUSE_ATTRIBUTES_CHANGED" <<  std::endl << std::flush;
      //}
      //if (raw->data.mouse.usFlags & (1 << MOUSE_MOVE_ABSOLUTE)) {
      //  std::cout << "MOUSE_MOVE_ABSOLUTE" << std::endl << std::flush;
      //}
      //if (raw->data.mouse.usFlags == MOUSE_VIRTUAL_DESKTOP) {
      //  std::cout << "MOUSE_VIRTUAL_DESKTOP" << std::endl << std::flush;
      //}
      if ((raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == MOUSE_MOVE_ABSOLUTE) {
        //int xPosRelative = raw->data.mouse.lLastX;
        //int yPosRelative = raw->data.mouse.lLastY;
        //pInputManager->update_raw_mouse_listeners_deltas(xPosRelative, yPosRelative);
        //
        //CURSORINFO cursorInfo = { 0 };
        //cursorInfo.cbSize = sizeof(cursorInfo);
        //GetCursorInfo(&cursorInfo);
        //float newXPos = cursorInfo.ptScreenPos.x;
        //float newYPos = cursorInfo.ptScreenPos.y;
        //pInputManager->update_mouse_listeners_position(newXPos, newYPos);

        //LPPOINT point = new POINT();
        //GetCursorPos(point);

        //ScreenToClient(pContextManager->windows_current_context()->window_id(), point);
        //std::cout << "xPoint" << point->x << std::endl << std::flush;
        //std::cout << "yPoint" << point->y << std::endl << std::flush;
        //https://stackoverflow.com/questions/31949476/raw-input-mouse-lastx-lasty-with-odd-values-while-logged-in-through-rdp

        const bool virtual_desktop = (raw->data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) == MOUSE_VIRTUAL_DESKTOP;
        const int width = GetSystemMetrics(virtual_desktop ? SM_CXVIRTUALSCREEN : SM_CXSCREEN);
        const int height = GetSystemMetrics(virtual_desktop ? SM_CYVIRTUALSCREEN : SM_CYSCREEN);
        float absolute_posX = (raw->data.mouse.lLastX / float(USHRT_MAX)) * width;
        float absolute_posY = (raw->data.mouse.lLastY / float(USHRT_MAX)) * height;
        float absolute_posZ = 0;
        if ((lastX_ != FLT_MAX) && (lastY_ != FLT_MAX) && (lastZ_ != FLT_MAX)) {
          pInputManager_->update_raw_mouse_listeners_deltas(absolute_posX - lastX_, absolute_posY - lastY_);
        }
        lastX_ = absolute_posX;
        lastY_ = absolute_posY;
        lastZ_ = absolute_posZ;
      }
      else {
        pInputManager_->update_raw_mouse_listeners_deltas(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
      }
      //MOUSE_LEFT_BUTTON_DOWN
      if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_1_DOWN) {
        pInputManager_->update_raw_mouse_listeners_button(true, bright::input::MouseButton::LEFT, data);
      }
      //MOUSE_LEFT_BUTTON_UP
      if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_1_UP) {
        pInputManager_->update_raw_mouse_listeners_button(false, bright::input::MouseButton::LEFT, data);
      }

      //MOUSE_RIGHT_BUTTON_DOWN
      if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_2_DOWN) {
        pInputManager_->update_raw_mouse_listeners_button(true, bright::input::MouseButton::RIGHT, data);
      }

      //MOUSE_RIGHT_BUTTON_UP
      if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_2_UP) {
        pInputManager_->update_raw_mouse_listeners_button(false, bright::input::MouseButton::RIGHT, data);
      }

      //MOUSE_MIDDLE_BUTTON_DOWN
      if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_3_DOWN) {
        pInputManager_->update_raw_mouse_listeners_button(true, bright::input::MouseButton::MIDDLE, data);
      }

      //MOUSE_MIDDLE_BUTTON_UP
      if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_3_UP) {
        pInputManager_->update_raw_mouse_listeners_button(false, bright::input::MouseButton::MIDDLE, data);
      }

      //Scroll wheel, Raw input comes from a mouse wheel. The wheel delta is stored in usButtonData.
      if (raw->data.mouse.usButtonFlags == RI_MOUSE_WHEEL) {
        data = (SHORT)raw->data.mouse.usButtonData;
        pInputManager_->update_raw_mouse_listeners_button(true, bright::input::MouseButton::WHEEL, data);
      }
    }
    return 0;
  }

  }
  return DefWindowProc(hWindow, msg, wParam, lParam);
}

std::shared_ptr<EntryPointEventHandler> EntryPointEventHandler::get_entry_point_event_handler()
{
  return pEntryPointEventHandler_;
}

std::shared_ptr<bright::context::ContextManager> EntryPointEventHandler::get_context_manager()
{
  return pContextManager_;
}

std::shared_ptr<bright::input::InputManager> EntryPointEventHandler::get_input_manager()
{
  return pInputManager_;
}

LRESULT CALLBACK WndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
  return EntryPointEventHandler::get_entry_point_event_handler()->handle_event(window, msg, wparam, lparam);
}