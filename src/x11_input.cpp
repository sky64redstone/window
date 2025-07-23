#include <X11/keysymdef.h>

#include "include/x11_input.hpp"
#include "include/linux_input.hpp"

namespace window {
  void load_x11_keys() noexcept {
    if (x11_key_converters[0].descriptor.k == KEY_NONE) {
      return;
    }

    x11_key_converters[0] = { 0, { KEY_NONE, "None" } };

    for (int i = 1; i < sizeof(x11_key_converters) / sizeof(x11_key_converters[0]); ++i) {
      x11_key_converters[i] = { 0, { KEY_NONE, "Unknown" } };
    }

    x11_key_converters[1] = { XK_a, { KEY_A, "A" } };
    x11_key_converters[2] = { XK_b, { KEY_B, "B" } };
    x11_key_converters[3] = { XK_c, { KEY_C, "C" } };
    x11_key_converters[4] = { XK_d, { KEY_D, "D" } };
    x11_key_converters[5] = { XK_e, { KEY_E, "E" } };
    x11_key_converters[6] = { XK_f, { KEY_F, "F" } };
    x11_key_converters[7] = { XK_g, { KEY_G, "G" } };
    x11_key_converters[8] = { XK_h, { KEY_H, "H" } };
    x11_key_converters[9] = { XK_i, { KEY_I, "I" } };
    x11_key_converters[10] = { XK_j, { KEY_J, "J" } };
    x11_key_converters[11] = { XK_k, { KEY_K, "K" } };
    x11_key_converters[12] = { XK_l, { KEY_L, "L" } };
    x11_key_converters[13] = { XK_m, { KEY_M, "M" } };
    x11_key_converters[14] = { XK_n, { KEY_N, "N" } };
    x11_key_converters[15] = { XK_o, { KEY_O, "O" } };
    x11_key_converters[16] = { XK_p, { KEY_P, "P" } };
    x11_key_converters[17] = { XK_q, { KEY_Q, "Q" } };
    x11_key_converters[18] = { XK_r, { KEY_R, "R" } };
    x11_key_converters[19] = { XK_s, { KEY_S, "S" } };
    x11_key_converters[20] = { XK_t, { KEY_T, "T" } };
    x11_key_converters[21] = { XK_u, { KEY_U, "U" } };
    x11_key_converters[22] = { XK_v, { KEY_V, "V" } };
    x11_key_converters[23] = { XK_w, { KEY_W, "W" } };
    x11_key_converters[24] = { XK_x, { KEY_X, "X" } };
    x11_key_converters[25] = { XK_y, { KEY_Y, "Y" } };
    x11_key_converters[26] = { XK_z, { KEY_Z, "Z" } };

    x11_key_converters[27] = { XK_0, { KEY_0, "Zero" } };
    x11_key_converters[28] = { XK_1, { KEY_1, "One" } };
    x11_key_converters[29] = { XK_2, { KEY_2, "Two" } };
    x11_key_converters[30] = { XK_3, { KEY_3, "Three" } };
    x11_key_converters[31] = { XK_4, { KEY_4, "Four" } };
    x11_key_converters[32] = { XK_5, { KEY_5, "Five" } };
    x11_key_converters[33] = { XK_6, { KEY_6, "Six" } };
    x11_key_converters[34] = { XK_7, { KEY_7, "Seven" } };
    x11_key_converters[35] = { XK_8, { KEY_8, "Eight" } };
    x11_key_converters[36] = { XK_9, { KEY_9, "Nine" } };

    x11_key_converters[37] = { XK_KP_0, { KEY_NP0, "Numpad Zero" } };
    x11_key_converters[38] = { XK_KP_1, { KEY_NP1, "Numpad One" } };
    x11_key_converters[39] = { XK_KP_2, { KEY_NP2, "Numpad Two" } };
    x11_key_converters[40] = { XK_KP_3, { KEY_NP3, "Numpad Three" } };
    x11_key_converters[41] = { XK_KP_4, { KEY_NP4, "Numpad Four" } };
    x11_key_converters[42] = { XK_KP_5, { KEY_NP5, "Numpad Five" } };
    x11_key_converters[43] = { XK_KP_6, { KEY_NP6, "Numpad Six" } };
    x11_key_converters[44] = { XK_KP_7, { KEY_NP7, "Numpad Seven" } };
    x11_key_converters[45] = { XK_KP_8, { KEY_NP8, "Numpad Eight" } };
    x11_key_converters[46] = { XK_KP_9, { KEY_NP9, "Numpad Nine" } };

    x11_key_converters[47] = { XK_KP_Insert, { KEY_NP0, "Numpad Zero" } };
    x11_key_converters[48] = { XK_KP_End, { KEY_NP1, "Numpad One" } };
    x11_key_converters[49] = { XK_KP_Down, { KEY_NP2, "Numpad Two" } };
    x11_key_converters[50] = { XK_KP_Next, { KEY_NP3, "Numpad Three" } };
    x11_key_converters[51] = { XK_KP_Left, { KEY_NP4, "Numpad Four" } };
    x11_key_converters[52] = { XK_KP_Begin, { KEY_NP5, "Numpad Five" } };
    x11_key_converters[53] = { XK_KP_Right, { KEY_NP6, "Numpad Six" } };
    x11_key_converters[54] = { XK_KP_Home, { KEY_NP7, "Numpad Seven" } };
    x11_key_converters[55] = { XK_KP_Up, { KEY_NP8, "Numpad Eight" } };
    x11_key_converters[56] = { XK_KP_Prior, { KEY_NP9, "Numpad Nine" } };
    
    x11_key_converters[57] = { XK_Shift_L, { KEY_SHIFT, "Shift" } };
    x11_key_converters[58] = { XK_Shift_R, { KEY_SHIFT, "Shift" } };
    x11_key_converters[59] = { XK_Control_L, { KEY_CTRL, "Control" } };
    x11_key_converters[60] = { XK_Control_R, { KEY_CTRL, "Control" } };
    x11_key_converters[61] = { XK_Alt_L, { KEY_ALT, "Alt" } };
    x11_key_converters[62] = { XK_Alt_R, { KEY_ALT, "Alt" } };
    x11_key_converters[63] = { XK_Super_L, { KEY_SUPER, "Super" } };
    x11_key_converters[64] = { XK_Super_R, { KEY_SUPER, "Super" } };
    x11_key_converters[65] = { XK_Caps_Lock, { KEY_CAPSLOCK, "Capslock" } };
    x11_key_converters[66] = { XK_BackSpace, { KEY_BACKSPACE, "Backspace" } };
    x11_key_converters[67] = { XK_Tab, { KEY_TAB, "Tab" } };
    x11_key_converters[68] = { XK_Return, { KEY_RETURN, "Return" } };
    x11_key_converters[69] = { XK_space, { KEY_SPACE, "Space" } };
    x11_key_converters[70] = { XK_Escape, { KEY_ESCAPE, "Escape" } };
    x11_key_converters[71] = { XK_Left, { KEY_LEFT, "Left Arrow" } };
    x11_key_converters[72] = { XK_Right, { KEY_RIGHT, "Right Arrow" } };
    x11_key_converters[73] = { XK_Up, { KEY_UP, "Up Arrow" } };
    x11_key_converters[74] = { XK_Down, { KEY_DOWN, "Down Arrow" } };

    x11_key_converters[75] = { XK_F1, { KEY_F1, "F1" } };
    x11_key_converters[76] = { XK_F2, { KEY_F2, "F2" } };
    x11_key_converters[77] = { XK_F3, { KEY_F3, "F3" } };
    x11_key_converters[78] = { XK_F4, { KEY_F4, "F4" } };
    x11_key_converters[79] = { XK_F5, { KEY_F5, "F5" } };
    x11_key_converters[80] = { XK_F6, { KEY_F6, "F6" } };
    x11_key_converters[81] = { XK_F7, { KEY_F7, "F7" } };
    x11_key_converters[82] = { XK_F8, { KEY_F8, "F8" } };
    x11_key_converters[83] = { XK_F9, { KEY_F9, "F9" } };
    x11_key_converters[84] = { XK_F10, { KEY_F10, "F10" } };
    x11_key_converters[85] = { XK_F11, { KEY_F11, "F11" } };
    x11_key_converters[86] = { XK_F12, { KEY_F12, "F12" } };
    x11_key_converters[87] = { XK_F13, { KEY_F13, "F13" } };
    x11_key_converters[88] = { XK_F14, { KEY_F14, "F14" } };
    x11_key_converters[89] = { XK_F15, { KEY_F15, "F15" } };
    x11_key_converters[90] = { XK_F16, { KEY_F16, "F16" } };
    x11_key_converters[91] = { XK_F17, { KEY_F17, "F17" } };
    x11_key_converters[92] = { XK_F18, { KEY_F18, "F18" } };
    x11_key_converters[93] = { XK_F19, { KEY_F19, "F19" } };
    x11_key_converters[94] = { XK_F20, { KEY_F20, "F20" } };
    x11_key_converters[95] = { XK_F21, { KEY_F21, "F21" } };
    x11_key_converters[96] = { XK_F22, { KEY_F22, "F22" } };
    x11_key_converters[97] = { XK_F23, { KEY_F23, "F23" } };
    x11_key_converters[98] = { XK_F24, { KEY_F24, "F24" } };

    x11_key_converters[99]  = { XK_Insert,      { KEY_INSERT, "Insert" } };
    x11_key_converters[100] = { XK_Delete,      { KEY_DELETE, "Delete" } };
    x11_key_converters[101] = { XK_Home,        { KEY_HOME, "Home" } };
    x11_key_converters[102] = { XK_End,         { KEY_END, "End" } };
    x11_key_converters[103] = { XK_Page_Up,     { KEY_PAGE_UP, "Page Up" } };
    x11_key_converters[104] = { XK_Page_Down,   { KEY_PAGE_DOWN, "Page Down" } };
    x11_key_converters[105] = { XK_Num_Lock,    { KEY_NUMLOCK, "Num Lock" } };
    x11_key_converters[106] = { XK_Print,       { KEY_PRINT, "Print Screen" } };
    x11_key_converters[107] = { XK_Scroll_Lock, { KEY_SCROLLLOCK, "Scroll Lock" } };
    x11_key_converters[108] = { XK_Pause,       { KEY_PAUSE, "Pause" } };

    x11_key_converters[109] = { XK_plus,        { KEY_PLUS, "Plus" } };
    x11_key_converters[110] = { XK_comma,       { KEY_COMMA, "Comma" } };
    x11_key_converters[111] = { XK_minus,       { KEY_MINUS, "Minus" } };
    x11_key_converters[112] = { XK_period,      { KEY_PERIOD, "Period" } };

    x11_key_converters[113] = { XK_semicolon,    { KEY_OEM1, "Keyboard specific 1" } };
    x11_key_converters[114] = { XK_slash,        { KEY_OEM2, "Keyboard specific 2" } };
    x11_key_converters[115] = { XK_grave,        { KEY_OEM3, "Keyboard specific 3" } };
    x11_key_converters[116] = { XK_bracketleft,  { KEY_OEM4, "Keyboard specific 4" } };
    x11_key_converters[117] = { XK_backslash,    { KEY_OEM5, "Keyboard specific 5" } };
    x11_key_converters[118] = { XK_bracketright, { KEY_OEM6, "Keyboard specific 6" } };
    x11_key_converters[119] = { XK_apostrophe,   { KEY_OEM7, "Keyboard specific 7" } };
    x11_key_converters[120] = { XK_section,      { KEY_OEM8, "Keyboard specific 8" } };
    x11_key_converters[121] = { XK_less,         { KEY_OEM9, "Keyboard specific 9" } };

    // those keys are sometimes used for Right Alt as well...
    x11_key_converters[122] = { XK_Mode_switch, { KEY_ALT, "Alt" } };
    x11_key_converters[123] = { XK_ISO_Level3_Shift, { KEY_ALT, "Alt" } };
    x11_key_converters[124] = { XK_Meta_R, { KEY_ALT, "Alt" } };
  }

  void load_x11_buttons() noexcept {
    if (x11_buttoncodes[0].b == BUTTON_NONE) {
      return;
    }

    x11_buttoncodes[0] = { BUTTON_NONE, "None" };
    x11_buttoncodes[1] = { BUTTON_NONE, "Left Mouse" };
    x11_buttoncodes[2] = { BUTTON_NONE, "Right Mouse" };
    x11_buttoncodes[3] = { BUTTON_NONE, "Middle Mouse" };
    x11_buttoncodes[4] = { BUTTON_NONE, "Scroll Up" };
    x11_buttoncodes[5] = { BUTTON_NONE, "Scroll Down" };
    x11_buttoncodes[6] = { BUTTON_NONE, "Scroll Left" };
    x11_buttoncodes[7] = { BUTTON_NONE, "Scroll Right" };
    x11_buttoncodes[8] = { BUTTON_NONE, "X1 Mouse" };
    x11_buttoncodes[9] = { BUTTON_NONE, "X2 Mouse" };
  }
}
