#include "window/window.hpp"

#include <stdio.h>

#include "window/log.hpp"

namespace window {

  namespace win32 {

    #define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
    #define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
    #define WGL_CONTEXT_FLAGS_ARB         0x2094
    #define WGL_CONTEXT_PROFILE_MASK_ARB  0x9126

    #define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
    #define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

    #define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001

    using wglCreateContextAttribsARBProc = HGLRC(WINAPI*)(
      HDC hdc, HGLRC hShareContext, const int* attribList
    );
    inline wglCreateContextAttribsARBProc wglCreateContextAttribsARB = nullptr;

    inline static void load_wgl_create_context() noexcept {
      if (wglCreateContextAttribsARB == nullptr) {
        wglCreateContextAttribsARB =
          reinterpret_cast<wglCreateContextAttribsARBProc>(
            wglGetProcAddress("wglCreateContextAttribsARB")
          );
      }
    }

    using wglSwapIntervalEXTProc = BOOL(WINAPI*)(int interval);
    inline wglSwapIntervalEXTProc wglSwapIntervalEXT = nullptr;

    inline static void load_swap_interval() noexcept {
      if (wglSwapIntervalEXT == nullptr) {
        wglSwapIntervalEXT = reinterpret_cast<wglSwapIntervalEXTProc>(
          wglGetProcAddress("wglSwapIntervalEXT")
        );
      }
    }

    inline static window_win32_data* get_data(HWND wnd, UINT msg, LPARAM l) noexcept {
      window_win32_data* data;

      if (msg == WM_NCCREATE) {
        CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(l);
        data = static_cast<window_win32_data*>(create->lpCreateParams);
        if (data != nullptr) {
          SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
        }
      }
      else {
        data = reinterpret_cast<window_win32_data*>(GetWindowLongPtr(wnd, GWLP_USERDATA));
      }

      return data;
    }

