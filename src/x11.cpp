#include "include/x11.hpp"

#include <stdio.h> // printf for x11 error handler

namespace window {

  namespace x11 {

    using glx_swap_interval_ext_proc = void(*)(Display*, GLXDrawable, int);
    inline glx_swap_interval_ext_proc glXSwapIntervalEXT = nullptr;

    void load_swap_interval() {
      if (glXSwapIntervalEXT == nullptr) {
        glXSwapIntervalEXT = reinterpret_cast<glx_swap_interval_ext_proc>(
          glXGetProcAddress(reinterpret_cast<const GLubyte*>("glXSwapIntervalEXT"))
        );
      }
    }

    inline int error_code = 0;

    int error_handler(Display* display, XErrorEvent* error) {
      error_code = error->error_code;
      char text[512];
      XGetErrorText(display, error_code, text, sizeof(text));
      fprintf(stderr,
        "[window] x11: Request Code: %i; Error Code: %i\n"
        "[window] x11: %s\n",
        error->request_code, error_code, text
      );
      return 0;
    }

    ::window::result create_window(window_x11_data& data, int w, int h, const char* title) noexcept {
      if (data.display != None) {
        return window::ALREADYEXISTS;
      }

      XSetErrorHandler(error_handler);

      data.display = XOpenDisplay(nullptr);

      if (data.display == nullptr) {
        fprintf(stderr, "[window] x11: Couldn't open the connection to the display server!!!\n");
        return window::CONNECTIONFAILED;
      }

      if (glXQueryExtension(data.display, nullptr, nullptr) != True) {
        fprintf(stderr, "[window] x11: GLX isn't supported on this device!!!\n");
        return window::UNSUPPORTED;
      }

      data.root    = DefaultRootWindow(data.display);

      int screen = XDefaultScreen(data.display);

      int sw = DisplayWidth(data.display, screen);
      int sh = DisplayHeight(data.display, screen);

      int x = (sw - w) / 2;
      int y = (sh - h) / 2;

      GLint glattr[] = {
        GLX_RGBA, GLX_DEPTH_SIZE, 24,
        GLX_DOUBLEBUFFER, None
      };

      XVisualInfo* visual = glXChooseVisual(data.display, screen, glattr);

      XSetWindowAttributes attr = {};
      attr.event_mask = ExposureMask | KeyPressMask |
        KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
        PointerMotionMask | FocusChangeMask | StructureNotifyMask;

      attr.colormap = XCreateColormap(
        data.display, data.root, 
        visual->visual, 
        AllocNone
      );
        
      data.win = XCreateWindow(
        data.display,
        data.root,
        x, y,
        w, h,
        0, // border width
        visual->depth,
        InputOutput,
        visual->visual,
        CWColormap | CWEventMask,
        &attr
      );

      XSync(data.display, False);

      if (error_code != 0) {
        return window::CREATIONFAILED;
      }

      data.isopen = true;

      data.context = glXCreateContext(data.display, visual, nullptr, GL_TRUE);
      if (data.context == NULL) {
        return window::CREATIONFAILED;
      }
      glXMakeCurrent(data.display, data.win, data.context);

      data.wmDelete = XInternAtom(data.display, "WM_DELETE_WINDOW", False);
      XSetWMProtocols(data.display, data.win, &data.wmDelete, 1);

      XMapWindow(data.display, data.win);

      XStoreName(data.display, data.win, title);

      return window::SUCCESS;
    }

    ::window::result set_title(const window_x11_data& data, const char* title) noexcept {
      error_code = 0;
      XSetErrorHandler(error_handler);
      XStoreName(data.display, data.win, title);
      XSync(data.display, False);
      return error_code == BadWindow ? 
        window::BADWINDOW : 
        error_code == 0 ? 
          window::SUCCESS : 
          window::BADALLOC;
    }

    ::window::result set_size(const window_x11_data& data, int w, int h) noexcept {
      // ensure w and h are positive
      w = w > 0 ? w : -w + 1;
      h = h > 0 ? h : -h + 1;
      // enable error handler and set size
      error_code = 0;
      XSetErrorHandler(error_handler);
      XResizeWindow(data.display, data.win, w, h);
      XSync(data.display, False);
      return error_code == 0 ? window::SUCCESS : window::BADWINDOW;
    }

    ::window::result poll_events(window_x11_data& data) noexcept {
      XEvent event;

      XPending(data.display);

      while (QLength(data.display)) {
        XNextEvent(data.display, &event);
          
        // process events

        switch (event.type) {
        case Expose: {
          XWindowAttributes attr;
          XGetWindowAttributes(data.display, data.win, &attr);

          data.x = attr.x;
          data.y = attr.y;
          data.width  = attr.width;
          data.height = attr.height;

          break;
        }
        case ConfigureNotify: {
          data.x = event.xconfigure.x;
          data.y = event.xconfigure.y;
          data.width  = event.xconfigure.width;
          data.height = event.xconfigure.height;

          break;
        }
        case KeyPress:
        case KeyRelease: {
          KeySym sym = XLookupKeysym(&event.xkey, 0);
          // TODO keyboard integration
          break;
        }
        case ButtonPress:
        case ButtonRelease: {
          // TODO mouse integration
          break;
        }
        case MotionNotify: {
          // TODO mouse integration
          break;
        }
        case FocusIn:
        case FocusOut: {
          // TODO key focus
          break;
        }
        case EnterNotify:
        case LeaveNotify: {
          // TODO mouse focus
          break;
        }
        case ClientMessage: {
          if (static_cast<Atom>(event.xclient.data.l[0]) == data.wmDelete) {
            data.isopen = false;
          }
          break;
        }
        }
      }

      return window::SUCCESS;
    } // pollevents

    ::window::result make_opengl_context(window_x11_data& data) noexcept {
      // Nothing to do in x11, because we need to create the window
      // with a opengl context :)
      return window::SUCCESS;
    }

    ::window::result swap_buffers(const window_x11_data& data) noexcept {
      glXSwapBuffers(data.display, data.win);
      return window::SUCCESS;
    }

    ::window::result swap_interval(const window_x11_data& data, int interval) noexcept {
      load_swap_interval();
      if (glXSwapIntervalEXT == nullptr) {
        fprintf(stderr, "[window] x11: glXSwapIntervalEXT not supported!!!\n");
        return window::UNSUPPORTED;
      }
      glXSwapIntervalEXT(data.display, data.win, interval);
      return window::SUCCESS;
    }

    void destroy(window_x11_data& data) noexcept {
      // opengl
      glXMakeCurrent(data.display, None, nullptr);
      glXDestroyContext(data.display, data.context);
      data.context = nullptr;
      // window
      XDestroyWindow(data.display, data.win);
      data.isopen = false;
      data.win = None;
      // display
      XCloseDisplay(data.display);
      data.display = nullptr;
    }

  } // namespace x11

} // namespace window
