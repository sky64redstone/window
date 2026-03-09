#ifndef LOG_HPP
  #define LOG_HPP

  namespace window {
    enum log_level {
      LOG_LEVEL_ERROR,
      LOG_LEVEL_WARNING,
      LOG_LEVEL_SUCCESS,
      LOG_LEVEL_INFO
    };

    // I have no idea how I should name this enum...
    enum log_layer {
      LOG_WAYLAND,
      LOG_X11,
      LOG_WIN
    };

    void log(log_level level, log_layer layer, const char* file, int line, const char* func, const char* fmt, ...);

    #define log_error(layer, ...)   log(::window::log_level::LOG_LEVEL_ERROR,   layer, __FILE__, __LINE__, __func__, __VA_ARGS__)
    #define log_warning(layer, ...) log(::window::log_level::LOG_LEVEL_WARNING, layer, __FILE__, __LINE__, __func__, __VA_ARGS__)
    #define log_success(layer, ...) log(::window::log_level::LOG_LEVEL_SUCCESS, layer, __FILE__, __LINE__, __func__, __VA_ARGS__)
    #define log_info(layer, ...)    log(::window::log_level::LOG_LEVEL_INFO,    layer, __FILE__, __LINE__, __func__, __VA_ARGS__)
    #define log_loading(layer, ...) log(::window::log_level::LOG_LEVEL_LOADING, layer, __FILE__, __LINE__, __func__, __VA_ARGS__)

  }

#endif