    inline static LRESULT CALLBACK global_window_event(HWND wnd, UINT msg, WPARAM w, LPARAM l) noexcept {
      window_win32_data* data = get_data(wnd, msg, l);
      if (!data) return DefWindowProc(wnd, msg, w, l);
      // get user data
      void* ud = data->input->user_data;

      switch (msg) {
      case WM_CREATE: {
        RECT rect{};
        GetWindowRect(wnd, &rect);
        data->x = rect.left;
        data->y = rect.top;
        data->width = rect.right - rect.left;
        data->height = rect.bottom - rect.top;

        return 0;
      }
      case WM_SETCURSOR: {
        SetCursor(LoadCursor(nullptr, IDC_ARROW));
        return 0;
      }
      case WM_SIZE: {
        data->width  = LOWORD(l);
        data->height = HIWORD(l);

        if (data->input && data->input->size_event) {
          data->input->size_event(data->width, data->height, ud);
        }

        return 0;
      }
      case WM_GETMINMAXINFO: {
        MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(l);

        info->ptMinTrackSize.x = 100;
        info->ptMinTrackSize.y = 50;

        return 0;
      }
      case WM_KEYDOWN:
      case WM_KEYUP: {
        if (data->input && data->input->key_event) {
          key_descriptor descriptor = ::window::os_to_key(w);

          data->input->key_event(msg == WM_KEYDOWN, descriptor, ud);
        }

        return 0;
      }
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP: {
        if (data->input && data->input->key_event) {
          key_descriptor descriptor = ::window::os_to_key(w);

          data->input->key_event(msg == WM_SYSKEYDOWN, descriptor, ud);
        }
        return 0;
      }
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      case WM_XBUTTONDOWN:
      case WM_XBUTTONUP: {
        if (data->input && data->input->button_event) {
          int btn = 0;
          bool down;

          switch (msg) {
          case WM_LBUTTONDOWN: btn = VK_LBUTTON; down = true; break;
          case WM_LBUTTONUP: btn = VK_LBUTTON; down = false; break;
          case WM_RBUTTONDOWN: btn = VK_RBUTTON; down = true; break;
          case WM_RBUTTONUP: btn = VK_RBUTTON; down = false; break;
          case WM_MBUTTONDOWN: btn = VK_MBUTTON; down = true; break;
          case WM_MBUTTONUP: btn = VK_MBUTTON; down = false; break;
          case WM_XBUTTONDOWN: btn = VK_XBUTTON1 + HIWORD(w) - 1; down = true; break;
          case WM_XBUTTONUP: btn = VK_XBUTTON1 + HIWORD(w) - 1; down = false; break;
          }

          button_descriptor descriptor = ::window::os_to_button(btn);

          data->input->button_event(down, descriptor, ud);
        }
        return 0;
      }
      case WM_LBUTTONDBLCLK:
      case WM_RBUTTONDBLCLK:
      case WM_MBUTTONDBLCLK:
      case WM_XBUTTONDBLCLK: {
        if (data->input) {
          int btn = 0;

          switch (msg) {
            case WM_LBUTTONDBLCLK: btn = VK_LBUTTON; break;
            case WM_RBUTTONDBLCLK: btn = VK_RBUTTON; break;
            case WM_MBUTTONDBLCLK: btn = VK_MBUTTON; break;
            case WM_XBUTTONDBLCLK: btn = VK_XBUTTON1 + HIWORD(w) - 1; break;
          }

          button_descriptor descriptor = ::window::os_to_button(btn);

          if (data->input->dblclk_event != nullptr)
            data->input->dblclk_event(descriptor, ud);
          // if we have no double click handler, we just prepend,
          // that its a normal second mouse button click
          else if (data->input->button_event != nullptr)
            data->input->button_event(true, descriptor, ud);
        }

        return 0;
      }
      case WM_MOUSEMOVE: {
        if (data->input && data->input->mouse_event) {
          int xPos = LOWORD(l);
          int yPos = HIWORD(l);

          data->input->mouse_event(xPos, yPos, ud);
        }
        return 0;
      }
      case WM_MOUSEWHEEL: {
        if (data->input && data->input->vscroll_event) {
          float wheel_delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(w)) / static_cast<float>(WHEEL_DELTA);

          data->input->vscroll_event(wheel_delta, ud);
        }
        return 0;
      }
      case WM_MOUSEHWHEEL: {
        if (data->input && data->input->hscroll_event) {
          float wheel_delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(w)) / static_cast<float>(WHEEL_DELTA);

          data->input->hscroll_event(wheel_delta, ud);
        }
        return 0;
      }
      case WM_CLOSE: {
        data->isopen = false;

        return 0;
      }
      case WM_DESTROY: {
        PostQuitMessage(0);

        return 0;
      }
      }

