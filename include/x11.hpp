#include "platform.hpp"

#ifdef window_x11

  namespace window::x11 {
    ::window::result create_window(window_x11_data& data, int w, int h, const char* title) noexcept;
    ::window::result set_title(const window_x11_data& data, const char* title) noexcept;
    ::window::result set_size(const window_x11_data& data, int w, int h) noexcept;
    ::window::result poll_events(window_x11_data& data) noexcept;
    ::window::result make_opengl_context(window_x11_data& data) noexcept;
    ::window::result swap_buffers(const window_x11_data& data) noexcept;
    ::window::result swap_interval(const window_x11_data& data, int interval) noexcept;
    void destroy(window_x11_data& data) noexcept;
  }

#endif
