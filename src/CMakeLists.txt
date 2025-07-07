add_library(window SHARED
  "../include/window.hpp"
)

target_include_directories(window PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_compile_definitions(window PRIVATE window_build)

if (window_win32)
  target_compile_definitions(window PUBLIC window_win32)
  target_sources(window PRIVATE win32_input.cpp)
  target_sources(window PRIVATE win32_window.cpp)
  target_link_libraries(window PUBLIC user32 kernel32 gdi32 dwmapi opengl32)
endif()

if (window_x11)
  target_compile_definitions(window PUBLIC window_x11)
  target_sources(window PRIVATE x11_window.cpp)
  target_link_libraries(window PUBLIC X11 GL)
endif()

if (window_wl)
  target_compile_definitions(window PUBLIC window_wl)
  target_sources(window PRIVATE wl_window.cpp xdg-shell-protocol.c)
  target_link_libraries(window PUBLIC wayland-client EGL wayland-egl GLESv2)
endif()

if (window_x11 OR window_wl)
  target_sources(window PRIVATE linux.cpp)
endif()

set_target_properties(window PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)