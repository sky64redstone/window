#ifndef PLATFORM_HPP
  #define PLATFORM_HPP

  #if defined(__linux__)
    #define window_os_linux
  #endif

  #if defined(_WIN32)
    #define window_os_win32
  #endif

  #include "input.hpp"
  #include "hints.hpp"

  #if defined(window_os_linux)
    #define window_api __attribute__((visibility("default")))
  #elif defined(window_os_win32)
    #ifdef window_build
      #define window_api __declspec(dllexport)
    #else
      #define window_api __declspec(dllimport)
    #endif

    #if defined(window_win32)
      #define NOMINMAX
      #define WIN32_LEAN_AND_MEAN
      #if defined(__MINGW32__) || defined(__MINGW64__)
        #include <windows.h>
        #include <dwmapi.h>
        #include <GL/gl.h>
      #else
        #include <Windows.h>
        #include <dwmapi.h>
        #include <gl/GL.h>
      #endif

      struct window_win32_data {
        ::window::input_data* input;
        HWND win;
        HDC dc;
        HGLRC rc;
        bool isopen;
        int x;
        int y;
        int width;
        int height;
      };
    #endif
  #endif

  namespace window {
    // with most failure codes, the operations also print 
    // debug information to stderr
    enum result {
      // the operation was successful
      SUCCESS,
      // the operation was aborted, because the 
      // operation would override existing data
      ALREADYEXISTS,
      // the operation isn't supported on the 
      // currently running system
      UNSUPPORTED,
      // the connection to the display server
      // failed
      CONNECTIONFAILED,
      // the creation of the object failed
      // e.g. the window couldn't be created
      CREATIONFAILED,
      // the operation failed, because the
      // window (or parts of it) isn't valid
      BADWINDOW,
      // the operation failed, because a
      // given parameter was malformed
      BADALLOC,
      // the operation failed, because previous
      // functions weren't set up properly
      UNKNOWNFAILURE,
      // the operation was aborted, because
      // a quit message was received
      QUIT
    };

    enum backend {
      WINDOWS,
      X11,
      WAYLAND
    };
  }

#endif
