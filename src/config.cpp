#include "window/config.hpp"

#include <cstring>

namespace window {
  config::config() noexcept {
    width = height = glmajor = glminor = 0;
    std::memset(&this->input, 0, sizeof(input_data));
  }

  config& config::title(std::string_view value) noexcept {
    this->win_title = value;
    return *this;
  }

  config& config::size(int width, int height) noexcept {
    this->width = width;
    this->height = height;
    return *this;
  }

  config& config::app_name(std::string_view value) noexcept {
    this->class_name = value;
    return *this;
  }

  config& config::user_data(void* value) noexcept {
    this->input.user_data = value;
    return *this;
  }

  config& config::on_key(key_event_callback fn) noexcept {
    this->input.key_event = fn;
    return *this;
  }

  config& config::on_mouse_move(mouse_event_callback fn) noexcept {
    this->input.mouse_event = fn;
    return *this;
  }

  config& config::on_mouse_button(button_event_callback fn) noexcept {
    this->input.button_event = fn;
    return *this;
  }

  config& config::on_double_click(dblclk_event_callback fn) noexcept {
    this->input.dblclk_event = fn;
    return *this;
  }

  config& config::on_vscroll(scroll_event_callback fn) noexcept {
    this->input.vscroll_event = fn;
    return *this;
  }

  config& config::on_hscroll(scroll_event_callback fn) noexcept {
    this->input.hscroll_event = fn;
    return *this;
  }

  config& config::on_resize(size_event_callback fn) noexcept {
    this->input.size_event = fn;
    return *this;
  }

  config& config::framebuffer() noexcept {
    this->gfx_backend = GRAPHICS_FRAMEBUFFER;
    return *this;
  }

  config& config::opengl() noexcept {
    this->gfx_backend = GRAPHICS_OPENGL;
    return *this;
  }

  config& config::version(int major, int minor) noexcept {
    this->glmajor = major;
    this->glminor = minor;
    return *this;
  }
}
