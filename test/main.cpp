#include <include/window.hpp>

#include <stdio.h>

static void key_event(bool down, window::key_descriptor& k) noexcept {
  if (down)
    printf("Key pressed: %s\n", k.description);
}

static void btn_event(bool down, window::button_descriptor& b) noexcept {
  if (down)
    printf("Button pressed: %s\n", b.description);
}

static void dblclk_event(window::button_descriptor& b) noexcept {
  printf("Button double click: %s\n", b.description);
}

static void vscroll_event(float delta) noexcept {
  printf("Mouse wheel vertical delta: %f\n", delta);
}

static void hscroll_event(float delta) noexcept {
  printf("Mouse wheel horiziontal delta: %f\n", delta);
}

static void mouse_event(int x, int y) noexcept {
  printf("Mouse moved: %i, %i\n", x, y);
}

static void size_event(int w, int h) noexcept {
  printf("Window resized: %i, %i\n", w, h);
}

int main() {
  window::window win{};

  // creating the opengl window
  win.create(300, 200, "TEST! :)");
  win.make_opengl_context();

  // setting event callbacks up
  win.set_key_event(key_event);
  win.set_btn_event(btn_event);
  win.set_dblclk_event(dblclk_event);
  win.set_vscroll_event(vscroll_event);
  win.set_hscroll_event(hscroll_event);
  win.set_mouse_event(mouse_event);
  win.set_size_event(size_event);
  
  // opengl setup
  glViewport(0, 0, 300, 200);
  glClearColor(0.f, 0.f, 0.f, 1.f);

  // main loop
  while (win.is_open()) {
    win.poll_events();
    glClear(GL_COLOR_BUFFER_BIT);
    win.swap_buffers();
  }
  win.destroy();

  return 0;
}
