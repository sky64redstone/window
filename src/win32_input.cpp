#include "include/input.hpp"
#include "include/platform.hpp"

namespace window {
  inline key_descriptor win32_keycodes[0xFF] = { { KEY_COUNT, "ERROR" } };

  static void load_win32_keys() noexcept {
    if (win32_keycodes[0].k != KEY_COUNT)
      return;

    win32_keycodes[0] = { KEY_NONE, "None" };
    for (int i = 1; i < sizeof(win32_keycodes) / sizeof(win32_keycodes[0]); ++i) {
      win32_keycodes[i] = { KEY_NONE, "Unknown" };
    }

    win32_keycodes[VK_BACK]       = { KEY_BACKSPACE, "Backspace" };
    win32_keycodes[VK_TAB]        = { KEY_TAB, "Tab" };
    win32_keycodes[VK_RETURN]     = { KEY_RETURN, "Return" };
    win32_keycodes[VK_SHIFT]      = { KEY_SHIFT, "Shift" };
    win32_keycodes[VK_CONTROL]    = { KEY_CTRL, "Control" };
    win32_keycodes[VK_MENU]       = { KEY_ALT, "Alt" };
    win32_keycodes[VK_CAPITAL]    = { KEY_CAPSLOCK, "Caps lock" };
    win32_keycodes[VK_ESCAPE]     = { KEY_ESCAPE, "Escape" };
    win32_keycodes[VK_SPACE]      = { KEY_SPACE, "Space" };
    win32_keycodes[VK_PRIOR]      = { KEY_PAGE_UP, "Page up" };
    win32_keycodes[VK_NEXT]       = { KEY_PAGE_DOWN, "Page down" };
    win32_keycodes[VK_END]        = { KEY_END, "End" };
    win32_keycodes[VK_HOME]       = { KEY_HOME, "Home" };
    win32_keycodes[VK_LEFT]       = { KEY_LEFT, "Left" };
    win32_keycodes[VK_UP]         = { KEY_UP, "Up" };
    win32_keycodes[VK_RIGHT]      = { KEY_RIGHT, "Right" };
    win32_keycodes[VK_DOWN]       = { KEY_DOWN, "Down" };
    win32_keycodes[VK_INSERT]     = { KEY_INSERT, "Insert" };
    win32_keycodes[VK_DELETE]     = { KEY_DELETE, "Delete" };
    win32_keycodes['0']           = { KEY_0, "Zero" };
    win32_keycodes['1']           = { KEY_1, "One" };
    win32_keycodes['2']           = { KEY_2, "Two" };
    win32_keycodes['3']           = { KEY_3, "Three" };
    win32_keycodes['4']           = { KEY_4, "Four" };
    win32_keycodes['5']           = { KEY_5, "Five" };
    win32_keycodes['6']           = { KEY_6, "Six" };
    win32_keycodes['7']           = { KEY_7, "Seven" };
    win32_keycodes['8']           = { KEY_8, "Eight" };
    win32_keycodes['9']           = { KEY_9, "Nine" };
    win32_keycodes['A']           = { KEY_A, "A" };
    win32_keycodes['B']           = { KEY_B, "B" };
    win32_keycodes['C']           = { KEY_C, "C" };
    win32_keycodes['D']           = { KEY_D, "D" };
    win32_keycodes['E']           = { KEY_E, "E" };
    win32_keycodes['F']           = { KEY_F, "F" };
    win32_keycodes['G']           = { KEY_G, "G" };
    win32_keycodes['H']           = { KEY_H, "H" };
    win32_keycodes['I']           = { KEY_I, "I" };
    win32_keycodes['J']           = { KEY_J, "J" };
    win32_keycodes['K']           = { KEY_K, "K" };
    win32_keycodes['L']           = { KEY_L, "L" };
    win32_keycodes['M']           = { KEY_M, "M" };
    win32_keycodes['N']           = { KEY_N, "N" };
    win32_keycodes['O']           = { KEY_O, "O" };
    win32_keycodes['P']           = { KEY_P, "P" };
    win32_keycodes['Q']           = { KEY_Q, "Q" };
    win32_keycodes['R']           = { KEY_R, "R" };
    win32_keycodes['S']           = { KEY_S, "S" };
    win32_keycodes['T']           = { KEY_T, "T" };
    win32_keycodes['U']           = { KEY_U, "U" };
    win32_keycodes['V']           = { KEY_V, "V" };
    win32_keycodes['W']           = { KEY_W, "W" };
    win32_keycodes['X']           = { KEY_X, "X" };
    win32_keycodes['Y']           = { KEY_Y, "Y" };
    win32_keycodes['Z']           = { KEY_Z, "Z" };
    win32_keycodes[VK_LWIN]       = { KEY_SUPER, "Super" };
    win32_keycodes[VK_RWIN]       = { KEY_SUPER, "Super" };
    win32_keycodes[VK_NUMPAD0]    = { KEY_NP0, "Numpad Zero" };
    win32_keycodes[VK_NUMPAD0]    = { KEY_NP0, "Numpad Zero" };
    win32_keycodes[VK_NUMPAD1]    = { KEY_NP1, "Numpad One" };
    win32_keycodes[VK_NUMPAD2]    = { KEY_NP2, "Numpad Two" };
    win32_keycodes[VK_NUMPAD3]    = { KEY_NP3, "Numpad Three" };
    win32_keycodes[VK_NUMPAD4]    = { KEY_NP4, "Numpad Four" };
    win32_keycodes[VK_NUMPAD5]    = { KEY_NP5, "Numpad Five" };
    win32_keycodes[VK_NUMPAD6]    = { KEY_NP6, "Numpad Six" };
    win32_keycodes[VK_NUMPAD7]    = { KEY_NP7, "Numpad Seven" };
    win32_keycodes[VK_NUMPAD8]    = { KEY_NP8, "Numpad Eight" };
    win32_keycodes[VK_NUMPAD9]    = { KEY_NP9, "Numpad Nine" };
    win32_keycodes[VK_MULTIPLY]   = { KEY_NP_MUL, "Numpad Multiply" };
    win32_keycodes[VK_ADD]        = { KEY_NP_ADD, "Numpad Add" };
    win32_keycodes[VK_SEPARATOR]  = { KEY_NP_SEP, "Numpad Seperator" };
    win32_keycodes[VK_SUBTRACT]   = { KEY_NP_SUB, "Numpad Subtract" };
    win32_keycodes[VK_DECIMAL]    = { KEY_NP_DEC, "Numpad Decimal" };
    win32_keycodes[VK_DIVIDE]     = { KEY_NP_DIV, "Numpad Divide" };
    win32_keycodes[VK_F1]         = { KEY_F1, "F1" };
    win32_keycodes[VK_F2]         = { KEY_F2, "F2" };
    win32_keycodes[VK_F3]         = { KEY_F3, "F3" };
    win32_keycodes[VK_F4]         = { KEY_F4, "F4" };
    win32_keycodes[VK_F5]         = { KEY_F5, "F5" };
    win32_keycodes[VK_F6]         = { KEY_F6, "F6" };
    win32_keycodes[VK_F7]         = { KEY_F7, "F7" };
    win32_keycodes[VK_F8]         = { KEY_F8, "F8" };
    win32_keycodes[VK_F9]         = { KEY_F9, "F9" };
    win32_keycodes[VK_F10]        = { KEY_F10, "F10" };
    win32_keycodes[VK_F11]        = { KEY_F11, "F11" };
    win32_keycodes[VK_F12]        = { KEY_F12, "F12" };
    win32_keycodes[VK_F13]        = { KEY_F13, "F13" };
    win32_keycodes[VK_F14]        = { KEY_F14, "F14" };
    win32_keycodes[VK_F15]        = { KEY_F15, "F15" };
    win32_keycodes[VK_F16]        = { KEY_F16, "F16" };
    win32_keycodes[VK_F17]        = { KEY_F17, "F17" };
    win32_keycodes[VK_F18]        = { KEY_F18, "F18" };
    win32_keycodes[VK_F19]        = { KEY_F19, "F19" };
    win32_keycodes[VK_F20]        = { KEY_F20, "F20" };
    win32_keycodes[VK_F21]        = { KEY_F21, "F21" };
    win32_keycodes[VK_F22]        = { KEY_F22, "F22" };
    win32_keycodes[VK_F23]        = { KEY_F23, "F23" };
    win32_keycodes[VK_F24]        = { KEY_F24, "F24" };
    win32_keycodes[VK_NUMLOCK]    = { KEY_NUMLOCK, "Num lock" };
    win32_keycodes[VK_SNAPSHOT]   = { KEY_PRINT, "Print" };
    win32_keycodes[VK_SCROLL]     = { KEY_SCROLLLOCK, "Scroll lock" };
    win32_keycodes[VK_PAUSE]      = { KEY_PAUSE, "Pause" };
    win32_keycodes[VK_LSHIFT]     = { KEY_SHIFT, "Shift" };
    win32_keycodes[VK_RSHIFT]     = { KEY_SHIFT, "Shift" };
    win32_keycodes[VK_LCONTROL]   = { KEY_CTRL, "Control" };
    win32_keycodes[VK_RCONTROL]   = { KEY_CTRL, "Control" };
    win32_keycodes[VK_LMENU]      = { KEY_ALT, "Alt" };
    win32_keycodes[VK_RMENU]      = { KEY_ALT, "Alt" };
    win32_keycodes[VK_OEM_1]      = { KEY_OEM1, "Keyboard specific 1" };
    win32_keycodes[VK_OEM_2]      = { KEY_OEM2, "Keyboard specific 2" };
    win32_keycodes[VK_OEM_3]      = { KEY_OEM3, "Keyboard specific 3" };
    win32_keycodes[VK_OEM_4]      = { KEY_OEM4, "Keyboard specific 4" };
    win32_keycodes[VK_OEM_5]      = { KEY_OEM5, "Keyboard specific 5" };
    win32_keycodes[VK_OEM_6]      = { KEY_OEM6, "Keyboard specific 6" };
    win32_keycodes[VK_OEM_7]      = { KEY_OEM7, "Keyboard specific 7" };
    win32_keycodes[VK_OEM_8]      = { KEY_OEM8, "Keyboard specific 8" };
    win32_keycodes[VK_OEM_102]    = { KEY_OEM9, "Keyboard specific 9" };
    win32_keycodes[VK_OEM_PLUS]   = { KEY_PLUS, "Plus" };
    win32_keycodes[VK_OEM_COMMA]  = { KEY_COMMA, "Comma" };
    win32_keycodes[VK_OEM_MINUS]  = { KEY_MINUS, "Minus" };
    win32_keycodes[VK_OEM_PERIOD] = { KEY_PERIOD, "Period" };
  }

