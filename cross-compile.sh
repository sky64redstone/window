#!/usr/bin/env bash
set -euo pipefail

if [[ "${1:-}" = "clean" ]]; then
  echo "Cleaning up..."
  rm -rf build-windows
  exit 0
fi

echo "[1/4] Preparing build directory..."
mkdir -p build-windows

echo "[2/4] Entering build directory..."
cd build-windows

echo "[3/4] Configuring project with CMake..."
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-mingw.cmake

echo "[4/4] Building project..."
cmake --build .

if [[ "${1:-}" = "run" ]]; then
  echo "Running example code..."
  export WINEPATH="/usr/x86_64-w64-mingw32/bin/"
  wine window_test.exe
fi
