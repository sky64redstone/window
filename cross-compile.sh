#!/usr/bin/env bash
set -euo pipefail

echo "[1/5] Preparing build directory..."
mkdir -p build-windows

echo "[2/5] Entering build directory..."
cd build-windows

echo "[3/5] Copying MinGW binaries..."
cp /usr/x86_64-w64-mingw32/bin/* .

echo "[4/5] Configuring project with CMake..."
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-mingw.cmake

echo "[5/5] Building project..."
cmake --build .
