#ifndef HINTS_HPP
  #define HINTS_HPP

  #include <string>

  #include "graphics.hpp"

  namespace window {
    struct hint_data {
      std::string appname;
      int glvmajor = 0;
      int glvminor = 0;
      graphics_backend gfx_backend = GRAPHICS_OPENGL;
    };
  }

#endif
