#include "window/x11.hpp"

#include <time.h>  // for clock_gettime (for dbl clk events)

#include "window/log.hpp"

namespace window {

  namespace x11 {
    using glXCreateContextAttribsARBProc =
      GLXContext(*)(
        Display*, GLXFBConfig, GLXContext, Bool, const int*
      );
    inline glXCreateContextAttribsARBProc glXCreateContextAttribsARB = nullptr;

    void load_create_context_attribs() {
      if (glXCreateContextAttribsARB == nullptr) {
        glXCreateContextAttribsARB = reinterpret_cast<glXCreateContextAttribsARBProc>(
          glXGetProcAddress(reinterpret_cast<const GLubyte*>("glXCreateContextAttribsARB"))
        );
      }
    }

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
      ::window::log_error(::window::LOG_X11, "Request Code: %i; Error Code: %i, %s",
        error->request_code, error_code, text
      );
      return 0;
    }

    ::window::result create_window(
      window_x11_data& data, int w, int h, std::string_view title, 
      graphics_backend gfx_backend, int glmajor, int glminor, std::string_view appname
    ) noexcept {
      if (data.display != None) {
        ::window::log_warning(::window::LOG_X11, "window already exists");
        return window::ALREADYEXISTS;
      }

      XSetErrorHandler(error_handler);

      data.display = XOpenDisplay(nullptr);

      if (data.display == nullptr) {
        ::window::log_error(::window::LOG_X11, "Couldn't open the connection to the display server");
        return window::CONNECTIONFAILED;
      }

      data.root    = DefaultRootWindow(data.display);

      int screen = XDefaultScreen(data.display);

      int sw = DisplayWidth(data.display, screen);
      int sh = DisplayHeight(data.display, screen);

      int x = (sw - w) / 2;
      int y = (sh - h) / 2;

      XSetWindowAttributes attr = {};
      attr.event_mask = ExposureMask | KeyPressMask |
        KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
        PointerMotionMask | FocusChangeMask | StructureNotifyMask;

      switch (gfx_backend) {
        case window::GRAPHICS_OPENGL: {
          if (glXQueryExtension(data.display, nullptr, nullptr) != True) {
            ::window::log_error(::window::LOG_X11, "GLX isn't supported on this device");
            destroy(data);
            return window::UNSUPPORTED;
          }

          GLXFBConfig fbConfig;
          XVisualInfo* visual;

          // do we need to create a specific context?
          bool glv = glmajor > 0 && glminor >= 0;

          if (glv) {
            int fbAttribs[] = {
              GLX_X_RENDERABLE, True,
              GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
              GLX_RENDER_TYPE, GLX_RGBA_BIT,
              GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,

              GLX_RED_SIZE,   8,
              GLX_GREEN_SIZE, 8,
              GLX_BLUE_SIZE,  8,
              GLX_ALPHA_SIZE, 8,

              GLX_DEPTH_SIZE, 24,
              GLX_STENCIL_SIZE, 8,

              GLX_DOUBLEBUFFER, True,
              None
            };

            int fbCount;
            GLXFBConfig* fbConfigs = glXChooseFBConfig(
              data.display,
              screen,
              fbAttribs,
              &fbCount
            );

            if (!fbConfigs) {
              ::window::log_error(::window::LOG_X11, "failed to choose fb config");
              return window::UNSUPPORTED;
            }

            fbConfig = fbConfigs[0];

            visual = glXGetVisualFromFBConfig(
              data.display,
              fbConfig
            );

            if (!visual) {
              ::window::log_error(::window::LOG_X11, "failed to get visual from fb config");
              destroy(data);
              return window::CREATIONFAILED;
            }

          } else {
            GLint glattr[] = {
              GLX_RGBA, GLX_DEPTH_SIZE, 24,
              GLX_DOUBLEBUFFER, None
            };

            visual = glXChooseVisual(data.display, screen, glattr);
          }

          data.visual = visual->visual;
          data.depth  = visual->depth;

          attr.colormap = XCreateColormap(
            data.display, data.root,
            data.visual,
            AllocNone
          );

          data.win = XCreateWindow(
            data.display,
            data.root,
            x, y,
            w, h,
            0, // border width
            data.depth,
            InputOutput,
            data.visual,
            CWColormap | CWEventMask,
            &attr
          );

          if (data.win == None) {
            ::window::log_error(::window::LOG_X11, "failed to create glx window");
            destroy(data);
            return window::CREATIONFAILED;
          }

          XSync(data.display, False);

          if (error_code != 0) {
            ::window::log_error(::window::LOG_X11, "received error code");
            destroy(data);
            return window::CREATIONFAILED;
          }

          if (glv) {
            load_create_context_attribs();

            if (!glXCreateContextAttribsARB) {
              ::window::log_error(::window::LOG_X11, "load_create_context_attribs failed");
              destroy(data);
              return window::UNSUPPORTED;
            }

            int contextAttribs[] = {
              GLX_CONTEXT_MAJOR_VERSION_ARB, glmajor,
              GLX_CONTEXT_MINOR_VERSION_ARB, glminor,

              GLX_CONTEXT_PROFILE_MASK_ARB,
              GLX_CONTEXT_CORE_PROFILE_BIT_ARB,

              #ifdef DEBUG
              GLX_CONTEXT_FLAGS_ARB,
              GLX_CONTEXT_DEBUG_BIT_ARB,
              #endif

              None
            };

            data.context = glXCreateContextAttribsARB(
              data.display,
              fbConfig,
              nullptr,
              True,
              contextAttribs
            );

            if (!data.context) {
              ::window::log_error(::window::LOG_X11, "failed to create glx context");
              destroy(data);
              return window::CREATIONFAILED;
            }
          } else {
            data.context = glXCreateContext(data.display, visual, nullptr, GL_TRUE);
            if (data.context == NULL) {
              ::window::log_error(::window::LOG_X11, "failed to create glx context");
              destroy(data);
              return window::CREATIONFAILED;
            }
          }

          glXMakeCurrent(data.display, data.win, data.context);

          break;
        }
        case window::GRAPHICS_FRAMEBUFFER: {
          data.visual = DefaultVisual(data.display, screen);
          data.depth = DefaultDepth(data.display, screen);

          attr.colormap = XCreateColormap(
            data.display,
            data.root,
            data.visual,
            AllocNone
          );

          data.win = XCreateWindow(
            data.display,
            data.root,
            x, y,
            w, h,
            0,
            data.depth,
            InputOutput,
            data.visual,
            CWColormap | CWEventMask,
            &attr
          );

          if (data.win == None) {
            ::window::log_error(::window::LOG_X11, "Failed to create framebuffer window");
            destroy(data);
            return window::CREATIONFAILED;
          }

          break;
        }
        default: {
          ::window::log_error(::window::LOG_X11, "Unsupported gfx backend!");
          destroy(data);
          return ::window::UNSUPPORTED;
        }
      }

      data.wmDelete = XInternAtom(data.display, "WM_DELETE_WINDOW", False);
      XSetWMProtocols(data.display, data.win, &data.wmDelete, 1);

      XMapWindow(data.display, data.win);

      data.isopen = true;

      if (!appname.empty()) {
        XClassHint class_hint{};
        class_hint.res_name  = const_cast<char*>("window_api::window");
        class_hint.res_class = const_cast<char*>(appname.data());

        XSetClassHint(data.display, data.win, &class_hint);
      }

      XStoreName(data.display, data.win, title.data());

      data.width = w;
      data.height = h;

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

      void* ud = data.input->user_data;

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
          KeySym keysym = XLookupKeysym(&event.xkey, 0);

          key_descriptor key = os_to_key(keysym);

          if (data.input->key_event) {
            data.input->key_event(event.type == KeyPress, key, ud);
          }

          //unsigned int keycode = event.xkey.keycode;
          //printf("Key: %s (%s:0x%lx)\n", key.description, XKeysymToString(keysym), keysym);

          break;
        }
        case ButtonPress:
        case ButtonRelease: {
          if (!data.input)
            break;

          // Vertical Scrolling :)
          if (event.xbutton.button == 4) {
            if (data.input->vscroll_event != nullptr) {
              data.input->vscroll_event(-1, ud);
            }
            break;
          }

          if (event.xbutton.button == 5) {
            if (data.input->vscroll_event != nullptr) {
              data.input->vscroll_event(1, ud);
            }
            break;
          }

          // Horiziontal Scrolling :)
          if (event.xbutton.button == 6) {
            if (data.input->hscroll_event != nullptr) {
              data.input->hscroll_event(-1, ud);
            }
            break;
          }

          if (event.xbutton.button == 7) {
            if (data.input->hscroll_event != nullptr) {
              data.input->hscroll_event(1, ud);
            }
            break;
          }

          bool pressed = event.type == ButtonPress;
          button_descriptor desc = os_to_button(event.xbutton.button);

          // Only generate double click events on pressing the button,
          // not on releasing it
          if (data.input->dblclk_event && pressed) {
            timespec t; clock_gettime(CLOCK_MONOTONIC, &t);
            unsigned long now = t.tv_sec * 1000 + t.tv_nsec / 1000000;

            // The recommended delta time for a double click
            // from Microsoft is: 500ms
            // TODO hint for the delta time!
            if (now - data.input->last_click <= 500/*ms delta*/) {
              data.input->dblclk_event(desc, ud);
              // prevent the next click to be a double click
              data.input->last_click = now - 500;
              break;
            }
            data.input->last_click = now;
          }

          // if we have no double click handler, we just prepend,
          // that its a normal second mouse button click
          if (data.input->button_event) {
            data.input->button_event(
              pressed,
              desc,
              ud
            );
          }


          break;
        }
        case MotionNotify: {
          
          if (data.input->mouse_event != nullptr) {
            data.input->mouse_event(event.xmotion.x, event.xmotion.y, ud);
          }

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

    ::window::result make_gfx_context(window_x11_data& data, graphics_backend gfx_backend) noexcept {
      switch (gfx_backend) {
        case window::GRAPHICS_OPENGL: {
          // Nothing to do in x11, because we need to create the window
          // with a opengl context :)
          break;
        }
        case window::GRAPHICS_FRAMEBUFFER: {
          if (data.fb->pixels != nullptr) {
            ::window::log_error(::window::LOG_X11, "framebuffer already exists");
            return window::ALREADYEXISTS;
          }

          // create framebuffer with 1:1 ratio to window screen size
          return resize_framebuffer(data, gfx_backend, 0, 0);
        }
        default: {
          ::window::log_error(::window::LOG_X11, "Unsupported gfx backend!");
          destroy(data);
          return ::window::UNSUPPORTED;
        }
      }
      return window::SUCCESS;
    }

    ::window::result resize_framebuffer(window_x11_data& data, graphics_backend gfx_backend, int width, int height) noexcept {
      if (gfx_backend != GRAPHICS_FRAMEBUFFER) {
        ::window::log_error(::window::LOG_X11, "selected graphics backend is not suitable for resize_framebuffer");
        return window::UNSUPPORTED;
      }

      if (data.fb == nullptr) {
        ::window::log_error(::window::LOG_X11, "data.fb is a nullptr");
        return window::BADWINDOW;
      }

      if (data.display == nullptr || data.win == None) {
        ::window::log_error(::window::LOG_X11, "window/display is not ready");
        return window::BADWINDOW;
      }

      if (width <= 0 || height <= 0) {
        XWindowAttributes attr{};
        if (XGetWindowAttributes(data.display, data.win, &attr) == 0) {
          ::window::log_error(::window::LOG_X11, "failed to query window size");
          return window::BADWINDOW;
        }

        if (width <= 0) {
          width = attr.width;
        }

        if (height <= 0) {
          height = attr.height;
        }
      }

      if (width <= 0 || height <= 0) {
        ::window::log_error(::window::LOG_X11, "invalid framebuffer size");
        return window::BADWINDOW;
      }

      std::size_t pixel_count = static_cast<std::size_t>(width) * static_cast<std::size_t>(height);

      std::uint32_t* pixels = new (std::nothrow) std::uint32_t[pixel_count]();

      if (pixels == nullptr) {
        ::window::log_error(::window::LOG_X11, "failed to allocate framebuffer");
        return window::BADALLOC;
      }

      delete[] data.fb->pixels;

      data.fb->pixels = pixels;
      data.fb->width = width;
      data.fb->height = height;
      data.fb->stride = width * sizeof(std::uint32_t);

      return window::SUCCESS;
    }
    
    ::window::result swap_buffers(const window_x11_data& data, graphics_backend gfx_backend) noexcept {
      switch (gfx_backend) {
        case window::GRAPHICS_OPENGL: {
          glXSwapBuffers(data.display, data.win);
          break;
        }
        case window::GRAPHICS_FRAMEBUFFER: {
          if (data.fb == nullptr || data.fb->pixels == nullptr) {
            ::window::log_error(::window::LOG_X11, "framebuffer is not allocated");
            return window::BADWINDOW;
          }

          if (data.display == nullptr || data.win == None) {
            ::window::log_error(::window::LOG_X11, "window/display is not ready");
            return window::BADWINDOW;
          }

          XWindowAttributes attr{};
          if (XGetWindowAttributes(data.display, data.win, &attr) == 0) {
            ::window::log_error(::window::LOG_X11, "failed to query window size");
            return window::BADWINDOW;
          }

          int win_width = attr.width;
          int win_height = attr.height;

          if (win_width <= 0 || win_height <= 0) {
            return window::BADWINDOW;
          }

          if (data.fb->width <= 0 || data.fb->height <= 0) {
            ::window::log_error(::window::LOG_X11, "invalid framebuffer dimensions");
            return window::BADWINDOW;
          }

          std::size_t scaled_count = static_cast<std::size_t>(win_width) * static_cast<std::size_t>(win_height);

          std::uint32_t* scaled = new (std::nothrow) std::uint32_t[scaled_count];

          if (scaled == nullptr) {
            ::window::log_error(::window::LOG_X11, "failed to allocate blit buffer");
            return window::BADALLOC;
          }

          int src_width = data.fb->width;
          int src_height = data.fb->height;
          int src_stride = data.fb->stride > 0 ? data.fb->stride : data.fb->width;

          for (int y = 0; y < win_height; ++y) {
            int src_y = static_cast<int>(
              static_cast<long long>(y) * src_height / win_height
            );

            if (src_y >= src_height) {
              src_y = src_height - 1;
            }

            const std::uint32_t* src_row =
            data.fb->pixels + static_cast<std::size_t>(src_y) * src_stride;

            std::uint32_t* dst_row =
            scaled + static_cast<std::size_t>(y) * win_width;

            for (int x = 0; x < win_width; ++x) {
              int src_x = static_cast<int>(
                static_cast<long long>(x) * src_width / win_width
              );

              if (src_x >= src_width) {
                src_x = src_width - 1;
              }

              dst_row[x] = src_row[src_x];
            }
          }

          XImage* image = XCreateImage(
            data.display,
            data.visual,
            data.depth,
            ZPixmap,
            0,
            reinterpret_cast<char*>(scaled),
            win_width,
            win_height,
            32,
            0
          );

          if (image == nullptr) {
            delete[] scaled;
            ::window::log_error(::window::LOG_X11, "failed to create XImage");
            return window::BADALLOC;
          }

          GC gc = DefaultGC(data.display, DefaultScreen(data.display));

          XPutImage(
            data.display,
            data.win,
            gc,
            image,
            0, 0,
            0, 0,
            static_cast<unsigned int>(win_width),
            static_cast<unsigned int>(win_height)
          );

          XFlush(data.display);

          image->data = nullptr;
          XDestroyImage(image);
          delete[] scaled;

          return window::SUCCESS;
        }

        default: {
          ::window::log_error(::window::LOG_X11, "Unsupported gfx backend!");
          return window::UNSUPPORTED;
        }
      }

      return window::SUCCESS;
    }

    ::window::result swap_interval(const window_x11_data& data, graphics_backend gfx_backend, int interval) noexcept {
      if (gfx_backend != window::GRAPHICS_OPENGL) {
        ::window::log_warning(::window::LOG_X11, "wrong graphics backend selected");
        return window::UNSUPPORTED;
      }

      load_swap_interval();
      if (glXSwapIntervalEXT == nullptr) {
        ::window::log_error(::window::LOG_X11, "glXSwapIntervalEXT not supported");
        return window::UNSUPPORTED;
      }
      glXSwapIntervalEXT(data.display, data.win, interval);
      return window::SUCCESS;
    }

    void destroy(window_x11_data& data) noexcept {
      // framebuffer
      if (data.fb->pixels != nullptr) {
        delete[] data.fb->pixels;
        data.fb->pixels = nullptr;
      }
      // opengl
      if (data.context != nullptr) {
        glXMakeCurrent(data.display, None, nullptr);
        glXDestroyContext(data.display, data.context);
        data.context = nullptr;
      }
      // window
      if (data.win != None) {
        XDestroyWindow(data.display, data.win);
        data.isopen = false;
        data.win = None;
      }
      // display
      if (data.display != nullptr) {
        XCloseDisplay(data.display);
        data.display = nullptr;
      }
    }

  } // namespace x11

} // namespace window
