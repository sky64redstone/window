#include "include/window.hpp"

#include <stdio.h>

namespace window {

  namespace win32 {

    using gl_swap_interval_proc = BOOL(WINAPI*)(int interval);
    inline gl_swap_interval_proc wglSwapIntervalEXT = nullptr;

    inline static void load_swap_interval() noexcept {
      if (wglSwapIntervalEXT == nullptr) {
        wglSwapIntervalEXT = reinterpret_cast<gl_swap_interval_proc>(
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
      case WM_SIZE: {
        data->x = LOWORD(l);
        data->y = HIWORD(l);

        return 0;
      }
      case WM_GETMINMAXINFO: {
        MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(l);

        info->ptMinTrackSize.x = 100;
        info->ptMinTrackSize.y = 50;

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
    win32 = {};
    win32.win = nullptr;
    win32.dc = nullptr;
    win32.rc = nullptr;
    win32.isopen = false;
    win32.x = -1;
    win32.y = -1;
    win32.width = -1;
    win32.height = -1;

#define TO_STRING(x) #x
#define EXPAND_AND_STRINGIFY(x) TO_STRING(x)
    fprintf(stdout, "dll api type: %s\n", EXPAND_AND_STRINGIFY(window_api));
  }

  window::~window() noexcept {
    destroy();
  }
  
  result window::create(int width, int height, const char* title) noexcept {
    static ATOM classAtom = 0;

    HINSTANCE inst = GetModuleHandleA(nullptr);

    if (classAtom == 0) {
      WNDCLASSA wcls{
        .style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc   = win32::global_window_event,
        .cbClsExtra    = 0,
        .cbWndExtra    = 0,
        .hInstance     = inst,
        .hIcon         = LoadIcon(inst, IDI_APPLICATION),
        .hCursor       = LoadCursor(inst, IDC_ARROW),
        .hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
        .lpszMenuName  = nullptr,
        .lpszClassName = "window_api::window"
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
      fprintf(stderr, "[window] win32: Unable to get the device context (DC)!!!\n");
      return BADWINDOW;
    }

    constexpr PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR), 1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
      PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    int pf = ChoosePixelFormat(win32.dc, &pfd);
    if (pf == 0) {
      fprintf(stderr, "[window] win32: Unable to choose Pixel Format!!!\n");
      return UNKNOWNFAILURE;
    }

    if (SetPixelFormat(win32.dc, pf, &pfd) == FALSE) {
      fprintf(stderr, "[window] win32: Unable to set Pixel Format!!!\n");
      return UNKNOWNFAILURE;
    }

    win32.rc = wglCreateContext(win32.dc);
    if (win32.rc == NULL) {
      fprintf(stderr, "[window] win32: Unable to create WGL Context!!!\n");
      return CREATIONFAILED;
    }

    if (wglMakeCurrent(win32.dc, win32.rc) == FALSE) {
      fprintf(stderr, "[window] win32: Unable to make current Render Context!!!\n");
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
    win32::wglSwapIntervalEXT(interval);
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