      return DefWindowProc(wnd, msg, w, l);
    }
  }

  window::window() noexcept {
    os_to_key(0); // pre load keys
    input = {};
    hintmem = {};
    win32 = {};
    win32.input = &input;
    win32.win = nullptr;
    win32.dc = nullptr;
    win32.rc = nullptr;
    win32.isopen = false;
    win32.x = -1;
    win32.y = -1;
    win32.width = -1;
    win32.height = -1;
  }

  window::~window() noexcept {
    destroy();
  }
  
  result window::create(int width, int height, const char* title) noexcept {
    static ATOM classAtom = 0;

    HINSTANCE inst = GetModuleHandleA(nullptr);

    if (classAtom == 0) {
      WNDCLASSA wcls{
        .style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS,
        .lpfnWndProc   = win32::global_window_event,
        .cbClsExtra    = 0,
        .cbWndExtra    = 0,
        .hInstance     = inst,
        .hIcon         = LoadIcon(inst, IDI_APPLICATION),
        .hCursor       = LoadCursor(inst, IDC_ARROW),
        .hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
        .lpszMenuName  = nullptr,
        .lpszClassName = hintmem.appname ? hintmem.appname : "window_api::window"
      };

      classAtom = RegisterClassA(&wcls);
    }

    constexpr DWORD exstyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    constexpr DWORD style   = WS_TILEDWINDOW | WS_VISIBLE;

    // Calculate the client size
    RECT rect = { 0, 0, width, height };
    AdjustWindowRectEx(&rect, style, FALSE, exstyle);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    win32.win = CreateWindowExA(
      exstyle,
      MAKEINTATOM(classAtom),
      title,
      style,
      CW_USEDEFAULT, CW_USEDEFAULT,
      width, height,
      nullptr, nullptr,
      inst,
      reinterpret_cast<LPVOID>(&win32)
    );

    if (win32.win == NULL) {
      fprintf(stderr, "[window] win32: Couldn't open a window!!!\n");
      return CREATIONFAILED;
    }

    win32.isopen = true;

    return SUCCESS;
  }

  result window::set_title(const char* title) const noexcept {
    BOOL success = SetWindowTextA(win32.win, title);
    return success != 0 ? SUCCESS : UNKNOWNFAILURE;
  }

  result window::set_size(int w, int h) noexcept {
    BOOL success = SetWindowPos(win32.win, nullptr, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
    return success != 0 ? SUCCESS : UNKNOWNFAILURE;
  }

  bool window::is_open() const noexcept {
    return win32.isopen;
  }

  result window::poll_events() noexcept {
    MSG msg;
    while (PeekMessage(&msg, win32.win, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        return QUIT;
      }
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    return SUCCESS;
  }

  result window::make_opengl_context() noexcept {
    win32.dc = GetDC(win32.win);

    if (win32.dc == NULL) {
      log_error(LOG_WIN, "Unable to get the device context (DC)");
      return BADWINDOW;
    }

    PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW |
      PFD_SUPPORT_OPENGL |
      PFD_DOUBLEBUFFER,
      PFD_TYPE_RGBA,
      32,
      0,0,0,0,0,0,
      0,
      0,
      0,
      0,0,0,0,
      24, // depth
      8,  // stencil
      0,
      PFD_MAIN_PLANE,
      0,
      0,0,0
    };

    int pf = ChoosePixelFormat(win32.dc, &pfd);
    if (!pf) {
      log_error(LOG_WIN, "Unable to choose Pixel Format");
      return UNKNOWNFAILURE;
    }

    if (!SetPixelFormat(win32.dc, pf, &pfd)) {
      log_error(LOG_WIN, "Unable to set Pixel Format");
      return UNKNOWNFAILURE;
    }

    // do we need to create a specific context?
    if (hintmem.glvmajor > 0 && hintmem.glvminor >= 0) {
      // create temp context for loading wglCreateContextAttribsARB
      HGLRC temp = nullptr;
      if (!win32::wglCreateContextAttribsARB) {
        temp = wglCreateContext(win32.dc);
        if (!temp) {
          log_error(LOG_WIN, "Unable to create temporary WGL Context");
          return CREATIONFAILED;
        }

        if (!wglMakeCurrent(win32.dc, temp)) {
          log_error(LOG_WIN, "Unable to make temporary current Render Context\n");
          return UNKNOWNFAILURE;
        }

        win32::load_wgl_create_context();

        if (!win32::wglCreateContextAttribsARB) {
          log_error(LOG_WIN, "wglCreateContextAttribsARB not supported");
          return UNSUPPORTED;
        }
      }

      int attribs[] = {
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB,

        WGL_CONTEXT_MAJOR_VERSION_ARB, hintmem.glvmajor,
        WGL_CONTEXT_MINOR_VERSION_ARB, hintmem.glvminor,

        #ifdef _DEBUG
        WGL_CONTEXT_FLAGS_ARB,
        WGL_CONTEXT_DEBUG_BIT_ARB,
        #endif

        0
      };

      // create now the real context with the right version

      HGLRC modern = win32::wglCreateContextAttribsARB(win32.dc, 0, attribs);

      if (!modern) {
        log_error(LOG_WIN, "Failed to create OpenGL %i.%i context", hintmem.glvmajor, hintmem.glvminor);
        return CREATIONFAILED;
      }

      // destroy and replace the temporary context
      if (temp) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(temp);
      }

      if (!wglMakeCurrent(win32.dc, modern)) {
        log_error(LOG_WIN, "Failed to activate modern context");
        return UNKNOWNFAILURE;
      }

      win32.rc = modern;

      return SUCCESS;
    }

    // create default context
    win32.rc = wglCreateContext(win32.dc);
    if (win32.rc == NULL) {
      log_error(LOG_WIN, "Unable to create WGL Context");
      return CREATIONFAILED;
    }

    if (wglMakeCurrent(win32.dc, win32.rc) == FALSE) {
      log_error(LOG_WIN, "Unable to make current Render Context");
      return UNKNOWNFAILURE;
    }

    return SUCCESS;
  }

  result window::swap_buffers() const noexcept {
    BOOL success = SwapBuffers(win32.dc);
    return success ? SUCCESS : UNKNOWNFAILURE;
  }

  result window::swap_interval(int interval) const noexcept {
    win32::load_swap_interval();
    if (win32::wglSwapIntervalEXT == nullptr) {
      fprintf(stderr, "[window] win32: The function wglSwapIntervalEXT seems to be unsupported!!!\n");
      return UNSUPPORTED;
    }
    BOOL success = win32::wglSwapIntervalEXT(interval);
    return success ? SUCCESS : UNKNOWNFAILURE;
  }

  void* window::set_user_data(void* data) noexcept {
    void* temp = input.user_data;
    input.user_data = data;
    return temp;
  }

  key_event_callback window::set_key_event(key_event_callback func) noexcept {
    key_event_callback temp = input.key_event;
    input.key_event = func;
    return temp;
  }

  button_event_callback window::set_btn_event(button_event_callback func) noexcept {
    button_event_callback temp = input.button_event;
    input.button_event = func;
    return temp;
  }

  dblclk_event_callback window::set_dblclk_event(dblclk_event_callback func) noexcept {
    dblclk_event_callback temp = input.dblclk_event;
    input.dblclk_event = func;
    return temp;
  }

  mouse_event_callback window::set_mouse_event(mouse_event_callback func) noexcept {
    mouse_event_callback temp = input.mouse_event;
    input.mouse_event = func;
    return temp;
  }

  scroll_event_callback window::set_vscroll_event(scroll_event_callback func) noexcept {
    scroll_event_callback temp = input.vscroll_event;
    input.vscroll_event = func;
    return temp;
  }

  scroll_event_callback window::set_hscroll_event(scroll_event_callback func) noexcept {
    scroll_event_callback temp = input.hscroll_event;
    input.hscroll_event = func;
    return temp;
  }

  size_event_callback window::set_size_event(size_event_callback func) noexcept {
    size_event_callback temp = input.size_event;
    input.size_event = func;
    return temp;
  }

  void window::set_appname(const char* appname) noexcept {
    hintmem.appname = appname;
  }

  void window::set_glversion(int major, int minor) noexcept {
    hintmem.glvmajor = major;
    hintmem.glvminor = minor;
  }

  backend window::get_backend() const noexcept {
    return ::window::backend::WINDOWS;
  }

  void window::destroy() noexcept {
    if (win32.rc != nullptr) {
      wglDeleteContext(win32.rc);
      //win32.rc = nullptr;
    }
    if (win32.dc != nullptr) {
      ReleaseDC(win32.win, win32.dc);
      //win32.dc = nullptr;
    }
    if (IsWindow(win32.win)) {
      DestroyWindow(win32.win);
    }
    win32.isopen = false;
  }
}
