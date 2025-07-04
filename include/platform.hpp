#ifndef INCLUDES_HPP
  #define INCLUDES_HPP

  #if defined(__linux__)
    #define window_os_linux
  #endif

  #if defined(_WIN32)
    #define window_os_win32
  #endif

  #if defined(window_os_linux)
    // include X11 stuff if needed
    #if defined(window_x11)
      #include <X11/X.h>
      #include <X11/Xlib.h>
      #include <X11/Xutil.h>
      // opengl integration
      #include <GL/glx.h>
      // x11 data struct
      struct window_x11_data {
        Display*   display;
        Window     root;
        Window     win;
        GLXContext context;
        Atom       wmDelete;
        bool       isopen;
        int        x;
        int        y;
        int        width;
        int        height;
      };
    #endif

    // include wayland stuff if needed
    #if defined(window_wl)
      struct window_wl_data {};
    #endif
  #elif defined(window_os_win32)
    #if defined(window_win32)
      #include <Windows.h> // TODO: windows integration

      struct window_win32_data {};
    #endif
  #endif

  namespace window {
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
      // e.g. the window couldn't be opened
      CREATIONFAILED,
      // the operation failed, because the
      // window isn't valid
      BADWINDOW,
      // the operation failed, because a
      // given parameter was malformed
      BADALLOC
    };
  }

#endif
