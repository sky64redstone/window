cmake_minimum_required (VERSION 3.10)

project(window)

macro(dependent_option OPTION_NAME OPTION_DESC DEFAULT_VALUE DEP_CONDITION)
    if(${DEP_CONDITION})
        option(${OPTION_NAME} "${OPTION_DESC}" ${DEFAULT_VALUE})
    else()
        set(${OPTION_NAME} OFF CACHE BOOL "${OPTION_DESC} (disabled due to missing dependency)" FORCE)
    endif()
endmacro()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# CMake Options
dependent_option(window_win32 "Enable win32 support" ON "WIN32" OFF)
dependent_option(window_x11   "Enable x11 support"   ON "UNIX"  OFF)
dependent_option(window_wl    "Enable wl support"    ON "UNIX"  OFF)

if (window_win32)
  message(STATUS "Win32 support enabled")
endif()
if (window_x11)
  message(STATUS "X11 support enabled")
endif()
if (window_wl)
  message(STATUS "Wayland support enabled")
endif()

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(test)