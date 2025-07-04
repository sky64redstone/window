#ifndef WINDOW_HPP
  #define WINDOW_HPP

  #include "platform.hpp"

  namespace window {
    class window {
    private:
      #if defined(window_x11)
        window_x11_data x11;
      #endif

      #if defined(window_wl)
        window_wl_data wl;
      #endif

      #if defined(window_win32)
        window_win32_data win32;
      #endif

    public:
      window() noexcept;
      ~window() noexcept;

      result create(int width, int height, const char* title) noexcept;

      result set_title(const char* title) const noexcept;
      result set_size(int width, int height) const noexcept;

      bool is_open() const noexcept;
      result poll_events() noexcept;

      result make_opengl_context() noexcept;

      result swap_buffers() const noexcept;
      result swap_interval(int interval) const noexcept;

      void destroy() noexcept;
    };
  }

#endif
