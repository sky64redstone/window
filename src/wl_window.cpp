#include "include/wayland.hpp"

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

namespace window::wl {
  const char* eglGetErrorString(EGLint error) {
    switch (error) {
    case EGL_SUCCESS:
      return "EGL_SUCCESS";
    case EGL_NOT_INITIALIZED:
      return "EGL_NOT_INITIALIZED";
    case EGL_BAD_ACCESS:
      return "EGL_BAD_ACCESS";
    case EGL_BAD_ALLOC:
      return "EGL_BAD_ALLOC";
    case EGL_BAD_ATTRIBUTE:
      return "EGL_BAD_ATTRIBUTE";
    case EGL_BAD_CONTEXT:
      return "EGL_BAD_CONTEXT";
    case EGL_BAD_CONFIG:
      return "EGL_BAD_CONFIG";
    case EGL_BAD_CURRENT_SURFACE:
      return "EGL_BAD_CURRENT_SURFACE";
    case EGL_BAD_DISPLAY:
      return "EGL_BAD_DISPLAY";
    case EGL_BAD_SURFACE:
      return "EGL_BAD_SURFACE";
    case EGL_BAD_MATCH:
      return "EGL_BAD_MATCH";
    case EGL_BAD_PARAMETER:
      return "EGL_BAD_PARAMETER";
    case EGL_BAD_NATIVE_PIXMAP:
      return "EGL_BAD_NATIVE_PIXMAP";
    case EGL_BAD_NATIVE_WINDOW:
      return "EGL_BAD_NATIVE_WINDOW";
    case EGL_CONTEXT_LOST:
      return "EGL_CONTEXT_LOST";
    default:
      return "Unknown EGL error";
    }
  }

  static void xdg_wm_base_ping(void* data, xdg_wm_base* xwm_base, uint32_t serial) {
    xdg_wm_base_pong(xwm_base, serial);
  }

  static const struct xdg_wm_base_listener wm_base_listener = {
    .ping = xdg_wm_base_ping,
  };

