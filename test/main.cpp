#include <window/window.hpp>

#include <stdio.h>
#include <stdlib.h>

static void key_event(bool down, window::key_descriptor& k, void* data) noexcept {
  if (down)
    printf("Key pressed: %s\n", k.description);
}

static void btn_event(bool down, window::button_descriptor& b, void* data) noexcept {
  if (down)
    printf("Button pressed: %s\n", b.description);
}

static void dblclk_event(window::button_descriptor& b, void* data) noexcept {
  printf("Button double click: %s\n", b.description);
}

static void vscroll_event(float delta, void* data) noexcept {
  printf("Mouse wheel vertical delta: %.2f\n", delta);
}

static void hscroll_event(float delta, void* data) noexcept {
  printf("Mouse wheel horiziontal delta: %.2f\n", delta);
}

static void mouse_event(int x, int y, void* data) noexcept {
  // we use a threshold to not spam the console full
  const int threshold = 50;

  static int last_x = -threshold;
  static int last_y = -threshold;

  if (abs(x - last_x) >= threshold || abs(y - last_y) >= threshold) {
    last_x = x;
    last_y = y;
    printf("Mouse moved: %d, %d\n", x, y);
  }
}

static void size_event(int w, int h, void* data) noexcept {
  // we use a threshold to not spam the console full
  const int threshold = 50;

  static int last_w = -threshold;
  static int last_h = -threshold;

  if (abs(w - last_w) >= threshold || abs(h - last_h) >= threshold) {
    last_w = w;
    last_h = h;
    printf("Window resized: %i, %i\n", w, h);
  }

  glViewport(0, 0, w, h);
}

int main() {
  bool use_opengl = false;

  window::window win{};

  // creating the opengl window
  win.set_appname("myappname");

  if (use_opengl) {
    win.set_glversion(3,3); // should be called before every create function
    win.set_gfx_backend(::window::GRAPHICS_OPENGL); //  default/not required
  } else {
    win.set_gfx_backend(::window::GRAPHICS_FRAMEBUFFER);
  }
  win.create(300, 200, "TEST! :)");
  win.make_gfx_context();

  printf("Backend: %s\n", (
    win.get_backend() == window::WAYLAND ? "Wayland" : (
      win.get_backend() == window::X11 ? "X11" : "Win32")
    )
  );

  printf("GFX Backend: %s\n",
    win.get_gfx_backend() == window::GRAPHICS_OPENGL ? "OpenGL" :
    win.get_gfx_backend() == window::GRAPHICS_FRAMEBUFFER ? "Framebuffer" :
    "Unknown"
  );

  if (use_opengl) {
    printf("OpenGL: %s\n", glGetString(GL_VERSION));
  }

  // setting event callbacks up
  win.set_key_event(key_event);
  win.set_btn_event(btn_event);
  win.set_dblclk_event(dblclk_event);
  win.set_vscroll_event(vscroll_event);
  win.set_hscroll_event(hscroll_event);
  win.set_mouse_event(mouse_event);
  win.set_size_event(size_event);
  
  if (use_opengl) {
    // opengl setup
    glViewport(0, 0, 300, 200);
    glClearColor(0.f, 0.f, 0.f, 1.f);
  }

  // main loop
  while (win.is_open()) {
    win.poll_events();
    if (use_opengl) {
      glClear(GL_COLOR_BUFFER_BIT);
    } else {
      for (int x = 0; x < win.framebuffer_width() >> 2; x++) {
        for (int y = 0; y < win.framebuffer_height() >> 2; y++) {
          win.framebuffer_pixels()[x + y * win.framebuffer_stride()] = 0xFFFFFFFF;
        }
      }
      for (int i = 0; i < win.framebuffer_width(); i++) {
        win.framebuffer_pixels()[i] = 0xFFFF0000;
      }
      for (int i = 0; i < win.framebuffer_height(); i++) {
        win.framebuffer_pixels()[i * win.framebuffer_width()] = 0xFFFF0000;
      }
    }
    win.swap_buffers();
  }
  win.destroy();

  return 0;
}
