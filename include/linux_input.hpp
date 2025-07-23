#include "include/input.hpp"
#include "include/platform.hpp"

namespace window {
  #if defined(window_x11) && defined(window_wl)
    void set_x11(bool x11) noexcept;
  #endif

  #ifdef window_x11
    struct x11_key_converter {
      KeySym native;
      key_descriptor descriptor;
    };

    inline x11_key_converter x11_key_converters[0xFF] = { { 0, { KEY_COUNT, "ERROR" } } };
  #endif

  #if defined(window_wl)
    inline key_descriptor wl_keycodes[] = { { KEY_COUNT, "ERROR" } };
  #endif

  void load_linux_keys() noexcept;

  #ifdef window_x11
    inline button_descriptor x11_buttoncodes[10] = { { BUTTON_COUNT, "ERROR" } };
  #endif

  #if defined(window_wl)
    inline button_descriptor wl_buttoncodes[] = { { BUTTON_COUNT, "ERROR" } };
  #endif
 
  void load_linux_buttons() noexcept;

}
