#ifndef WINDOW_HPP
  #define WINDOW_HPP

  #include "platform.hpp"
  #include "input.hpp"

  namespace window {
    class window_api window {
    public:
      #if defined(window_x11)
        window_x11_data x11;
      #endif

      #if defined(window_wl)
        window_wl_data wl;
      #endif

      #if defined(window_win32)
        window_win32_data win32;
      #endif

      input_data input;

    public:
      window() noexcept;
      ~window() noexcept;

      result create(int width, int height, const char* title) noexcept;

      result set_title(const char* title) const noexcept;
      result set_size(int width, int height) noexcept;

      bool is_open() const noexcept;
      result poll_events() noexcept;

      result make_opengl_context() noexcept;

      result swap_buffers() const noexcept;
      result swap_interval(int interval) const noexcept;

      key_event_callback    set_key_event(key_event_callback       func) noexcept;
      button_event_callback set_btn_event(button_event_callback    func) noexcept;
      dblclk_event_callback set_dblclk_event(dblclk_event_callback func) noexcept;
      mouse_event_callback  set_mouse_event(mouse_event_callback   func) noexcept;
      wheel_event_callback  set_wheel_event(wheel_event_callback   func) noexcept;
      size_event_callback   set_size_event(size_event_callback     func) noexcept;

      void destroy() noexcept;
    };
  }

#endif
