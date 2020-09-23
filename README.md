# Arrow Pacman

**Arrow Pacman** is a **CLI [ncurses](https://en.wikipedia.org/wiki/Ncurses "Ncurses - Wikipedia") game**, inspired to the original [Pacman](https://en.wikipedia.org/wiki/Pac-Man "Pac-Man - Wikipedia") game.

Since the developer didn't like the `C` letter to be used as Pacman's sprite in a terminal, he decided to represent it with an arrow: `>` (*greater than*). That's how the game got its name.

Beside this, the game is still in development, and full of lovely bugs right behind the corner. Literally every corner of the maze. And outside the game too.

This  project is born just for fun, plus the developer's skills are really bad, so the code may be weird and ugly.

Pull requests and suggestions are welcome, if you'd like to contribute.

## Installation

Before obtaining the game, know that if you don't own a Unix-based system, it won't work, since ncurses works only there. If you are on Windows, consider using the [WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10 "Install Windows Subsystem for Linux (WSL) on Windows 10") and switching to Linux as soon as possible.

Ensure [Meson](https://mesonbuild.com/Quick-guide.html#installation-using-package-manager) is installed on your distro, along with [Ninja](https://ninja-build.org/):

```shell
meson --version
ninja --version
```

If not, install them following the given links.

In order to get the game, you have to clone this repo first:

```shell
git clone https://github.com/dst212/arrow-pacman
cd arrow-pacman/
```

The game can be compiled in two ways:

- the standard way, single player with a level editor;

- for servers, without the level editor to prevent someone from editing the server's levels.

To compile the standard way:

```shell
meson build
ninja -C build
mv build/arrow-pacman ./
```

To compile for servers:

```shell
meson server -Dserver=true
ninja -C server
mv server/arrow-pacman ./
```

Data are stored into `./arrow-pacman-data`, where, you can find a folder containing a demo level named `demo`.

## Usage

To run the game, in both cases:

```shell
./arrow-pacman
```

A menu will show up. Choose `* Play` to start the game, then type `demo`. If you want to create a level, choose `* Editor`. If you wish to quit, choose `* Quit`.

`* About` will show software's info and the license disclaimer.

## How to play

First of all, glossary:

- a **tick** is the delay between a "frame" and the following one;

- the **Pacman** (or **player**) is you in the game;

- the **ghosts** are those other entities which move around the map;

- the **map** is the whole area made of **cells** in which you can move;

- a **blank space** is a crossable cell;

- a **wall** is an uncrossable cell;

- a **bypassable wall** is a kind of wall crossable only by ghosts;

- a **portal** is a cell which allows the Pacman to be moved to another cell in the map;

- a **point** (shown as `·` in the demo level, increases the score by 5), is the Pacman's food;

- a **powered point** (shown as `∗` in the demo level, increases the score by 10), is a point which turns the ghosts into snacks for the Pacman for a small amount of ticks;

- the **hidden treasure** is nothing but a thing I just invented.

The goal is to eat all the ***points*** you will find around the map, without getting caught by a ghost.

If the Pacman reaches a ***powered point***, it will be able to eat the ghosts for a while, let's say *x ticks*. If a ghost gets eaten, it will quickly reach it's home (it's spawn point) so as it can be restored. Eating a ghost will increase the score by 20.

When all the points are eaten, **the Pacman levels up**, the tick value is decreased a bit and so does the amount of ticks given by the powered point to make all the ghosts edible.

The game is over when you lose all your lives.

Commands:

- `↑ ← ↓ →`: move the pacman around;

- `p`: pause the game.

## Level Editor

Arrow Pacman provides a **level editor**, so as everyone can create custom levels.

A level structure is made of:

- the **map** (79 cells in width and 24 in height);

- the **player**;

- a maximum of 8 **ghosts**;

- the map properties.

Note that if some changes are applied to the level-file structure in the source code, older level-files may not be compatible anymore, and it will be necessary to convert them (instructions will come along with the updates).

Here is a list of possible commands:

- `↑ ← ↓ →`: move the cursor around and select walls;

- `i j k l`: move up, left, down or right the cursor copying the content of the current cell;

- `a`: change the current cell's **appearance**;

- `w`: set the current cell as a **wall**, and pick a new texture;

- `b`: make the wall **bypassable**;

- `p`: place a **portal**;

- `r`: turn any cell into a **space** without changing the appearance; used on a blank space, it will **reset** its appearance;

- `c`: change the current's cell **color**;

- `v`: change **the whole map's color** (of every cell);

- `.`: put a **normal point**;

- `*`: put a **powered point**;

- `s`: place and edit the **Pacman's spawn-point**;

- `g`: place/edit a **ghost**;

- `u`: **re-print** the whole map, if some visual bugs pop out;

- `n`: edit **properties**, such as the power-ticks (the duration of a powered point's effect);

- `m`: open the **menu** to save the file, open a new one or quit.

When you change a wall's **appearance**, you can change cell using `↑ ↓` and change the group of cells using `→ ←` .

The Pacman and the ghosts' menus also have an option to edit the **sprites' appearance**. Use `↑ ← ↓ →` to move, `ENTER` to change the appearance, and `c` to change color.

---

## TODO list:

- Add scores and an "admin console" for server's score editing.

- Multiply the 20 points of score when more than a ghost is eaten while the powered point's effect is active.

## Known issues:

- Sometimes, the ghosts and the player collide wrongly, but collision is detected anyway.

- On color change in the sprite editor, the background turns black.

- Ghost routes algorithm may not work with different maps.

## Notes

The program is licensed under GPLv3. You are free to modify/redistribute it under the terms of the GNU General Public License. If you wish to run your own server-version of Arrow Pacman and provide it as a service, you do **NOT** have to publish the source code, **BUT** if you redistribute it you have to.

You can share the level-files made with the editor where and as you like, they are not licensed.

The program makes use of [cstuff libraries](https://github.com/dst212/cstuff), a shitty and simple library used for some basic functions mostly about ncurses and strings, licensed under the LGPLv3.
