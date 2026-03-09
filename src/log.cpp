#include "include/log.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

namespace window {
  #define LOG_STDOUT stderr

  static const char* level_names[] = {
    "  ERROR  ",
    " WARNING ",
    " SUCCESS ",
    "  INFO   "
  };
  static const char* layer_names[] = {
    "wayland",
    "x11",
    "win32"
  };

  void log(log_level level, log_layer layer, const char* file,
            int line, const char* func, const char* fmt, ...) {

    va_list ap;
    va_start(ap, fmt);

    /* timestamp */
    time_t t = time(NULL);
    struct tm tm;
    localtime_r(&t, &tm);
    char timebuf[20];
    strftime(timebuf, sizeof timebuf, "%d.%m.%Y %H:%M:%S", &tm);

    /* level string (bounds-checked) */
    const char* lvl = "UNKNOWN";
    if (level >= LOG_LEVEL_ERROR && level <= LOG_LEVEL_INFO) {
      lvl = level_names[level];
    }

    const char* lay = "unknown";
    if (layer >= LOG_WAYLAND && layer <= LOG_WIN) {
      lay = layer_names[layer];
    }

    /* header: timestamp, level, file:line, func */
    fprintf(
      LOG_STDOUT,
      "%s [libwindow|%s] [%s] %s:%d %s(): ",
      timebuf, lay, lvl, file ? file : "?", line, func ? func : "?"
    );

    /* message */
    vfprintf(LOG_STDOUT, fmt, ap);
    fputc('\n', LOG_STDOUT);

    va_end(ap);
  }

}
