add_executable(window_test "main.cpp")

target_link_libraries(window_test PRIVATE window)

if (UNIX)
  target_link_libraries(window_test PRIVATE GL)
else()
  target_link_libraries(window_test PRIVATE user32 kernel32 gdi32 dwmapi opengl32)
endif()


set_target_properties(window_test PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
