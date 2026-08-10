#include <window/window.hpp>

#include <math.h>
#include <cstdio>
#include <GL/gl.h>

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

  window::config c;
  window::window win{};

  if (use_opengl) {
    c.opengl()
    .version(3,3);
  } else {
    c.framebuffer();
  }

  // creating the opengl window
  win.create(
    c.title("Test! :)")
     .size(300, 200)
     .app_name("myappname")
     // event callback config
     .on_key(key_event)
     .on_mouse_move(mouse_event)
     .on_mouse_button(btn_event)
     .on_double_click(dblclk_event)
     .on_vscroll(vscroll_event)
     .on_hscroll(hscroll_event)
     .on_resize(size_event)
  );

  printf("Win Backend: %s\n", (
    win.get_backend() == window::WAYLAND ? "Wayland" : (
      win.get_backend() == window::X11 ? "X11" : "Win32")
    )
  );
  printf("GFX Backend: %s\n",
    use_opengl ? "OpenGL" : "Framebuffer"
  );
  if (use_opengl) {
    printf("OpenGL: %s\n", glGetString(GL_VERSION));
  }
  
  if (use_opengl) {
    // opengl setup
    glViewport(0, 0, 300, 200);
    glClearColor(0.f, 0.f, 0.f, 1.f);
  } else {
    const int w = win.framebuffer_width();
    const int h = win.framebuffer_height();
    const int offset = win.framebuffer_stride() / sizeof(std::uint32_t);
    auto fb = win.framebuffer_pixels();

    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        float u = (w > 1) ? (float)x/(w-1) : 0.0f;
        float v = (h > 1) ? (float)y/(h-1) : 0.0f;
        fb[x + y * offset] =
          0xFF000000u |
          (((uint32_t)(255*(u*0.9f+0.1f)) & 255u) << 16) |
          (((uint32_t)(255*(v*0.9f+0.1f)) & 255u) << 8) |
          ((uint32_t)(255*(1.0f-u)) & 255u);
      }
    }
  }
  win.swap_buffers();

  // main loop
  while (win.is_open()) {
    win.poll_events();
    if (use_opengl) {
      glClear(GL_COLOR_BUFFER_BIT);
    }
    win.swap_buffers();
  }
  win.destroy();

  return 0;
}
