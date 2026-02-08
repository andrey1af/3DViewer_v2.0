<p align="center">
  <img src="src/gui/resources/icons/logo.png" width="110" height="110" alt="3DViewer logo"/>
</p>

# 3DViewer v2.0

Desktop application for visualizing 3D models in .obj format.

## About

`3DViewer v2.0` renders `.obj` models, applies affine transformations, and saves user settings between runs.
The project is focused on clean architecture, modularity, and testability.

## Features

- Load `.obj` files (vertices and faces), including support for negative indices.
- Render the model in `QOpenGLWidget`.
- Affine transformations: translation, rotation around `X/Y/Z`, scaling.
- Rendering settings: `parallel/central` projection, edge style/color/thickness, vertex style/color/size, background color.
- Save parameters to `settings.ini`.
- Export a frame to `JPEG/BMP`.
- Record a GIF screencast (`640x480`, `10 fps`, `5 sec`).
- Control with mouse wheel, drag, and hotkeys.

## Technologies

- `C++20`
- `Qt5 Widgets`
- `OpenGL` + GLSL
- `Makefile`
- `Google Test`
- `lcov/genhtml`
- `clang-format`, `cppcheck`, `valgrind`

## Architecture

- `MVC`: separation of UI and business logic.
- `Facade`: a single entry point for model operations.
- `Observer`: notifications about scene and settings changes.
- `Singleton`: centralized access to settings and the facade.

## Project Structure (full version)

```text
src/
  backend/        # model loading, scene, transformation matrices, facade
  gui/            # window, menu, OpenGL view, settings, styles, shaders
  headers/        # shared headers
  tests/          # unit tests for .obj parsing and affine transformations
  Makefile
```

## Quick Start (Linux)

### 1) Install dependencies

```bash
sudo apt update
sudo apt install -y \
  g++ make pkgconf qt5-qmake qtbase5-dev qtbase5-dev-tools \
  libgl1-mesa-dev libglu1-mesa-dev \
  lcov valgrind cppcheck clang-format
```

### 2) Build and install the application

```bash
cd src
make
make install
```

### 3) Run

```bash
~/3DViewer/3DViewer
```

## Useful `make` targets

| Command | Description |
|---|---|
| `make` | Builds and installs the application |
| `make tests` | Runs unit tests |
| `make gcov_report` | Generates a coverage report (`lcov` + `genhtml`) |
| `make clang` | Checks formatting |
| `make cpp` | Runs `cppcheck` |
| `make memory` | Checks for memory leaks |
| `make uninstall` | Removes the installed binary |
| `make clean` | Cleans build artifacts |

## Author

- [Lafutkin Andrey](https://github.com/AndreyLafutkin)
