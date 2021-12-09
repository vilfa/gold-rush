# Gold Rush
This is a simple sandbox game with random terrain generation, where you play
as a squirrel trying to collect as many golden hazelnuts as possible to stock
up for the winter. You have a 5 minute time limit to collect as many as possible.
After the time limit expires, your score automatically resets and you can play again.

This game is written entirely in C++ using Open GL version 4.2 and Glfw.

## Build & Run
Clone the repo.
```bash
$ git clone https://github.com/vilfa/gold-rush
$ cd gold-rush
```

Required dependencies are OpenGL & Glfw.
```bash
$ mkdir build
$ cmake --build build --config Debug --target all -j<threads> --
```

Then just do
```bash
$ ./build/gold-rush
```

## Screenshots
![final version](https://github.com/luksii/squirrel-gold-rush/blob/master/screenshots/ingame_01.png)
![finished terrain only](https://github.com/luksii/squirrel-gold-rush/blob/master/screenshots/finished_terrain.png)
![terrain plants](https://github.com/luksii/squirrel-gold-rush/blob/master/screenshots/terrain_vegetation_cropped.png)
![player squirrel](https://github.com/luksii/squirrel-gold-rush/blob/master/screenshots/player_squirrel_cropped.png)
![terrain only](https://github.com/luksii/squirrel-gold-rush/blob/master/screenshots/terrain_only.png)
