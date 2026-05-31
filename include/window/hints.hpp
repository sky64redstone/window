#ifndef HINTS_HPP
  #define HINTS_HPP

  #include <string>

  namespace window {
    struct hint_data {
      std::string appname;
      int glvmajor, glvminor;
    };
  }

#endif
