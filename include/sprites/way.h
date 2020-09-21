/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//Way's functions and macros

#define turnAround(way)	((way & 0b1100) ? (way) >> 2 : (way) << 2)
#define turnLeft(way)	((way & 0b0001) ? (way) << 3 : (way) >> 1)
#define turnRight(way)	((way & 0b1000) ? (way) >> 3 : (way) << 1)

extern void shiftSprite(Coord*, Way);
extern Way randomMove(const Way, Way);

//END
