#include "include/window.hpp"

int main() {
  window::window win{};
  win.create(300, 200, "TEST! :)");

  while (win.is_open()) {
    win.poll_events();
  }

  win.destroy();

  return 0;
}
