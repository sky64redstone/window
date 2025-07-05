#include "platform.hpp"

#ifdef window_wl

  namespace window::wl {
    ::window::result create_window(window_wl_data& data, int w, int h, const char* title) noexcept;
    ::window::result set_title(const window_wl_data& data, const char* title) noexcept;
    ::window::result set_size(window_wl_data& data, int w, int h) noexcept;
    ::window::result poll_events(window_wl_data& data) noexcept;
    ::window::result make_opengl_context(window_wl_data& data) noexcept;
    ::window::result swap_buffers(const window_wl_data& data) noexcept;
    ::window::result swap_interval(const window_wl_data& data, int interval) noexcept;
    void destroy(window_wl_data& data) noexcept;
  }

#endif
