#ifndef WINDOW_CONFIG_HPP
  #define WINDOW_CONFIG_HPP

  #include <string_view>

  #include "graphics.hpp"
  #include "platform.hpp"
  #include "input.hpp"

  namespace window {
    class window_api config {
    public:
      std::string_view win_title;
      std::string_view class_name;
      int width;
      int height;

      input_data input;

      graphics_backend gfx_backend;
      int glmajor;
      int glminor;

    public:
      config() noexcept;

      config& title(std::string_view value) noexcept;
      config& size(int width, int height) noexcept;
      config& app_name(std::string_view value) noexcept;
      config& user_data(void* value) noexcept;

      config& on_key(key_event_callback fn) noexcept;
      config& on_mouse_move(mouse_event_callback fn) noexcept;
      config& on_mouse_button(button_event_callback fn) noexcept;
      config& on_double_click(dblclk_event_callback fn) noexcept;
      config& on_vscroll(scroll_event_callback fn) noexcept;
      config& on_hscroll(scroll_event_callback fn) noexcept;
      config& on_resize(size_event_callback fn) noexcept;

      config& framebuffer() noexcept;
      config& opengl() noexcept;
      config& version(int major, int minor) noexcept;
    };
  }

#endif
