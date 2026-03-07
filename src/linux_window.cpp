#include "include/window.hpp"

#ifdef window_x11
  #include "include/x11.hpp"
#endif

#ifdef window_wl
  #include "include/wayland.hpp"
#endif

namespace window {

  #if defined(window_x11) && defined(window_wl)
    static bool is_valid_wl(const window_wl_data& data) {
      return data.display != nullptr;
    }
  #endif

  window::window() noexcept {
    input = {};
    #ifdef window_x11
      x11 = {};
      x11.input    = &input;
      x11.display  = nullptr;
      x11.root     = None;
      x11.win      = None;
      x11.context  = nullptr;
      x11.wmDelete = None;
      x11.isopen   = false;
      x11.x        = -1;
      x11.y        = -1;
      x11.width    = -1;
      x11.height   = -1;
    #endif
    #ifdef window_wl
      wl = {};
      wl.display      = nullptr;
      wl.compositor   = nullptr;
      wl.surface      = nullptr;
      wl.registry     = nullptr;

      wl.xwm_base     = nullptr;
      wl.xsurface     = nullptr;
      wl.xtoplevel    = nullptr;

      wl.decoration_manager = nullptr;
      wl.decoration   = nullptr;

      wl.egl_window   = nullptr;
      wl.egl_display  = nullptr;
      wl.egl_config   = nullptr;
      wl.egl_context  = nullptr;
      wl.egl_surface  = nullptr;

      wl.isopen       = false;
      wl.width        = -1;
      wl.height       = -1;
    #endif
  }

  window::~window() noexcept {
    destroy();
  }

  result window::create(int width, int height, const char* title) noexcept {
    #ifdef window_wl
      result wl_result = wl::create_window(wl, width, height, title);
      #ifdef window_x11
        if (wl_result == SUCCESS) {
          return SUCCESS;
        }
      #endif
    #endif
    #ifdef window_x11
      return x11::create_window(x11, width, height, title);
    #endif
    #if defined(window_wl) && !defined(window_x11)
      return wl_result;
    #endif
  }

  result window::set_title(const char* title) const noexcept {
    #if defined(window_wl) && defined(window_x11)
      // check if wayland or x11 is running
      if (is_valid_wl(wl)) {
        return wl::set_title(wl, title);
      }
    #endif
    #ifdef window_x11
      return x11::set_title(x11, title);
    #endif
    #if defined(window_wl) && !defined(window_x11)
      return wl::set_title(wl, title);
    #endif
  }

  result window::set_size(int width, int height) noexcept {
      #if defined(window_wl) && defined(window_x11)
      // check if wayland or x11 is running
      if (is_valid_wl(wl)) {
        return wl::set_size(wl, width, height);
      }         
    #endif
    #ifdef window_x11
      return x11::set_size(x11, width, height);
    #endif
    #if defined(window_wl) && !defined(window_x11)
      return wl::set_size(wl, width, height);
    #endif
  }

  bool window::is_open() const noexcept {
    #if defined(window_wl) && defined(window_x11)
      if (is_valid_wl(wl)) {
        return wl.isopen;
      }
    #endif
    #ifdef window_x11
      return x11.isopen;
    #endif
    #if defined(window_wl) && !defined(window_x11)
      return wl.is_open;
    #endif
  }

  result window::poll_events() noexcept {
    #if defined(window_wl) && defined(window_x11)
      if (is_valid_wl(wl)) {
        return wl::poll_events(wl);
      }
    #endif
    #ifdef window_x11
      return x11::poll_events(x11);
    #endif
    #if defined(window_wl) && !defined(window_x11)
      return wl::poll_events(wl);
    #endif
  }

  result window::make_opengl_context() noexcept {
    #if defined(window_wl) && defined(window_x11)
      if (is_valid_wl(wl)) {
        return wl::make_opengl_context(wl);
      }
    #endif
    #ifdef window_x11
      return x11::make_opengl_context(x11);
    #endif
    #if defined(window_wl) && !defined(window_x11)
      return wl::make_opengl_context(wl);
    #endif
  }

  result window::swap_buffers() const noexcept {
    #if defined(window_wl) && defined(window_x11)
      if (is_valid_wl(wl)) {
        return wl::swap_buffers(wl);
      }
    #endif
    #ifdef window_x11
      return x11::swap_buffers(x11);
    #endif
    #if defined(window_wl) && !defined(window_x11)
      return wl::swap_buffers(wl);
    #endif
  }

  result window::swap_interval(int interval) const noexcept {
    #if defined(window_wl) && defined(window_x11)
      if (is_valid_wl(wl)) {
        return wl::swap_interval(wl, interval);
      }
    #endif
    #ifdef window_x11
      return x11::swap_interval(x11, interval);
    #endif
    #if defined(window_wl) && !defined(window_x11)
      return wl::swap_interval(wl, interval);
    #endif
  }

  key_event_callback window::set_key_event(key_event_callback func) noexcept {
    key_event_callback temp = input.key_event;
    input.key_event = func;
    return temp;
  }

  button_event_callback window::set_btn_event(button_event_callback func) noexcept {
    button_event_callback temp = input.button_event;
    input.button_event = func;
    return temp;
  }

  dblclk_event_callback window::set_dblclk_event(dblclk_event_callback func) noexcept {
    dblclk_event_callback temp = input.dblclk_event;
    input.dblclk_event = func;
    return temp;
  }

  mouse_event_callback window::set_mouse_event(mouse_event_callback func) noexcept {
    mouse_event_callback temp = input.mouse_event;
    input.mouse_event = func;
    return temp;
  }

  scroll_event_callback window::set_vscroll_event(scroll_event_callback func) noexcept {
    scroll_event_callback temp = input.vscroll_event;
    input.vscroll_event = func;
    return temp;
  }

  scroll_event_callback window::set_hscroll_event(scroll_event_callback func) noexcept {
    scroll_event_callback temp = input.hscroll_event;
    input.hscroll_event = func;
    return temp;
  }

  size_event_callback window::set_size_event(size_event_callback func) noexcept {
    size_event_callback temp = input.size_event;
    input.size_event = func;
    return temp;
  }

  void window::destroy() noexcept {
    #if defined(window_wl) && defined(window_x11)
      if (is_valid_wl(wl)) {
        wl::destroy(wl);
        return;
      }
    #endif
    #ifdef window_x11
      return x11::destroy(x11);
    #endif
    #if defined(window_wl) && !defined(window_x11)
      return wl::destroy(wl);
    #endif
  }
}
