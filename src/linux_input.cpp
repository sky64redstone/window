#include "include/linux_input.hpp"

#ifdef window_x11
  #include <X11/keysymdef.h>
#endif

#ifdef window_wl
  #if !defined(window_x11)
    #include <xkbcommon/xkbcommon-keysyms.h>
  #endif
#endif

namespace window {
  // keep this before the #include <linux/input-event-codes.h>
  // because the header defines macros like KEY_A :(
  struct key_converter {
    int native;
    key_descriptor descriptor;
  };

  #ifdef window_x11
    inline key_converter key_converters[] = {
      { 0, { KEY_NONE, "None" } },

      { XK_a, { KEY_A, "A" } },
      { XK_b, { KEY_B, "B" } },
      { XK_c, { KEY_C, "C" } },
      { XK_d, { KEY_D, "D" } },
      { XK_e, { KEY_E, "E" } },
      { XK_f, { KEY_F, "F" } },
      { XK_g, { KEY_G, "G" } },
      { XK_h, { KEY_H, "H" } },
      { XK_i, { KEY_I, "I" } },
      { XK_j, { KEY_J, "J" } },
      { XK_k, { KEY_K, "K" } },
      { XK_l, { KEY_L, "L" } },
      { XK_m, { KEY_M, "M" } },
      { XK_n, { KEY_N, "N" } },
      { XK_o, { KEY_O, "O" } },
      { XK_p, { KEY_P, "P" } },
      { XK_q, { KEY_Q, "Q" } },
      { XK_r, { KEY_R, "R" } },
      { XK_s, { KEY_S, "S" } },
      { XK_t, { KEY_T, "T" } },
      { XK_u, { KEY_U, "U" } },
      { XK_v, { KEY_V, "V" } },
      { XK_w, { KEY_W, "W" } },
      { XK_x, { KEY_X, "X" } },
      { XK_y, { KEY_Y, "Y" } },
      { XK_z, { KEY_Z, "Z" } },

      { XK_0, { KEY_0, "Zero" } },
      { XK_1, { KEY_1, "One" } },
      { XK_2, { KEY_2, "Two" } },
      { XK_3, { KEY_3, "Three" } },
      { XK_4, { KEY_4, "Four" } },
      { XK_5, { KEY_5, "Five" } },
      { XK_6, { KEY_6, "Six" } },
      { XK_7, { KEY_7, "Seven" } },
      { XK_8, { KEY_8, "Eight" } },
      { XK_9, { KEY_9, "Nine" } },

      { XK_KP_0, { KEY_NP0, "Numpad Zero" } },
      { XK_KP_1, { KEY_NP1, "Numpad One" } },
      { XK_KP_2, { KEY_NP2, "Numpad Two" } },
      { XK_KP_3, { KEY_NP3, "Numpad Three" } },
      { XK_KP_4, { KEY_NP4, "Numpad Four" } },
      { XK_KP_5, { KEY_NP5, "Numpad Five" } },
      { XK_KP_6, { KEY_NP6, "Numpad Six" } },
      { XK_KP_7, { KEY_NP7, "Numpad Seven" } },
      { XK_KP_8, { KEY_NP8, "Numpad Eight" } },
      { XK_KP_9, { KEY_NP9, "Numpad Nine" } },

      { XK_KP_Insert, { KEY_NP0, "Numpad Zero" } },
      { XK_KP_End,    { KEY_NP1, "Numpad One" } },
      { XK_KP_Down,   { KEY_NP2, "Numpad Two" } },
      { XK_KP_Next,   { KEY_NP3, "Numpad Three" } },
      { XK_KP_Left,   { KEY_NP4, "Numpad Four" } },
      { XK_KP_Begin,  { KEY_NP5, "Numpad Five" } },
      { XK_KP_Right,  { KEY_NP6, "Numpad Six" } },
      { XK_KP_Home,   { KEY_NP7, "Numpad Seven" } },
      { XK_KP_Up,     { KEY_NP8, "Numpad Eight" } },
      { XK_KP_Prior,  { KEY_NP9, "Numpad Nine" } },

      { XK_Shift_L,   { KEY_SHIFT, "Shift" } },
      { XK_Shift_R,   { KEY_SHIFT, "Shift" } },
      { XK_Control_L, { KEY_CTRL, "Control" } },
      { XK_Control_R, { KEY_CTRL, "Control" } },
      { XK_Alt_L,     { KEY_ALT, "Alt" } },
      { XK_Alt_R,     { KEY_ALT, "Alt" } },
      { XK_Super_L,   { KEY_SUPER, "Super" } },
      { XK_Super_R,   { KEY_SUPER, "Super" } },
      { XK_Caps_Lock, { KEY_CAPSLOCK, "Capslock" } },
      { XK_BackSpace, { KEY_BACKSPACE, "Backspace" } },
      { XK_Tab,       { KEY_TAB, "Tab" } },
      { XK_Return,    { KEY_RETURN, "Return" } },
      { XK_space,     { KEY_SPACE, "Space" } },
      { XK_Escape,    { KEY_ESCAPE, "Escape" } },
      { XK_Left,      { KEY_LEFT, "Left Arrow" } },
      { XK_Right,     { KEY_RIGHT, "Right Arrow" } },
      { XK_Up,        { KEY_UP, "Up Arrow" } },
      { XK_Down,      { KEY_DOWN, "Down Arrow" } },

      { XK_F1,  { KEY_F1, "F1" } },
      { XK_F2,  { KEY_F2, "F2" } },
      { XK_F3,  { KEY_F3, "F3" } },
      { XK_F4,  { KEY_F4, "F4" } },
      { XK_F5,  { KEY_F5, "F5" } },
      { XK_F6,  { KEY_F6, "F6" } },
      { XK_F7,  { KEY_F7, "F7" } },
      { XK_F8,  { KEY_F8, "F8" } },
      { XK_F9,  { KEY_F9, "F9" } },
      { XK_F10, { KEY_F10, "F10" } },
      { XK_F11, { KEY_F11, "F11" } },
      { XK_F12, { KEY_F12, "F12" } },
      { XK_F13, { KEY_F13, "F13" } },
      { XK_F14, { KEY_F14, "F14" } },
      { XK_F15, { KEY_F15, "F15" } },
      { XK_F16, { KEY_F16, "F16" } },
      { XK_F17, { KEY_F17, "F17" } },
      { XK_F18, { KEY_F18, "F18" } },
      { XK_F19, { KEY_F19, "F19" } },
      { XK_F20, { KEY_F20, "F20" } },
      { XK_F21, { KEY_F21, "F21" } },
      { XK_F22, { KEY_F22, "F22" } },
      { XK_F23, { KEY_F23, "F23" } },
      { XK_F24, { KEY_F24, "F24" } },

      { XK_Insert,      { KEY_INSERT, "Insert" } },
      { XK_Delete,      { KEY_DELETE, "Delete" } },
      { XK_Home,        { KEY_HOME, "Home" } },
      { XK_End,         { KEY_END, "End" } },
      { XK_Page_Up,     { KEY_PAGE_UP, "Page Up" } },
      { XK_Page_Down,   { KEY_PAGE_DOWN, "Page Down" } },
      { XK_Num_Lock,    { KEY_NUMLOCK, "Num Lock" } },
      { XK_Print,       { KEY_PRINT, "Print Screen" } },
      { XK_Scroll_Lock, { KEY_SCROLLLOCK, "Scroll Lock" } },
      { XK_Pause,       { KEY_PAUSE, "Pause" } },

      { XK_plus,   { KEY_PLUS, "Plus" } },
      { XK_comma,  { KEY_COMMA, "Comma" } },
      { XK_minus,  { KEY_MINUS, "Minus" } },
      { XK_period, { KEY_PERIOD, "Period" } },

      { XK_semicolon,    { KEY_OEM1, "Keyboard specific 1" } },
      { XK_slash,        { KEY_OEM2, "Keyboard specific 2" } },
      { XK_grave,        { KEY_OEM3, "Keyboard specific 3" } },
      { XK_bracketleft,  { KEY_OEM4, "Keyboard specific 4" } },
      { XK_backslash,    { KEY_OEM5, "Keyboard specific 5" } },
      { XK_bracketright, { KEY_OEM6, "Keyboard specific 6" } },
      { XK_apostrophe,   { KEY_OEM7, "Keyboard specific 7" } },
      { XK_section,      { KEY_OEM8, "Keyboard specific 8" } },
      { XK_less,         { KEY_OEM9, "Keyboard specific 9" } },

      { XK_Mode_switch,      { KEY_ALT, "Alt" } },
      { XK_ISO_Level3_Shift, { KEY_ALT, "Alt" } },
      { XK_Meta_R,           { KEY_ALT, "Alt" } }
    };
  #else
    #ifdef window_wl // should be set, if window_x11 is unset
      inline key_converter key_converters[] = {
        { 0, { KEY_NONE, "None" } },

        { XKB_KEY_a, { KEY_A, "A" } },
        { XKB_KEY_b, { KEY_B, "B" } },
        { XKB_KEY_c, { KEY_C, "C" } },
        { XKB_KEY_d, { KEY_D, "D" } },
        { XKB_KEY_e, { KEY_E, "E" } },
        { XKB_KEY_f, { KEY_F, "F" } },
        { XKB_KEY_g, { KEY_G, "G" } },
        { XKB_KEY_h, { KEY_H, "H" } },
        { XKB_KEY_i, { KEY_I, "I" } },
        { XKB_KEY_j, { KEY_J, "J" } },
        { XKB_KEY_k, { KEY_K, "K" } },
        { XKB_KEY_l, { KEY_L, "L" } },
        { XKB_KEY_m, { KEY_M, "M" } },
        { XKB_KEY_n, { KEY_N, "N" } },
        { XKB_KEY_o, { KEY_O, "O" } },
        { XKB_KEY_p, { KEY_P, "P" } },
        { XKB_KEY_q, { KEY_Q, "Q" } },
        { XKB_KEY_r, { KEY_R, "R" } },
        { XKB_KEY_s, { KEY_S, "S" } },
        { XKB_KEY_t, { KEY_T, "T" } },
        { XKB_KEY_u, { KEY_U, "U" } },
        { XKB_KEY_v, { KEY_V, "V" } },
        { XKB_KEY_w, { KEY_W, "W" } },
        { XKB_KEY_x, { KEY_X, "X" } },
        { XKB_KEY_y, { KEY_Y, "Y" } },
        { XKB_KEY_z, { KEY_Z, "Z" } },

        { XKB_KEY_0, { KEY_0, "Zero" } },
        { XKB_KEY_1, { KEY_1, "One" } },
        { XKB_KEY_2, { KEY_2, "Two" } },
        { XKB_KEY_3, { KEY_3, "Three" } },
        { XKB_KEY_4, { KEY_4, "Four" } },
        { XKB_KEY_5, { KEY_5, "Five" } },
        { XKB_KEY_6, { KEY_6, "Six" } },
        { XKB_KEY_7, { KEY_7, "Seven" } },
        { XKB_KEY_8, { KEY_8, "Eight" } },
        { XKB_KEY_9, { KEY_9, "Nine" } },

        { XKB_KEY_KP_0, { KEY_NP0, "Numpad Zero" } },
        { XKB_KEY_KP_1, { KEY_NP1, "Numpad One" } },
        { XKB_KEY_KP_2, { KEY_NP2, "Numpad Two" } },
        { XKB_KEY_KP_3, { KEY_NP3, "Numpad Three" } },
        { XKB_KEY_KP_4, { KEY_NP4, "Numpad Four" } },
        { XKB_KEY_KP_5, { KEY_NP5, "Numpad Five" } },
        { XKB_KEY_KP_6, { KEY_NP6, "Numpad Six" } },
        { XKB_KEY_KP_7, { KEY_NP7, "Numpad Seven" } },
        { XKB_KEY_KP_8, { KEY_NP8, "Numpad Eight" } },
        { XKB_KEY_KP_9, { KEY_NP9, "Numpad Nine" } },

        { XKB_KEY_KP_Insert, { KEY_NP0, "Numpad Zero" } },
        { XKB_KEY_KP_End,    { KEY_NP1, "Numpad One" } },
        { XKB_KEY_KP_Down,   { KEY_NP2, "Numpad Two" } },
        { XKB_KEY_KP_Next,   { KEY_NP3, "Numpad Three" } },
        { XKB_KEY_KP_Left,   { KEY_NP4, "Numpad Four" } },
        { XKB_KEY_KP_Begin,  { KEY_NP5, "Numpad Five" } },
        { XKB_KEY_KP_Right,  { KEY_NP6, "Numpad Six" } },
        { XKB_KEY_KP_Home,   { KEY_NP7, "Numpad Seven" } },
        { XKB_KEY_KP_Up,     { KEY_NP8, "Numpad Eight" } },
        { XKB_KEY_KP_Prior,  { KEY_NP9, "Numpad Nine" } },

        { XKB_KEY_Shift_L,   { KEY_SHIFT, "Shift" } },
        { XKB_KEY_Shift_R,   { KEY_SHIFT, "Shift" } },
        { XKB_KEY_Control_L, { KEY_CTRL, "Control" } },
        { XKB_KEY_Control_R, { KEY_CTRL, "Control" } },
        { XKB_KEY_Alt_L,     { KEY_ALT, "Alt" } },
        { XKB_KEY_Alt_R,     { KEY_ALT, "Alt" } },
        { XKB_KEY_Super_L,   { KEY_SUPER, "Super" } },
        { XKB_KEY_Super_R,   { KEY_SUPER, "Super" } },

        { XKB_KEY_Caps_Lock, { KEY_CAPSLOCK, "Capslock" } },
        { XKB_KEY_BackSpace, { KEY_BACKSPACE, "Backspace" } },
        { XKB_KEY_Tab,       { KEY_TAB, "Tab" } },
        { XKB_KEY_Return,    { KEY_RETURN, "Return" } },
        { XKB_KEY_space,     { KEY_SPACE, "Space" } },
        { XKB_KEY_Escape,    { KEY_ESCAPE, "Escape" } },

        { XKB_KEY_Left,  { KEY_LEFT, "Left Arrow" } },
        { XKB_KEY_Right, { KEY_RIGHT, "Right Arrow" } },
        { XKB_KEY_Up,    { KEY_UP, "Up Arrow" } },
        { XKB_KEY_Down,  { KEY_DOWN, "Down Arrow" } },

        { XKB_KEY_F1,  { KEY_F1, "F1" } },
        { XKB_KEY_F2,  { KEY_F2, "F2" } },
        { XKB_KEY_F3,  { KEY_F3, "F3" } },
        { XKB_KEY_F4,  { KEY_F4, "F4" } },
        { XKB_KEY_F5,  { KEY_F5, "F5" } },
        { XKB_KEY_F6,  { KEY_F6, "F6" } },
        { XKB_KEY_F7,  { KEY_F7, "F7" } },
        { XKB_KEY_F8,  { KEY_F8, "F8" } },
        { XKB_KEY_F9,  { KEY_F9, "F9" } },
        { XKB_KEY_F10, { KEY_F10, "F10" } },
        { XKB_KEY_F11, { KEY_F11, "F11" } },
        { XKB_KEY_F12, { KEY_F12, "F12" } },

        { XKB_KEY_Insert,      { KEY_INSERT, "Insert" } },
        { XKB_KEY_Delete,      { KEY_DELETE, "Delete" } },
        { XKB_KEY_Home,        { KEY_HOME, "Home" } },
        { XKB_KEY_End,         { KEY_END, "End" } },
        { XKB_KEY_Page_Up,     { KEY_PAGE_UP, "Page Up" } },
        { XKB_KEY_Page_Down,   { KEY_PAGE_DOWN, "Page Down" } },

        { XKB_KEY_Num_Lock,    { KEY_NUMLOCK, "Num Lock" } },
        { XKB_KEY_Print,       { KEY_PRINT, "Print Screen" } },
        { XKB_KEY_Scroll_Lock, { KEY_SCROLLLOCK, "Scroll Lock" } },
        { XKB_KEY_Pause,       { KEY_PAUSE, "Pause" } },

        { XKB_KEY_plus,   { KEY_PLUS, "Plus" } },
        { XKB_KEY_comma,  { KEY_COMMA, "Comma" } },
        { XKB_KEY_minus,  { KEY_MINUS, "Minus" } },
        { XKB_KEY_period, { KEY_PERIOD, "Period" } },

        { XKB_KEY_semicolon,    { KEY_OEM1, "Keyboard specific 1" } },
        { XKB_KEY_slash,        { KEY_OEM2, "Keyboard specific 2" } },
        { XKB_KEY_grave,        { KEY_OEM3, "Keyboard specific 3" } },
        { XKB_KEY_bracketleft,  { KEY_OEM4, "Keyboard specific 4" } },
        { XKB_KEY_backslash,    { KEY_OEM5, "Keyboard specific 5" } },
        { XKB_KEY_bracketright, { KEY_OEM6, "Keyboard specific 6" } },
        { XKB_KEY_apostrophe,   { KEY_OEM7, "Keyboard specific 7" } },
        { XKB_KEY_section,      { KEY_OEM8, "Keyboard specific 8" } },
        { XKB_KEY_less,         { KEY_OEM9, "Keyboard specific 9" } },

        { XKB_KEY_Mode_switch,      { KEY_ALT, "Alt" } },
        { XKB_KEY_ISO_Level3_Shift, { KEY_ALT, "Alt" } },
        { XKB_KEY_Meta_R,           { KEY_ALT, "Alt" } }
      };
    #endif
  #endif
}

