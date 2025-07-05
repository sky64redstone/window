add_library(window SHARED
  "../include/window.hpp"
)

target_include_directories(window PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

if (window_win32)
  target_compile_definitions(window PRIVATE window_win32)
  target_sources(window PRIVATE win32.cpp)
endif()

if (window_x11)
  target_compile_definitions(window PRIVATE window_x11)
  target_sources(window PRIVATE x11.cpp)
  target_link_libraries(window PUBLIC X11 GL)
endif()

if (window_wl)
  target_compile_definitions(window PRIVATE window_wl)
  target_sources(window PRIVATE wayland.cpp xdg-shell-protocol.c)
  target_link_libraries(window PUBLIC wayland-client EGL wayland-egl GLESv2)
endif()

if (window_x11 OR window_wl)
  target_sources(window PRIVATE linux.cpp)
endif()
