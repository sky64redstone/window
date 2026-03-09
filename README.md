# libwindow

`libwindow` is a lightweight library for creating and managing **cross-platform windows** in C++. It provides a simple interface for opening, controlling, and interacting with windows while keeping dependencies minimal.

The project is designed as a small utility library and can serve as a starting point for building window-based tools, graphical experiments, or platform abstraction layers.

---

## Usage

A minimal usage example can be found in:

```
test/main.cpp
```

After building the project, the example program will be compiled automatically.

Run it with:

```bash
cd build
./window_test
```

For API details, refer to the main header:

```
include/window.hpp
```

---

## Supported Platforms

Currently supported platforms:

* Linux (both X11 and Wayland)
* Windows

---

## Requirements

To build **libwindow**, you need a C++ compiler, CMake, and a build tool.
On Linux you will also need development libraries depending on whether you use **X11** or **Wayland**.

* A C/C++ compiler (e.g. GCC or Clang)
* CMake
* Git

> [!WARNING]
> The external links and package names below are provided for convenience only, and I do not guarantee their validity, safety, or that they are free of malware, so you should verify downloads and sources yourself before installing any software.

### Windows

Recommended setup:

1. Install **Visual Studio** with the **Desktop development with C++** workload
   [https://visualstudio.microsoft.com/](https://visualstudio.microsoft.com/)

2. Install **CMake**
   [https://cmake.org/download/](https://cmake.org/download/)

3. Install **Git**
   [https://git-scm.com/downloads](https://git-scm.com/downloads)

After installation you can build the project from a **Developer Command Prompt** or **PowerShell**.

### Linux (Ubuntu / Debian)

Install the basic build tools:

```bash
sudo apt install build-essential cmake git
```

#### X11 Support

```bash
sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

#### Wayland Support

```bash
sudo apt install libwayland-dev wayland-protocols libxkbcommon-dev
```

You only need the packages for the window system you intend to use.

---

### Linux (Arch / Manjaro)

Install the basic build tools:

```bash
sudo pacman -S base-devel cmake git
```

#### X11 Support

```bash
sudo pacman -S libx11 libxrandr libxinerama libxcursor libxi
```

#### Wayland Support

```bash
sudo pacman -S wayland wayland-protocols libxkbcommon
```

---

## Installation

Make sure you have installed make, cmake and a compiler that is supported by cmake

Clone the repository:

```bash
git clone https://github.com/sky64redstone/window.git
cd window
```

Build the library with the following commands

```bash
mkdir build
cd build
cmake ..
make
```

---

## Project Structure

Example structure:

```
window/
├── src/          # Source code
├── include/      # Header files
├── test/         # Example code
├── build/        # Compiled files
├── .gitignore
├── CMakeLists.txt
└── README.md
```
