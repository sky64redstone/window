#if defined(window_x11) && !defined(X11_HPP)
  #define X11_HPP

  #include <string_view>

  #include <X11/X.h>
  #include <X11/Xlib.h>
  #include <X11/Xutil.h>
  // opengl integration
  #include <GL/glx.h>
  #include <GL/glxext.h>

  #include "platform.hpp"

  // x11 data struct
  struct window_x11_data {
    ::window::input_data* input;
    ::window::framebuffer_data* fb;

    Display*   display;
    Window     root;
    Window     win;
    GLXContext context;
    Atom       wmDelete;
    Visual*    visual;
    int        depth;
    bool       isopen;
    int        x;
    int        y;
    int        width;
    int        height;
  };

  namespace window::x11 {
    ::window::result create_window(
      window_x11_data& data, int w, int h, std::string_view title, 
      graphics_backend gfx_backend, int glmajor, int glminor, std::string_view appname
    ) noexcept;
    ::window::result set_title(const window_x11_data& data, const char* title) noexcept;
    ::window::result set_size(const window_x11_data& data, int w, int h) noexcept;
    ::window::result poll_events(window_x11_data& data) noexcept;
    ::window::result make_gfx_context(window_x11_data& data, graphics_backend gfx_backend) noexcept;
    ::window::result resize_framebuffer(window_x11_data& data, graphics_backend gfx_backend, int width, int height) noexcept;
    ::window::result swap_buffers(const window_x11_data& data, graphics_backend gfx_backend) noexcept;
    ::window::result swap_interval(const window_x11_data& data, graphics_backend gfx_backend, int interval) noexcept;
    void destroy(window_x11_data& data) noexcept;
  }

#endif
