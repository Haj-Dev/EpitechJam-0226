# Super Jam Boy
The best game you'll never want to play!

## Build Instructions

Follow [Devkitpro's installation guide](https://devkitpro.org/wiki/Getting_Started) and [Dualie's one](https://github.com/Dogbonee/Dualie/tree/main).
Then:
- Create a `build` folder in the project root.
- `cd` into `build`
- Run `cmake .. -DCMAKE_TOOLCHAIN_FILE='/opt/devkitpro/cmake/3DS.cmake' -DDualie_DIR=/opt/devkitpro/cmake/` (arguments may differ depending on your installation)
- Run `make`. A `.3dsx` file will be generated.
