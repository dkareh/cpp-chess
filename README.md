# COP 3003 - Programming II - C++ Chess

[![OpenSSF Best Practices](https://www.bestpractices.dev/projects/8519/badge)](https://www.bestpractices.dev/projects/8519)
[![OpenSSF Scorecard](https://api.securityscorecards.dev/projects/github.com/dkareh/cpp-chess/badge)](https://securityscorecards.dev/viewer/?uri=github.com/dkareh/cpp-chess)

## Functionality

Users can play chess, and all of the rules are enforced (except draw conditions like dead position, threefold repetition, etc.).
Users can also choose between several different visual styles.

## Getting Started

Download the project by, for instance, cloning the repository with Git:

```shell
git clone https://github.com/dkareh/cpp-chess.git
cd cpp-chess
```

Here are examples of how to configure, build, install, and run the game with:

1. [CMake](https://cmake.org/) and [Ninja](https://ninja-build.org/)
2. CMake (no presets)
3. [Zig](https://ziglang.org/)

Let's assume we want a debuggable executable to be installed under the project directory.

### CMake and Ninja

```shell
cmake --preset ninja-multi
cmake --build --preset ninja-debug --target install
./out/bin/chess-d
```

### CMake (no presets)

```shell
mkdir build
cd build
# For single-configuration generators (using `CMAKE_BUILD_TYPE`):
cmake --install-prefix="$PWD/out" -D CMAKE_BUILD_TYPE=Debug ..
# For Ninja multi-configuration generator (ignoring `CMAKE_BUILD_TYPE`):
cmake --install-prefix="$PWD/out" -D CMAKE_DEFAULT_BUILD_TYPE=Debug ..
# For IDE multi-configuration generators (ignoring `CMAKE_BUILD_TYPE`):
cmake --install-prefix="$PWD/out" ..
cmake --build . --target install
./out/bin/chess-d
```

Feel free to change the steps. You could:

1. Use a specific CMake generator such as "MinGW Makefiles"
2. Run Visual Studio once everything has been configured
3. Use `%cd%\out` in Windows Command Prompt instead of `$PWD/out`
4. Directly invoke `make install`, `ninja install`, etc.

### Zig

The build system was last tested with Zig `0.12.0-dev.2818+97290e0bf`.

```shell
zig build run -Doptimize=Debug
```

## Why?

I'm not an avid chess player, but I watch my friends play chess sometimes, and it looks fun!
Additionally, the rules of chess are complex enough to be interesting to me.

## Maintainers

- Daniel Kareh ([@dkareh](https://github.com/dkareh))

## To Do

- [X] Tell players if checkmate or stalemate has occurred.
- [X] Add support for castling.
- [ ] Check common draw conditions like threefold repetition.