  static void registry_handler(void* data, wl_registry* registry,
                               uint32_t id, const char* interface, uint32_t version) {
    if (data != nullptr) {
      window_wl_data* win_data = static_cast<window_wl_data*>(data);
      if (strcmp(interface, wl_compositor_interface.name) == 0) {
        win_data->compositor = static_cast<wl_compositor*>(
          wl_registry_bind(registry, id, &wl_compositor_interface, 4)
        );
      } else if (strcmp(interface, wl_seat_interface.name) == 0) {
        win_data->seat = static_cast<wl_seat*>(
          wl_registry_bind(registry, id, &wl_seat_interface, 5)
        );
      } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        win_data->xwm_base = static_cast<xdg_wm_base*>(
          wl_registry_bind(registry, id, &xdg_wm_base_interface, 1)
        );
        xdg_wm_base_add_listener(win_data->xwm_base, &wm_base_listener, NULL);
      } else if (strcmp(interface, zxdg_decoration_manager_v1_interface.name) == 0) {
        win_data->decoration_manager = static_cast<zxdg_decoration_manager_v1*>(
          wl_registry_bind(registry, id, &zxdg_decoration_manager_v1_interface, 1)
        );
      }
    }
  }

  static void registry_remover(void* data, struct wl_registry* registry, uint32_t id) {}

  static const struct wl_registry_listener registry_listener = {
    .global = registry_handler,
    .global_remove = registry_remover,
  };

  static void xdg_surface_configure(void* data, xdg_surface* surface, uint32_t serial) {
    window_wl_data* win = static_cast<window_wl_data*>(data);

    xdg_surface_ack_configure(surface, serial);

    xdg_surface_set_window_geometry(
      win->xsurface,
      0, 0,
      win->width,
      win->height);

    wl_surface_commit(win->surface);
  }
  
  static const xdg_surface_listener xsurface_listener = {
    .configure = xdg_surface_configure
  };

  static void xdg_toplevel_close(void* data, xdg_toplevel* toplevel) {
    window_wl_data* win = static_cast<window_wl_data*>(data);
    win->isopen = false;
  }

  static void xdg_toplevel_configure(void* data, xdg_toplevel* toplevel, 
      int32_t width, int32_t height, wl_array* states) {
    window_wl_data* win = static_cast<window_wl_data*>(data);

    // Some compositors send 0,0 meaning "pick your own size"
    if (width <= 0 || height <= 0)
      return;

    win->width = width;
    win->height = height;

    // Resize EGL window if it exists
    if (win->egl_window)
      wl_egl_window_resize(win->egl_window, width, height, 0, 0);

    if (win->input && win->input->size_event)
      win->input->size_event(width, height);

    // Update surface geometry in xdg_surface.configure
    //xdg_surface_set_window_geometry(win->xsurface, 0, 0, width, height);
    //wl_surface_commit(win->surface);
  }

  static const xdg_toplevel_listener toplevel_listener = {
    .configure = xdg_toplevel_configure,
    .close = xdg_toplevel_close
  };

  static void keyboard_keymap(void* data, wl_keyboard* keyboard,
                              uint32_t format, int fd, uint32_t size) {
    window_wl_data* win = static_cast<window_wl_data*>(data);

    if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
      close(fd);
      return;
    }

    char* map = (char*)mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);

    win->kb_ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);

    win->kb_keymap = xkb_keymap_new_from_string(
      win->kb_ctx,
      map,
      XKB_KEYMAP_FORMAT_TEXT_V1,
      XKB_KEYMAP_COMPILE_NO_FLAGS);

    munmap(map, size);
    close(fd);

    win->kb_state = xkb_state_new(win->kb_keymap);
  }

  static void keyboard_enter(void* data, wl_keyboard* keyboard, uint32_t serial, wl_surface* surface, wl_array* keys) {
  }

  static void keyboard_leave(void* data, wl_keyboard* keyboard, uint32_t serial, wl_surface* surface) {
  }

  static void keyboard_key(void* data, wl_keyboard* keyboard, uint32_t serial,
                           uint32_t time, uint32_t key, uint32_t state) {
    window_wl_data* win = static_cast<window_wl_data*>(data);

    xkb_keysym_t sym = xkb_state_key_get_one_sym(win->kb_state, key + 8);

    key_descriptor desc = os_to_key(sym);

    if (win->input && win->input->key_event) {
      win->input->key_event(
        state == WL_KEYBOARD_KEY_STATE_PRESSED,
        desc
      );
    }
  }

  static void keyboard_modifiers(void* data, wl_keyboard* keyboard, uint32_t serial,
                                 uint32_t mods_depressed, uint32_t mods_latched,
                                 uint32_t mods_locked, uint32_t group) {
    window_wl_data* win = static_cast<window_wl_data*>(data);

    xkb_state_update_mask(
      win->kb_state,
      mods_depressed,
      mods_latched,
      mods_locked,
      0, 0,
      group
    );
  }

  static void keyboard_repeat_info(void* data, wl_keyboard* keyboard, int32_t rate, int32_t delay) {
  }

  static const wl_keyboard_listener keyboard_listener = {
    .keymap = keyboard_keymap,
    .enter = keyboard_enter,
    .leave = keyboard_leave,
    .key = keyboard_key,
    .modifiers = keyboard_modifiers,
    .repeat_info = keyboard_repeat_info
  };

  static void pointer_enter(void* data, wl_pointer* pointer, uint32_t serial,
                            wl_surface* surface, wl_fixed_t sx, wl_fixed_t sy) {
  }

  static void pointer_leave(void* data, wl_pointer* pointer,
                            uint32_t serial, wl_surface* surface) {
  }

  static void pointer_motion(void* data, wl_pointer* pointer, uint32_t time,
                             wl_fixed_t sx, wl_fixed_t sy) {
    window_wl_data* win = static_cast<window_wl_data*>(data);

    if (win->input && win->input->mouse_event) {
      win->input->mouse_event(
        wl_fixed_to_int(sx),
        wl_fixed_to_int(sy)
      );
    }
  }

  static void pointer_button(void* data, wl_pointer* pointer, uint32_t serial,
                             uint32_t time, uint32_t button, uint32_t state) {
    window_wl_data* win = static_cast<window_wl_data*>(data);

    button_descriptor desc = os_to_button(button);

    if (win->input && win->input->button_event) {
      win->input->button_event(
        state == WL_POINTER_BUTTON_STATE_PRESSED,
        desc
      );
    }
  }

  static void pointer_axis(void* data, wl_pointer* pointer, uint32_t time,
                           uint32_t axis, wl_fixed_t value) {
    window_wl_data* win = static_cast<window_wl_data*>(data);

    int scroll = wl_fixed_to_int(value);

    if (axis == WL_POINTER_AXIS_VERTICAL_SCROLL) {
      if (win->input->vscroll_event)
        win->input->vscroll_event(scroll);
    }

    if (axis == WL_POINTER_AXIS_HORIZONTAL_SCROLL) {
      if (win->input->hscroll_event)
        win->input->hscroll_event(scroll);
    }
  }

  static void pointer_frame(void* data, wl_pointer* pointer) {
  }

  static void pointer_axis_source(void* data, wl_pointer* pointer, uint32_t source) {
  }

  static void pointer_axis_stop(void* data, wl_pointer* pointer, uint32_t time, uint32_t axis) {
  }

  static void pointer_axis_discrete(void* data, wl_pointer* pointer, uint32_t axis, int32_t discrete) {
  }

  static const wl_pointer_listener pointer_listener = {
    .enter = pointer_enter,
    .leave = pointer_leave,
    .motion = pointer_motion,
    .button = pointer_button,
    .axis = pointer_axis,
    .frame = pointer_frame,
    .axis_source = pointer_axis_source,
    .axis_stop = pointer_axis_stop,
    .axis_discrete = pointer_axis_discrete
  };

  ::window::result create_window(window_wl_data& data, int w, int h, const char* title) noexcept {
    data.display = wl_display_connect(NULL);
    if (data.display == nullptr) {
      fprintf(stderr, "[window] wayland: Failed to connect to Wayland display\n");
      return ::window::CONNECTIONFAILED;
    }

    data.registry = wl_display_get_registry(data.display);
    wl_registry_add_listener(data.registry, &registry_listener, reinterpret_cast<void*>(&data));
    wl_display_roundtrip(data.display);
    wl_display_roundtrip(data.display); // Needed to receive xdg_wm_base events

    if (data.compositor == nullptr) {
      fprintf(stderr, "[window] wayland: Missing required Wayland interfaces (compositor)\n");
      return ::window::UNSUPPORTED;
    }

    if (data.xwm_base == nullptr) {
      fprintf(stderr, "[window] wayland: Missing required Wayland interfaces"
                      "(xdg window manager base)\n");
      return ::window::UNSUPPORTED;
    }

    data.surface = wl_compositor_create_surface(data.compositor);
    if (data.surface == nullptr) {
      fprintf(stderr, "[window] wayland: Failed to create surface\n");
      return ::window::CREATIONFAILED;
    }
      
    // Create xdg-surface and toplevel
    data.xsurface = xdg_wm_base_get_xdg_surface(data.xwm_base, data.surface);
    if (data.xsurface == nullptr) {
      fprintf(stderr, "[window] wayland: Failed to create xdg surface\n");
      return ::window::CREATIONFAILED;
    }
    xdg_surface_add_listener(data.xsurface, &xsurface_listener, &data);

    data.xtoplevel = xdg_surface_get_toplevel(data.xsurface);
    if (data.xtoplevel == nullptr) {
      fprintf(stderr, "[window] wayland: Failed to get xdg toplevel\n");
      return ::window::CREATIONFAILED;
    }
    xdg_toplevel_add_listener(data.xtoplevel, &toplevel_listener, &data);

    // add keyboard and mouse
    if (data.seat) {
      data.keyboard = wl_seat_get_keyboard(data.seat);
      wl_keyboard_add_listener(data.keyboard, &keyboard_listener, &data);
      data.pointer = wl_seat_get_pointer(data.seat);
      wl_pointer_add_listener(data.pointer, &pointer_listener, &data);
    }

    // set dimensions, frame and title of the window :)
    if (data.decoration_manager) {
      data.decoration = zxdg_decoration_manager_v1_get_toplevel_decoration(
        data.decoration_manager,
        data.xtoplevel
      );

      zxdg_toplevel_decoration_v1_set_mode(
        data.decoration,
        ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE
      );
    }
    xdg_surface_set_window_geometry(data.xsurface, 0, 0, w, h);
    xdg_toplevel_set_title(data.xtoplevel, title);
    wl_surface_commit(data.surface);

    data.isopen = true;
    data.width = w;
    data.height = h;

    return ::window::SUCCESS;
  }

  ::window::result set_title(const window_wl_data& data, const char* title) noexcept {
    xdg_toplevel_set_title(data.xtoplevel, title);
    wl_surface_commit(data.surface);
    return ::window::SUCCESS;
  }

  ::window::result set_size(window_wl_data& data, int w, int h) noexcept {
    xdg_surface_set_window_geometry(data.xsurface, 0, 0, w, h);
    // update egl
    if (data.egl_window != nullptr) {
      wl_egl_window_resize(data.egl_window, w, h, 0, 0);
    }

    wl_surface_damage_buffer(data.surface, 0, 0, w, h);
    wl_surface_commit(data.surface);

    data.width = w;
    data.height = h;

    return ::window::SUCCESS;
  }

  ::window::result poll_events(window_wl_data& data) noexcept {
    int result = wl_display_dispatch(data.display);
    if (result == -1) {
      data.isopen = false;
      return ::window::QUIT;
    }
    return result < 0 ? ::window::UNKNOWNFAILURE : ::window::SUCCESS;
  }
    
  ::window::result make_opengl_context(window_wl_data& data) noexcept {
    data.egl_display = eglGetDisplay((EGLNativeDisplayType)data.display);
    if (data.egl_display == EGL_NO_DISPLAY) {
      fprintf(stderr, "[window] wayland: Failed to connect to egl display server!!!\n");
      return ::window::CONNECTIONFAILED;
    }

    if (eglInitialize(data.egl_display, nullptr, nullptr) == false) {
      EGLint error = eglGetError();
      fprintf(stderr, 
        "[window] wayland: Failed to Inizialize EGL (code:0x%x=%s)\n",
        error, eglGetErrorString(error)
      );
      return ::window::UNKNOWNFAILURE;
    }

    if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE) {
      EGLint error = eglGetError();
      fprintf(stderr,
        "[window] wayland: Failed to bind OpenGL API (code 0x%x=%s)\n",
        error, eglGetErrorString(error)
      );
      return ::window::UNKNOWNFAILURE;
    }

    EGLint attr[] = {
      //EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
      EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
      EGL_RED_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_SURFACE_TYPE,
      EGL_WINDOW_BIT,
      EGL_NONE
    };

    EGLint configs = 0;
    if (eglChooseConfig(data.egl_display, attr, &data.egl_config, 1, &configs) == false) {
      EGLint error = eglGetError();
      fprintf(stderr, 
        "[window] wayland: Failed to choose EGL Config (code: 0x%x=%s)\n", 
        error, eglGetErrorString(error)
      );
      return ::window::UNKNOWNFAILURE;
    }

    if (configs == 0) {
      fprintf(stderr, 
        "[window] wayland: Failed to choose EGL Config (no valid available)\n"
      );
      return ::window::UNKNOWNFAILURE;
    }

    EGLint ctxattr[] = {
      //EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_CONTEXT_MAJOR_VERSION, 4,
      EGL_CONTEXT_MINOR_VERSION, 5,
      EGL_NONE
    };

    data.egl_context = eglCreateContext(data.egl_display, data.egl_config, EGL_NO_CONTEXT, ctxattr);

    if (data.egl_context == EGL_NO_CONTEXT) {
      EGLint error = eglGetError();
      fprintf(stderr,
        "[window] wayland: Failed to create EGL Context (code 0x%x=%s)\n",
        error, eglGetErrorString(error)
      );
      return ::window::CREATIONFAILED;
    }

    data.egl_window = wl_egl_window_create(data.surface, data.width, data.height);
    if (data.egl_window == nullptr) {
      EGLint error = eglGetError();
      fprintf(stderr,
        "[window] wayland: Failed to create EGL Window (code 0x%x=%s)\n",
        error, eglGetErrorString(error)
      );
      return ::window::CREATIONFAILED;
    }

    data.egl_surface = eglCreateWindowSurface(data.egl_display, data.egl_config, data.egl_window, NULL);
    if (data.egl_surface == EGL_NO_SURFACE) {
      EGLint error = eglGetError();
      fprintf(stderr,
        "[window] wayland: Failed to create EGL Surface (code 0x%x=%s)\n",
        error, eglGetErrorString(error)
      );
      return ::window::CREATIONFAILED;
    }
      
    if (eglMakeCurrent(data.egl_display, data.egl_surface, data.egl_surface, data.egl_context) == false) {
      EGLint error = eglGetError();
      fprintf(stderr,
        "[window] wayland: Failed to attach an EGL rendering context to EGL surfaces (code 0x%x=%s)\n",
        error, eglGetErrorString(error)
      );
      return ::window::CREATIONFAILED;
    }

    return ::window::SUCCESS;
  }

  ::window::result swap_buffers(const window_wl_data& data) noexcept {
    if (eglSwapBuffers(data.egl_display, data.egl_surface) == false) {
      EGLint error = eglGetError();
      fprintf(stderr,
        "[window] wayland: Failed to swap buffers (code 0x%x=%s)\n",
        error, eglGetErrorString(error)
      );
      return ::window::BADWINDOW;
    }
    return ::window::SUCCESS;
  }

  ::window::result swap_interval(const window_wl_data& data, int interval) noexcept {
    if (eglSwapInterval(data.egl_display, interval) == false) {
      EGLint error = eglGetError();
      fprintf(stderr,
        "[window] wayland: Failed to set swap interval (code 0x%x=%s)\n",
        error, eglGetErrorString(error)
      );
      return ::window::BADWINDOW;
    }
    return ::window::SUCCESS;
  }

  void destroy(window_wl_data& data) noexcept {
    if (data.decoration != nullptr) {
      zxdg_toplevel_decoration_v1_destroy(data.decoration);
      data.decoration = nullptr;
    }
    if (data.decoration_manager != nullptr) {
      zxdg_decoration_manager_v1_destroy(data.decoration_manager);
      data.decoration_manager = nullptr;
    }
    if (data.kb_state != nullptr) {
      xkb_state_unref(data.kb_state);
      data.kb_state = nullptr;
    }
    if (data.kb_keymap != nullptr) {
      xkb_keymap_unref(data.kb_keymap);
      data.kb_keymap = nullptr;
    }
    if (data.kb_ctx != nullptr) {
      xkb_context_unref(data.kb_ctx);
      data.kb_ctx = nullptr;
    }
    if (data.keyboard != nullptr) {
      wl_keyboard_destroy(data.keyboard);
      data.keyboard = nullptr;
    }
    if (data.pointer != nullptr) {
      wl_pointer_destroy(data.pointer);
      data.pointer = nullptr;
    }
    if (data.seat != nullptr) {
      wl_seat_destroy(data.seat);
      data.seat = nullptr;
    }
    if (data.egl_display != nullptr) {
      eglDestroySurface(data.egl_display, data.egl_surface);
      wl_egl_window_destroy(data.egl_window);
      eglDestroyContext(data.egl_display, data.egl_context);
      eglTerminate(data.egl_display);

      data.egl_display = nullptr;
      data.egl_window  = nullptr;
      data.egl_config  = nullptr;
      data.egl_context = nullptr;
      data.egl_surface = nullptr;
    }
    if (data.display) {
      xdg_wm_base_destroy(data.xwm_base);
      xdg_toplevel_destroy(data.xtoplevel);
      xdg_surface_destroy(data.xsurface);

      wl_surface_destroy(data.surface);
      wl_registry_destroy(data.registry);
      wl_display_disconnect(data.display);

      data.display      = nullptr;
      data.compositor   = nullptr;
      data.surface      = nullptr;
      data.registry     = nullptr;

      data.xwm_base     = nullptr;
      data.xsurface     = nullptr;
      data.xtoplevel    = nullptr;
    }
    data.isopen = false;
  }
}
