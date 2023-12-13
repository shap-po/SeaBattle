#  SeaBattle

## Description

SeaBattle is a game for two players. Each player has their field with ships and tries to guess the location of the opponent's ships. The player who destroys all the opponent's ships first wins.
The game supports local multiplayer and playing against the computer with different difficulty levels.
For now, there is only one game mode - one-ship mode. In the future, I plan to add the classic mode as well.

### One-ship mode

In this mode, each player has only one ship. After each turn, the player can move the ship to any adjacent cell, that was not shot before. The player who destroys the opponent's ship first wins.

## Building

### Dependencies

For the project to compile, I recommend using Ninja as a build system. If you know what you are doing, you can use any other build system supported by CMake.

#### Windows

Install [MSYS2](https://www.msys2.org/) and install the following packages:
- mingw-w64-x86_64-gcc
- mingw-w64-x86_64-cmake
- mingw-w64-x86_64-ninja
- mingw-w64-x86_64-gdb (optional for debugging)

By running the following command in the MSYS2 mingw64 terminal (you can open it by running `mingw64.exe` in the MSYS2 installation directory):

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-gdb
```

Don't forget to add the MSYS2 mingw64 `bin` directory to the `PATH` environment variable:
- `C:\msys64\mingw64\bin` by default

#### Linux

Install the following packages:
- gcc
- cmake
- ninja

By running the following command in the terminal:

```bash
sudo apt install gcc cmake ninja-build
```

### Compiling

Then run the following commands in the project directory:

```bash
mkdir build
cd build
cmake -G Ninja ..
ninja
```
