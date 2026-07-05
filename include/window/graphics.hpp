#ifndef WINDOW_GRAPHICS_HPP
  #define WINDOW_GRAPHICS_HPP

  #include <cstdint>

  namespace window {
    enum graphics_backend {
      GRAPHICS_FRAMEBUFFER,
      GRAPHICS_OPENGL
    };

    struct framebuffer_data {
      /* TODO: more efficient way with shm (would work at least with x11) */
      std::uint32_t* pixels = nullptr;
      int width = 0;
      int height = 0;
      int stride = 0;
    };
  }
#endif
