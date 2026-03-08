#include "include/input.hpp"
#include "include/platform.hpp"

namespace window {
  #if defined(window_x11) && defined(window_wl)
    void set_x11(bool x11) noexcept;
  #endif
}
