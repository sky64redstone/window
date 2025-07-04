#include "include/window.hpp"

#ifdef window_x11
  #include "include/x11.hpp"
#endif

#if defined(window_x11) || defined(window_wl)

  namespace window {

    #if defined(window_x11) && defined(window_wl)
      bool is_valid_wl(const window_wl_data& data) {
        return false; // TODO as soon as I implement wayland
      }
    #endif

    window::window() noexcept {
      #ifdef window_x11
        x11 = {};
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
      #endif
    }

    window::~window() noexcept {
      #ifdef window_x11
        if (x11.display == nullptr) {
          return;
        }
      #endif
      #ifdef window_wl
      #endif
      destroy();
    }

    result window::create(int width, int height, const char* title) noexcept {
      #ifdef window_wl
        #if !defined(window_x11)
          //result wl_result = 
        #endif
        //wl::create_window(wl, width, height, title);
      #endif
      #ifdef window_x11
        return x11::create_window(x11, width, height, title);
      #endif
      #if defined(window_wl) && !defined(window_x11)
        //return wl_result;
      #endif
    }

    result window::set_title(const char* title) const noexcept {
      #if defined(window_wl) && defined(window_x11)
        // check if wayland or x11 is running
        if (is_valid_wl(wl)) {
          return SUCCESS; // TODO as soon as I implement wayland
        }         
      #endif
      #ifdef window_x11
        return x11::set_title(x11, title);
      #endif
      #if defined(window_wl) && !defined(window_x11)
        //return wl::set_title(wl, title);
      #endif
    }

    result window::set_size(int width, int height) const noexcept {
       #if defined(window_wl) && defined(window_x11)
        // check if wayland or x11 is running
        if (is_valid_wl(wl)) {
          return SUCCESS; // TODO as soon as I implement wayland
        }         
      #endif
      #ifdef window_x11
        return x11::set_size(x11, width, height);
      #endif
      #if defined(window_wl) && !defined(window_x11)
        //return wl::set_size(wl, width, height);
      #endif
    }

    bool window::is_open() const noexcept {
      #if defined(window_wl) && defined(window_x11)
        if (is_valid_wl(wl)) {
          return false; // TODO as soon as I implement wayland
        }
      #endif
      #ifdef window_x11
        return x11.isopen;
      #endif
      #if defined(window_wl) && !defined(window_x11)
        //return wl.is_open;
      #endif
    }

    result window::poll_events() noexcept {
      #if defined(window_wl) && defined(window_x11)
        if (is_valid_wl(wl)) {
          return SUCCESS; // TODO as soon as I implement wayland
        }
      #endif
      #ifdef window_x11
        return x11::poll_events(x11);
      #endif
      #if defined(window_wl) && !defined(window_x11)
        //return wl::poll_events(wl);
      #endif
    }

    result window::make_opengl_context() noexcept {
      #if defined(window_wl) && defined(window_x11)
        if (is_valid_wl(wl)) {
          return SUCCESS; // TODO as soon as I implement wayland
        }
      #endif
      #ifdef window_x11
        return x11::make_opengl_context(x11);
      #endif
      #if defined(window_wl) && !defined(window_x11)
        //return wl::make_opengl_context(wl);
      #endif
    }

    result window::swap_buffers() const noexcept {
      #if defined(window_wl) && defined(window_x11)
        if (is_valid_wl(wl)) {
          return SUCCESS; // TODO as soon as I implement wayland
        }
      #endif
      #ifdef window_x11
        return x11::swap_buffers(x11);
      #endif
      #if defined(window_wl) && !defined(window_x11)
        //return wl::swap_buffers(wl);
      #endif
    }

    result window::swap_interval(int interval) const noexcept {
      #if defined(window_wl) && defined(window_x11)
        if (is_valid_wl(wl)) {
          return SUCCESS; // TODO as soon as I implement wayland
        }
      #endif
      #ifdef window_x11
        return x11::swap_interval(x11, interval);
      #endif
      #if defined(window_wl) && !defined(window_x11)
        //return wl::swap_interval(wl, interval);
      #endif
    }

    void window::destroy() noexcept {
      #if defined(window_wl) && defined(window_x11)
        if (is_valid_wl(wl)) {
          return; // TODO as soon as I implement wayland
        }
      #endif
      #ifdef window_x11
        return x11::destroy(x11);
      #endif
      #if defined(window_wl) && !defined(window_x11)
        //return wl::destroy(wl);
      #endif
    }
  }

#endif
