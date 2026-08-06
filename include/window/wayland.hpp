#if defined(window_wl) && !defined(WAYLAND_HPP)
  #define WAYLAND_HPP

  #include <wayland-client.h>
  #include <wayland-egl.h>
  #include <EGL/egl.h>
  #include <xkbcommon/xkbcommon.h>

  #include "xdg-shell-client-protocol.h"
  #include "xdg-decoration-client-protocol.h"

  #include "platform.hpp"

  struct window_wl_data {
    ::window::input_data* input;
    ::window::hint_data*  hintmem;
    ::window::framebuffer_data* fb;

    wl_display*    display;
    wl_compositor* compositor;
    wl_surface*    surface;
    wl_registry*   registry;
    wl_seat*       seat;
    wl_keyboard*   keyboard;
    wl_pointer*    pointer;

    xdg_wm_base*   xwm_base;
    xdg_surface*   xsurface;
    xdg_toplevel*  xtoplevel;

    xkb_context*   kb_ctx;
    xkb_keymap*    kb_keymap;
    xkb_state*     kb_state;

    zxdg_decoration_manager_v1* decoration_manager;
    zxdg_toplevel_decoration_v1* decoration;

    wl_egl_window* egl_window;
    EGLDisplay     egl_display;
    EGLConfig      egl_config;
    EGLContext     egl_context;
    EGLSurface     egl_surface;

    wl_shm*        shm;
    wl_shm_pool*   shm_pool;
    wl_buffer*     buffer;
    std::size_t    buffer_size; // total mapped size TODO: remove/combine with x11
    bool           buffer_busy; // TODO: mb combine with x11

    bool           isopen;
    int            width;
    int            height;
  };

  namespace window::wl {
    ::window::result create_window(window_wl_data& data, int w, int h, const char* title) noexcept;
    ::window::result set_title(const window_wl_data& data, const char* title) noexcept;
    ::window::result set_size(window_wl_data& data, int w, int h) noexcept;
    ::window::result poll_events(window_wl_data& data) noexcept;
    ::window::result make_gfx_context(window_wl_data& data) noexcept;
    ::window::result resize_framebuffer(window_wl_data& data, int width, int height) noexcept;
    ::window::result swap_buffers(window_wl_data& data) noexcept;
    ::window::result swap_interval(const window_wl_data& data, int interval) noexcept;
    void destroy(window_wl_data& data) noexcept;
  }

#endif