#ifdef window_wl
  #include <linux/input-event-codes.h>
#endif

namespace window {

  #if defined(window_x11) && defined(window_wl)
    inline bool input_x11 = false;

    void set_x11(bool x11) noexcept {
      input_x11 = x11;
    }
  #endif

  // after converting the keys from wayland with xkbcommon, they match x11 keysyms
  key_descriptor os_to_key(int os_key) noexcept {
    const int x11_size = sizeof(key_converters) / sizeof(key_converters[0]);
    for (int i = 0; i < x11_size; ++i) {
      if (key_converters[i].native == os_key) {
        return key_converters[i].descriptor;
      }
    }
    // TODO Custom keys
    // e.g. return key_descriptor{ KEY_COUNT + os_key, "Unknown" };
    return key_converters[0].descriptor;
  }

  #ifdef window_x11
    button_descriptor x11_os_to_button(int os_button) noexcept {
      switch (os_button) {
        case 1:  return { BUTTON_NONE, "Left Mouse" };
        case 2:  return { BUTTON_NONE, "Right Mouse" };
        case 3:  return { BUTTON_NONE, "Middle Mouse" };
        case 4:  return { BUTTON_NONE, "Scroll Up" };
        case 5:  return { BUTTON_NONE, "Scroll Down" };
        case 6:  return { BUTTON_NONE, "Scroll Left" };
        case 7:  return { BUTTON_NONE, "Scroll Right" };
        case 8:  return { BUTTON_NONE, "X1 Mouse" };
        case 9:  return { BUTTON_NONE, "X2 Mouse" };
        default: return { BUTTON_NONE, "None" };
      }
    }
  #endif

  #ifdef window_wl
    button_descriptor wl_os_to_button(int os_button) noexcept {
      switch (os_button) {
        case BTN_LEFT:   return { BUTTON_NONE, "Left Mouse" };
        case BTN_RIGHT:  return { BUTTON_NONE, "Right Mouse" };
        case BTN_MIDDLE: return { BUTTON_NONE, "Middle Mouse" };
        case BTN_SIDE:   return { BUTTON_NONE, "X1 Mouse" };
        case BTN_EXTRA:  return { BUTTON_NONE, "X2 Mouse" };
        default: return { BUTTON_NONE, "None" };
      }
    }
  #endif

  button_descriptor os_to_button(int os_button) noexcept {
    #if defined(window_wl) && defined(window_x11)
      if (input_x11) {
        return x11_os_to_button(os_button);
      }
    #endif

    #ifdef window_wl
      return wl_os_to_button(os_button);
    #endif

    #if defined(window_x11) && !defined(window_wl)
      return x11_os_to_button(os_button);
    #endif
  }
}
