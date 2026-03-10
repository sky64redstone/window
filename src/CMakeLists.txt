add_library(window SHARED
  "log.cpp"
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
  add_dependencies(window wayland_protocols)

  set_source_files_properties(${WL_GENERATED_SOURCES} PROPERTIES GENERATED TRUE)

  target_sources(window PRIVATE
    wl_window.cpp
    ${WL_GENERATED_SOURCES}
  )

  if (window_no_pkg)
    message(STATUS "PkgConfig disabled")
    target_link_libraries(window
      PUBLIC
        wayland-client EGL
        wayland-egl
        xkbcommon
    )
  else()
    message(STATUS "PkgConfig enabled")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(DEPS REQUIRED IMPORTED_TARGET
      wayland-client egl
      wayland-egl
      xkbcommon
    )
    target_link_libraries(window
      PUBLIC PkgConfig::DEPS
    )
  endif()

  target_compile_definitions(window PUBLIC window_wl)

endif()

if (window_x11 OR window_wl)
  target_sources(window PRIVATE linux_window.cpp linux_input.cpp)
endif()

set_target_properties(window PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
