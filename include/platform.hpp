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
      #include <wayland-client.h>
      #include <wayland-egl.h>
      #include <EGL/egl.h>
      #include <GLES2/gl2.h>
      
      #include "xdg-shell-client-protocol.h"

      struct window_wl_data {
        wl_display*    display;
        wl_compositor* compositor;
        wl_surface*    surface;
        xdg_wm_base*   xwm_base;
        xdg_surface*   xsurface;
        xdg_toplevel*  xtoplevel;
        wl_registry*   registry;
        wl_egl_window* egl_window;
        EGLDisplay     egl_display;
        EGLConfig      egl_config;
        EGLContext     egl_context;
        EGLSurface     egl_surface;
        bool           isopen;
        int            width;
        int            height;
      };
    #endif
  #elif defined(window_os_win32)
    #if defined(window_win32)
      #include <Windows.h> // TODO: windows integration

      struct window_win32_data {};
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
      UNKNOWNFAILURE
    };
  }

#endif
