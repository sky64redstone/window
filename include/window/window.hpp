#ifndef WINDOW_HPP
  #define WINDOW_HPP

  #include "input.hpp"
  #include "platform.hpp"

  #if defined(window_x11)
    #include "x11.hpp"
  #endif

  #if defined(window_wl)
    #include "wayland.hpp"
  #endif

  namespace window {
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
      hint_data  hintmem;
      framebuffer_data fb;

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
       * Creates a graphics context for the current window.
       * The type of the context is determined by the current set
       * graphics backend.
       * 
       * returns: result::SUCCESS on success, otherwise failure
       *
       * NOTE: If the gfx backend is set to GRAPHICS_FRAMEBUFFER, then
       *       this creates the framebuffer with the current window size.
       *       This function should be called before any graphics function
       *
       * SEE: set_gfx_backend(graphics_backend)
       *      get_gfx_backend()
       */
      result make_gfx_context() noexcept;

      /*
       * Resizes the current framebuffer to a specified size.
       * The size is messured in pixels.
       * If width or height is set to zero, the current window width or
       * height is used, respectively.
       *
       * returns: result::SUCCESS on success, otherwise failure
       *
       * NOTE: this function overrides any data on the current framebuffer
       */
      result resize_framebuffer(int width, int height) noexcept;

      /*
       * With OpenGL:
       * Swaps the front and back buffer for the current window with opengl.
       *
       * With Framebuffer:
       * Renders the current framebuffer on the window.
       *
       * returns: result::SUCCESS on success, otherwise failure
       *
       * NOTE: make_opengl_context or make_framebuffer_context
       *       should be called before this one
       */
      result swap_buffers() noexcept;

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
       * Enables the program to set a custom user data, which will be received
       * with every event callback to identify the window.
       *
       * data: custom user data to set
       *
       * returns: previous contained data
       */
      void* set_user_data(void* data) noexcept;

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
       * The appname is used for identifying all windows that belong 
       * to an certain app. Using the default name, may impact user experience.
       * Needs to be set before creating the window.
       *
       * NOTE: window hint (data may be ignored by the operating system)
       */
      void set_appname(const char* appname) noexcept;

      /*
       * Specifies a desired opengl version.
       * Needs to be set before creating the window.
       *
       * NOTE: window hint (data may be ignored by the operating system)
       */
      void set_glversion(int major, int minor) noexcept;

      /*
       * Checks which window backend is being used. (usefull on linux)
       *
       * returns: window backend
       */
      backend get_backend() const noexcept;

      /*
       * Returns the currently used graphics backend
       *
       * returns: graphics backend
       */
      graphics_backend get_gfx_backend() const noexcept;

      /*
       * Sets the graphics backend. The default is GRAPHICS_OPENGL.
       *
       * returns: result::SUCCESS on success, otherwise failure
       *
       * NOTE: This function will fail if a current graphics context exists.
       */
      result set_gfx_backend(graphics_backend backend) noexcept;

      /*
       * Returns the pointer to the framebuffer. Pixel encoding is ARGB.
       * May be nullptr if make_gfx_context was never called.
       * This function should be used to render pixels on the screen.
       * You can compute the offset of a pixel with this:
       * y_offset = (y * framebuffer_stride()) / sizeof(uint32_t)
       * offset = y_offset + x;
       *
       * returns: pointer to framebuffer
       *
       * SEE: framebuffer_width, framebuffer_height, framebuffer_stride
       */
      std::uint32_t* framebuffer_pixels() noexcept;

      /*
       * Returns the pointer to the framebuffer. Pixel encoding is ARGB.
       * May be nullptr if make_gfx_context was never called.
       * This function should be used to read pixels in the current framebuffer.
       * You can compute the offset of a pixel with this:
       * y_offset = (y * framebuffer_stride()) / sizeof(uint32_t)
       * offset = y_offset + x;
       *
       * returns: pointer to framebuffer
       *
       * SEE: framebuffer_width, framebuffer_height, framebuffer_stride
       */
      const std::uint32_t* framebuffer_pixels() const noexcept;

      /*
       * Returns the width of the framebuffer.
       *
       * returns: framebuffer width in pixels (uint32_t)
       *
       * NOTE: Should not be used to compute a y offset in the buffer.
       *       Instead use the framebuffer_stride function.
       *
       * SEE: framebuffer_height, framebuffer_stride
       */
      int framebuffer_width() const noexcept;

      /*
       * Returns the height of the framebuffer.
       *
       * returns: framebuffer height in pixels
       *
       * SEE: framebuffer_width, framebuffer_stride
       */
      int framebuffer_height() const noexcept;

      /*
       * Returns the stride of the framebuffer. The stride is the number of
       * bytes between the start of one row and the start of the next.
       * To compute the offset of a row use this formular:
       * y_offset = (y * framebuffer_stride()) / sizeof(uint32_t)
       * By default this is set to framebuffer_width() * sizeof(uint32_t).
       *
       * returns: framebuffer width in pixels (uint32_t)
       *
       * SEE: framebuffer_width, framebuffer_height
       */
      int framebuffer_stride() const noexcept;

      /*
       * Closes the window and destroyes all allocated resources.
       *
       * NOTE: Only destroyes operating system data.
       *       It keeps input and hint configuration.
       */
      void destroy() noexcept;
    };
  }

#endif
