/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//ghosts' functions

#define resetGhost(s)			(s).pos = (s).spawn, (s).way = WEST, (s).flag = GH_NORMAL
#define ghostCanMove(f, y, x)	(validCoord(y, x) && !isWall(f, y, x))

extern void printGhost(WINDOW*, const Level*, const int8_t);
extern void reprintGhost(WINDOW*, const Level*, const int8_t, const short);

extern Way ghostMoves(Coord, const Map[MAP_HEIGHT][MAP_WIDTH]);
extern void moveGhost(Level*, const int8_t, uint16_t*);

#include "routes.h"

//END
