# Pong

![Platform][platform-shield]
[![Apache 2.0 License][license-shield]][license-url]

A clone of Atari's arcade game _Pong_.

## Prerequisites

- [SDL2][sdl2-download]
- [SDL2\_image][sdl2-image-download]

### Linux

Debian/Ubuntu-based distributions:

```console
# apt install libsdl2-dev libsdl2-image-dev
```

Arch Linux:

```console
# pacman -S sdl2 sdl2_image
```

### Windows

Download and extract the development libraries for SDL2 and SDL2\_image:
- If you use Visual Studio, choose the Visual C++ libraries. 
- If you use Mingw-w64, choose the MinGW libraries.
- Keep track of where you extract the libraries so that CMake can find them.

## Building

1. Clone the repository and create a build directory.
    ```sh
    git clone --recurse-submodules https://github.com/PotatoTech/pong.git
    cd pong
    mkdir build
    cd build
    ```

### Linux

2. Build the project.
    ```sh
    cmake ..
    make
    ```

### Windows

#### Visual Studio

2. Generate the build files.
    ```sh
    cmake .. -DSDL2_PATH="path_to_sdl2" -DSDL2_IMAGE_PATH="path_to_sdl2_image" -G "Visual Studio 16 2019"
    ```
    Replace `Visual Studio 16 2019` with your version. Click
    [here][visual-studio-generators] for the list of Visual Studio generators
    recognized by CMake.
3. Open the `pong.sln` file in Visual Studio.
4. Build the solution.
5. Copy the SDL2 and SDL2\_image runtime libraries into the `bin` directory.
    - **Note**: The runtime `.dll` files are included in the `lib\x64`
    subdirectory of the development libraries, so it is not necessary to
    download the runtime libraries separately.

#### Mingw-w64

2. Build the project.
    ```sh
    cmake .. -DSDL2_PATH="path_to_sdl2\x86_64-w64-mingw32" -DSDL2_IMAGE_PATH="path_to_sdl2_image\x86_64-w64-mingw32" -G "MinGW Makefiles"
    mingw32-make
    ```
3. Copy the SDL2 and SDL2\_image runtime libraries into the `bin` directory.
    - **Note**: The runtime `.dll` files are included in the
    `x86_64-w64-mingw32\bin` subdirectory of the development libraries, so
    it is not necessary to download the runtime libraries separately.

## Controls

Control the left paddle with the E and D keys. Control the right paddle with the
up and down arrow keys. Pause and unpause the game by pressing Enter.

## License

Copyright &#x00A9; 2020 PotatoTech

Pong is licensed under the Apache 2.0 license.
See [LICENSE][license-url] for more information.



<!-- Markdown links and images -->
[platform-shield]: https://img.shields.io/badge/platform-linux--64%20%7C%20win--64-lightgrey?style=flat-square
[license-shield]: https://img.shields.io/badge/license-Apache%202.0-blue?style=flat-square
[license-url]: LICENSE

[sdl2-download]: https://www.libsdl.org/download-2.0.php
[sdl2-image-download]: https://www.libsdl.org/projects/SDL_image/
[visual-studio-generators]: https://cmake.org/cmake/help/v3.14/manual/cmake-generators.7.html#visual-studio-generators
