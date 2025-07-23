#include "include/linux_input.hpp"

#ifdef window_x11
  #include "include/x11_input.hpp"
#endif

#ifdef window_wl
  #include "include/wl_input.hpp"
#endif

namespace window {

  #if defined(window_x11) && defined(window_wl)
    inline bool input_x11 = false;

    void set_x11(bool x11) noexcept {
      input_x11 = x11;
    }
  #endif

  void load_linux_keys() noexcept {
    #ifdef window_x11
      load_x11_keys();
    #endif
    #ifdef window_wl
      load_wl_keys();
    #endif
  }

  void load_linux_buttons() noexcept {
    #ifdef window_x11
      load_x11_buttons();
    #endif
    #ifdef window_wl
      load_wl_buttons();
    #endif
  }

  key_descriptor os_to_key(int os_key) noexcept {
    #if defined(window_wl) && defined(window_x11)
      if (input_x11) {
        const int x11_size = sizeof(x11_key_converters) / sizeof(x11_key_converters[0]);
        load_x11_keys();
        for (int i = 0; i < x11_size; ++i) {
          if (x11_key_converters[i].native == os_key) {
            return x11_key_converters[i].descriptor;
          }
        }
        // TODO Custom keys
        // e.g. return key_descriptor{ KEY_COUNT + os_key, "Unknown" };
        // maybe replace the description with XLookupString?
        return x11_key_converters[0].descriptor;
      }
    #endif

    #ifdef window_wl
      const int wl_size = sizeof(wl_keycodes) / sizeof(wl_keycodes[0]);
      if (os_key > wl_size || os_key < 0) {
        os_key = 0;
      }
      load_wl_keys();
      return wl_keycodes[os_key];
    #endif

    #if defined(window_x11) && !defined(window_wl)
      const int x11_size = sizeof(x11_key_converters) / sizeof(x11_key_converters[0]);
      load_x11_keys();
      for (int i = 0; i < x11_size; ++i) {
        if (x11_key_converters[i].native == os_key) {
          return x11_key_converters[i].descriptor;
        }
      }
      // TODO Custom keys
      // e.g. return key_descriptor{ KEY_COUNT + os_key, "Unknown" };
      // maybe replace the description with XLookupString?
      return x11_key_converters[0].descriptor;
    #endif
  }

  button_descriptor os_to_button(int os_key) noexcept {
    #if defined(window_wl) && defined(window_x11)
      if (input_x11) {
        load_x11_buttons();
        return x11_buttoncodes[os_key];
      }
    #endif

    #ifdef window_wl
      load_wl_buttons();
      return wl_buttoncodes[os_key];
    #endif

    #if defined(window_x11) && !defined(window_wl)
      load_x11_buttons();
      return x11_buttoncodes[os_key];
    #endif
  }
}