  key_descriptor os_to_key(int os_key) noexcept {
    // TODO REMOVE AND DO IT IN E.G. winproc: NCCREATE
    load_win32_keys();

    if (os_key < 0 || os_key > sizeof(win32_keycodes) / sizeof(win32_keycodes[0])) {
      return win32_keycodes[0];
    }

    return win32_keycodes[os_key];
  }

  button_descriptor win32_buttoncodes[VK_XBUTTON2 + 1] = { { BUTTON_COUNT, "ERROR" } };

  static void load_win32_buttons() noexcept {
    if (win32_buttoncodes[0].b != BUTTON_COUNT)
      return;

    win32_buttoncodes[0]           = { BUTTON_NONE, "None" };
    win32_buttoncodes[VK_LBUTTON]  = { BUTTON_LEFT, "Left Mouse" };
    win32_buttoncodes[VK_RBUTTON]  = { BUTTON_RIGHT, "Right Mouse" };
    win32_buttoncodes[VK_MBUTTON]  = { BUTTON_MIDDLE, "Middle Mouse" };
    win32_buttoncodes[VK_XBUTTON1] = { BUTTON_X1, "X1 Mouse" };
    win32_buttoncodes[VK_XBUTTON2] = { BUTTON_X2, "X2 Mouse" };
  }

  button_descriptor os_to_button(int os_button) noexcept {
    // TODO REMOVE AND DO IT IN E.G. winproc: NCCREATE
    load_win32_buttons();

    if (os_button < 0 || os_button > sizeof(win32_buttoncodes) / sizeof(win32_buttoncodes[0])) {
      return win32_buttoncodes[0];
    }

    return win32_buttoncodes[os_button];
  }
}
