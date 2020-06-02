# Minesweeper

## Install

Install C++ libraries: `sdl2` and `sdl2_image`

```
# For MacOS
brew install sdl2 sdl2_image
```

If the header files `SDL2/SDL.h` and `SDL2/SDL_image.h` are not in the standard directory, add to the variable `INCLUDES` in `Makefile` the correct path:

```
INCLUDES := -I./src -I/directory/that/contains/header/files
```

Do the same for the lib files (`libSDL2.a` and `libSDL2_image.a`), but update the variable `LDFLAGS`:

```
LDFLAGS := -L/directory/that/contains/lib/files
```

Run `make` inside the project root directory

```
make
```

Execute the newly-created binary file to play

```
./minesweeper
```

Enjoy!
