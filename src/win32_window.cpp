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
        data->width  = LOWORD(l);
        data->height = HIWORD(l);

        if (data->input->size_event != nullptr) {
          data->input->size_event(data->width, data->height);
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
        key_descriptor descriptor = ::window::os_to_key(w);
        data->input->keys[descriptor.k] = input_char(msg == WM_KEYDOWN, l & (1 << 30));

        if (data->input->key_event != nullptr)
          data->input->key_event(msg == WM_KEYDOWN, descriptor);

        return 0;
      }
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP: {
        key_descriptor descriptor = ::window::os_to_key(w);
        data->input->keys[descriptor.k] = input_char(msg == WM_SYSKEYDOWN, l & (1 << 30));

        if (data->input->key_event != nullptr)
          data->input->key_event(msg == WM_SYSKEYDOWN, descriptor);

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
        data->input->buttons[descriptor.b] = down;

        if (data->input->button_event != nullptr)
          data->input->button_event(down, descriptor);

        return 0;
      }
      case WM_LBUTTONDBLCLK:
      case WM_RBUTTONDBLCLK:
      case WM_MBUTTONDBLCLK:
      case WM_XBUTTONDBLCLK: {
        int btn = 0;

        switch (msg) {
        case WM_LBUTTONDBLCLK: btn = VK_LBUTTON; break;
        case WM_RBUTTONDBLCLK: btn = VK_RBUTTON; break;
        case WM_MBUTTONDBLCLK: btn = VK_MBUTTON; break;
        case WM_XBUTTONDBLCLK: btn = VK_XBUTTON1 + HIWORD(w) - 1; break;
        }

        button_descriptor descriptor = ::window::os_to_button(btn);
        data->input->buttons[descriptor.b] = true;

        if (data->input->dblclk_event != nullptr)
          data->input->dblclk_event(descriptor);
        // if we have no double click handler, we just prepend, 
        // that its a normal second mouse button click
        else if (data->input->button_event != nullptr)
          data->input->button_event(true, descriptor);

        return 0;
      }
      case WM_MOUSEMOVE: {
        int xPos = LOWORD(l);
        int yPos = HIWORD(l);

        data->input->mouseX = xPos;
        data->input->mouseY = yPos;

        if (data->input->mouse_event != nullptr)
          data->input->mouse_event(xPos, yPos);

        return 0;
      }
      case WM_MOUSEWHEEL: {
        float wheel_delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(w)) / static_cast<float>(WHEEL_DELTA);

        data->input->mouse_wheel += wheel_delta;

        if (data->input->wheel_event != nullptr)
          data->input->wheel_event(wheel_delta);

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
    win32 = {};
    win32.win = nullptr;
    win32.dc = nullptr;
    win32.rc = nullptr;
    win32.input = &input;
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

  wheel_event_callback window::set_wheel_event(wheel_event_callback func) noexcept {
    wheel_event_callback temp = input.wheel_event;
    input.wheel_event = func;
    return temp;
  }

  size_event_callback window::set_size_event(size_event_callback func) noexcept {
    size_event_callback temp = input.size_event;
    input.size_event = func;
    return temp;
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