#include <include/window.hpp>

#ifdef window_os_linux
  #include <GLES2/gl2.h>
#endif

#if defined(window_win32)
  #define NOMINMAX
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #include <dwmapi.h>
  #include <gl/GL.h>
#endif

int main() {
  window::window win{};

  win.create(300, 200, "TEST! :)");
  win.make_opengl_context();

  glViewport(0, 0, 300, 200);
  glClearColor(0.f, 0.f, 0.f, 1.f);

  while (win.is_open()) {
    win.poll_events();
    glClear(GL_COLOR_BUFFER_BIT);
    win.swap_buffers();
  }
  win.destroy();

  return 0;
}
