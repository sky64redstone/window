cmake_minimum_required (VERSION 3.10)

project(window)

macro(dependent_option OPTION_NAME OPTION_DESC DEFAULT_VALUE DEP_CONDITION)
    if(${DEP_CONDITION})
        option(${OPTION_NAME} "${OPTION_DESC}" ${DEFAULT_VALUE})
    else()
        set(${OPTION_NAME} OFF CACHE BOOL "${OPTION_DESC} (disabled due to missing dependency)" FORCE)
    endif()
endmacro()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# CMake Options
option(window_no_pkg          "Disable PkgConfig"    OFF)
dependent_option(window_win32 "Enable win32 support" ON "WIN32" OFF)
dependent_option(window_x11   "Enable x11 support"   ON "UNIX"  OFF)
dependent_option(window_wl    "Enable wl support"    ON "UNIX"  OFF)

if (window_win32)
  message(STATUS "Win32 support enabled")
endif()
if (window_x11)
  message(STATUS "X11 support enabled")
endif()
if (window_wl)
  message(STATUS "Wayland support enabled")

  find_program(WAYLAND_SCANNER wayland-scanner REQUIRED)

  set(WAYLAND_PROTOCOL_DIR /usr/share/wayland-protocols)

  set(XDG_SHELL_XML
      ${WAYLAND_PROTOCOL_DIR}/stable/xdg-shell/xdg-shell.xml)

  set(XDG_DECOR_XML
      ${WAYLAND_PROTOCOL_DIR}/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml)

  set(XDG_SHELL_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/xdg-shell-protocol.c)
  set(XDG_SHELL_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/xdg-shell-client-protocol.h)

  set(XDG_DECOR_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/xdg-decoration-protocol.c)
  set(XDG_DECOR_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/xdg-decoration-client-protocol.h)

  add_custom_command(
      OUTPUT ${XDG_SHELL_SOURCE} ${XDG_SHELL_HEADER}
      COMMAND ${WAYLAND_SCANNER} client-header ${XDG_SHELL_XML} ${XDG_SHELL_HEADER}
      COMMAND ${WAYLAND_SCANNER} private-code ${XDG_SHELL_XML} ${XDG_SHELL_SOURCE}
      DEPENDS ${XDG_SHELL_XML}
  )

  add_custom_command(
      OUTPUT ${XDG_DECOR_SOURCE} ${XDG_DECOR_HEADER}
      COMMAND ${WAYLAND_SCANNER} client-header ${XDG_DECOR_XML} ${XDG_DECOR_HEADER}
      COMMAND ${WAYLAND_SCANNER} private-code ${XDG_DECOR_XML} ${XDG_DECOR_SOURCE}
      DEPENDS ${XDG_DECOR_XML}
  )

  add_custom_target(wayland_protocols
      DEPENDS
          ${XDG_SHELL_SOURCE}
          ${XDG_SHELL_HEADER}
          ${XDG_DECOR_SOURCE}
          ${XDG_DECOR_HEADER}
  )

  set(WL_GENERATED_SOURCES
      ${XDG_SHELL_SOURCE}
      ${XDG_DECOR_SOURCE}
  )

  set(WL_GENERATED_HEADERS
      ${XDG_SHELL_HEADER}
      ${XDG_DECOR_HEADER}
  )
endif()

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(test)
