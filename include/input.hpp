#ifndef INPUT_HPP
  #define INPUT_HPP

  #define input_char(curr, prev) static_cast<char>(static_cast<bool>(curr) | (static_cast<bool>(prev) << 1))
  #define input_get_curr(data) static_cast<bool>(static_cast<char>(data) & 0b01)
  #define input_get_prev(data) static_cast<bool>(static_cast<char>(data) & 0b10)

  namespace window {

    enum key : char {
      KEY_NONE,

      // Alphabet keys
      KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G,
      KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N,
      KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U,
      KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

      // Number keys (top row)
      KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
      KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

      // Numpad number keys
      KEY_NP1, KEY_NP2, KEY_NP3, KEY_NP4, KEY_NP5,
      KEY_NP6, KEY_NP7, KEY_NP8, KEY_NP9, KEY_NP0,

      // Modifier keys
      KEY_SHIFT,
      KEY_CTRL,
      KEY_ALT,
      KEY_SUPER,     // Windows/Command key
      KEY_CAPSLOCK,

      // Special characters
      KEY_BACKSPACE,
      KEY_TAB,
      KEY_RETURN,
      KEY_SPACE,
      KEY_ESCAPE,

      // Numpad Symbols
      KEY_NP_MUL, // multiply key
      KEY_NP_ADD, // add key
      KEY_NP_SUB, // subtract key
      KEY_NP_DEC, // decimal key
      KEY_NP_SEP, // seperator key: locale-specific seperator (. or ,)
      KEY_NP_DIV, // divide key

      // Arrow keys
      KEY_LEFT,
      KEY_RIGHT,
      KEY_UP,
      KEY_DOWN,

      // Function keys
      KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
      KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10,
      KEY_F11, KEY_F12, KEY_F13, KEY_F14, KEY_F15,
      KEY_F16, KEY_F17, KEY_F18, KEY_F19, KEY_F20,
      KEY_F21, KEY_F22, KEY_F23, KEY_F24,

      // Other special keys
      KEY_INSERT,
      KEY_DELETE,
      KEY_HOME,
      KEY_END,
      KEY_PAGE_UP,
      KEY_PAGE_DOWN,
      KEY_NUMLOCK,
      KEY_PRINT,
      KEY_SCROLLLOCK,
      KEY_PAUSE,
      KEY_PLUS,
      KEY_COMMA,
      KEY_MINUS,
      KEY_PERIOD,

      // Keyboard specific keys
      // TODO remove, because of cross-platform in-compatible
      KEY_OEM1, // Keyboard specific: US: ;: DE: üÜ
      KEY_OEM2, // Keyboard specific: US: /? DE: #'
      KEY_OEM3, // Keyboard specific: US: `~ DE: öÖ
      KEY_OEM4, // Keyboard specific: US: [{ DE: ß?
      KEY_OEM5, // Keyboard specific: US: \| DE: ^°
      KEY_OEM6, // Keyboard specific: US: ]} DE: ´`
      KEY_OEM7, // Keyboard specific: US: '" DE: äÄ
      KEY_OEM8, // Keyboard specific: US: -- DE: --
      KEY_OEM9, // Keyboard specific: DE: <>|

      KEY_COUNT
    };

    struct key_descriptor {
      key k;
      const char* description;
    };

    key_descriptor os_to_key(int os_key) noexcept;

    enum button : char {
      BUTTON_NONE,
      BUTTON_LEFT,
      BUTTON_RIGHT,
      BUTTON_MIDDLE,
      BUTTON_X1,
      BUTTON_X2,

      BUTTON_COUNT
    };

    struct button_descriptor {
      button b;
      const char* description;
    };

    button_descriptor os_to_button(int os_button) noexcept;

    using key_event_callback    = void(*)(bool down, key_descriptor& k);
    using button_event_callback = void(*)(bool down, button_descriptor& b);
    using dblclk_event_callback = void(*)(button_descriptor& b);
    using mouse_event_callback  = void(*)(int x, int y);
    using wheel_event_callback  = void(*)(float delta);
    using size_event_callback   = void(*)(int w, int h);

    struct input_data {
      key_event_callback    key_event;
      button_event_callback button_event;
      dblclk_event_callback dblclk_event;
      mouse_event_callback  mouse_event;
      wheel_event_callback  wheel_event;
      size_event_callback   size_event;

      char keys[KEY_COUNT];

      char buttons[BUTTON_COUNT];

      int mouseX;
      int mouseY;

      float mouse_wheel;
    };
  }

#endif
