#ifndef WINDOW_HPP
  #define WINDOW_HPP

  #include "platform.hpp"
  #include "input.hpp"

  namespace window {
    enum backend {
      WIN32,
      X11,
      WAYLAND
    };

    class window_api window {
    public:
      #if defined(window_x11)
        window_x11_data x11;
      #endif

      #if defined(window_wl)
        window_wl_data wl;
      #endif

      #if defined(window_win32)
        window_win32_data win32;
      #endif

      input_data input;

    public:
      window() noexcept;
      ~window() noexcept;

      /*
       * Create a new window.
       *
       * width:  The inital width in pixels of the window
       * height: The inital height in pixels of the window
       * title:  ASCII encoded name of the window
       *
       * returns: result::SUCCESS on success, otherwise failure
       *
       * NOTE: 'width' and 'height' might be ignored on some linux system.
       *       Instead the window manager sends a size event to notifiy about
       *       the new dimensions of the window
       * SEE:  window::result in include/platform.hpp for detailed failure codes
       *       window::window.set_size_event(...) for a size event callback
       */
      result create(int width, int height, const char* title) noexcept;

      /*
       * Sets the window title.
       *
       * title: ASCII encoded name of the window
       *
       * returns: result::SUCCESS on success, otherwise failure
       */
      result set_title(const char* title) const noexcept;

      /*
       * Sets the size of the window in pixels.
       *
       * width:  The inital width in pixels of the window
       * height: The inital height in pixels of the window
       *
       * returns: result::SUCCESS on success, otherwise failure
       *
       * NOTE: 'width' and 'height' might be ignored on some linux system.
       *       Instead the window manager sends a size event to notifiy about
       *       the new dimensions of the window
       * SEE:  window::window.set_size_event(...) for a size event callback
       */
      result set_size(int width, int height) noexcept;

      /*
       * Checks if the window is still open.
       *
       * returns: false when the window should close
       */
      bool is_open() const noexcept;

      /*
       * Resolves incomming window events and dispatches those 
       * to the registered event handlers.
       *
       * returns: result::SUCCESS on success, otherwise failure
       *
       * NOTE: this function is non-blocking
       */
      result poll_events() noexcept;

      /*
       * Creates an opengl context for the current window.
       * 
       * returns: result::SUCCESS on success, otherwise failure
       *
       * NOTE: this function should be called before any opengl function
       */
      result make_opengl_context() noexcept;

      /*
       * Swaps the front and back buffer for the current window with opengl.
       *
       * returns: result::SUCCESS on success, otherwise failure
       *
       * NOTE: make_opengl_context should be called before this one
       */
      result swap_buffers() const noexcept;

      /*
       * Sets the opengl swap interval for this window.
       *
       * interval: number of screen updates to wait before swapping the buffers of a window
       *
       * returns: result::SUCCESS on success, otherwise failure
       *
       * NOTE: make_opengl_context should be called before this one
       *       setting 'interval' to 0 means maximum fps
       *       setting 'interval' to 1 means vsync
       *       setting 'interval' to 2 means half the monitors frame rate
       *       ...
       */
      result swap_interval(int interval) const noexcept;

      /*
       * Sets an event callback function for a key event.
       * This function is called when a key is pressed, released or repeated.
       *
       * returns: the previous registered callback
       */
      key_event_callback    set_key_event(key_event_callback        func) noexcept;

      /*
       * Sets an event callback function for a mouse button event.
       * This function is called when a mouse button is pressed or released.
       *
       * returns: the previous registered callback
       */
      button_event_callback set_btn_event(button_event_callback     func) noexcept;

      /*
       * Sets an event callback function for a mouse button event.
       * This function is called when a mouse button is pressed two times in a short time.
       *
       * returns: the previous registered callback
       */
      dblclk_event_callback set_dblclk_event(dblclk_event_callback  func) noexcept;

      /*
       * Sets an event callback function for a mouse move event.
       * This function is called when the mouse was moved.
       *
       * returns: the previous registered callback
       */
      mouse_event_callback  set_mouse_event(mouse_event_callback    func) noexcept;

      /*
       * Sets an event callback function for a vertical scroll event.
       * This function is called when the user wants to scroll up and down.
       *
       * returns: the previous registered callback
       */
      scroll_event_callback set_vscroll_event(scroll_event_callback func) noexcept;

      /*
       * Sets an event callback function for a horizontal scroll event.
       * This function is called when the user wants to scroll left and right.
       *
       * returns: the previous registered callback
       */
      scroll_event_callback set_hscroll_event(scroll_event_callback func) noexcept;

      /*
       * Sets an event callback function for a window resize event.
       * This function is called when the window dimensions get set from outside.
       *
       * returns: the previous registered callback
       *
       * NOTE: Calling the set_size function won't create a size event
       */
      size_event_callback   set_size_event(size_event_callback      func) noexcept;

      /*
       * Checks which window backend is being used. (usefull on linux)
       *
       * returns: window backend
       */
      backend get_backend() const noexcept;

      /*
       * Closes the window and destroyes all allocated resources.
       */
      void destroy() noexcept;
    };
  }

#endif
