/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//map's functions and boolean macros

//c -> coords, m -> map cell, f -> field (the whole map)
#define MAP_NONWALL		-2 //non-walls
#define MAP_SPACE		0
#define MAP_POINT		1
#define MAP_EATENPOINT	(-MAP_POINT)
#define MAP_PPOINT		2
#define MAP_EATENPPOINT	(-MAP_PPOINT)

#define MAP_WALL		-1 //walls
#define MAP_BYPASSABLE	0
#define MAP_SOLID		1

#define PACMAN_POINT	L"·"
#define PACMAN_PPOINT	L"∗"
#define PACMAN_HEART	L"❤"

#define validCoord(y, x)		(0 <= (x) && 0 <= (y) && (x) < MAP_WIDTH && (y) < MAP_HEIGHT)
#define outOfMap(c)				(!validCoord((c).y, (c).x)) //redundant, will be removed somehow
//portal's coordinates are used as identifiers if it's not a valid portal
#define isPoint(f, i, j)			(((f)[i][j]).portal.x == MAP_NONWALL && ((f)[i][j]).portal.y == MAP_POINT) //pacman's food
#define isPowerPoint(f, i, j)		(((f)[i][j]).portal.x == MAP_NONWALL && ((f)[i][j]).portal.y == MAP_PPOINT) //has nothing to do with Microsoft, it's the point which make ghosts "eatable"
#define isEatenPoint(f, i, j)		(((f)[i][j]).portal.x == MAP_NONWALL && ((f)[i][j]).portal.y == MAP_EATENPOINT)
#define isEatenPowerPoint(f, i, j)	(((f)[i][j]).portal.x == MAP_NONWALL && ((f)[i][j]).portal.y == MAP_EATENPPOINT)
#define isSpace(f, i, j)			(((f)[i][j]).portal.x == MAP_NONWALL && ((f)[i][j]).portal.y == MAP_SPACE) //a cell which can be travelled both by ghosts and by the player
#define isCrossable(f, i, j)		(((f)[i][j]).portal.x == MAP_NONWALL && -2 <= ((f)[i][j]).portal.y && ((f)[i][j]).portal.y <= 2) //both ghosts and the player can travel it
#define isWall(f, i, j)				(((f)[i][j]).portal.x == MAP_WALL && ((f)[i][j]).portal.y == MAP_SOLID) //is a solid wall
#define isBypassable(f, i, j)		(((f)[i][j]).portal.x == MAP_WALL && ((f)[i][j]).portal.y == MAP_BYPASSABLE) //only ghosts can travel it
#define isGenericWall(f, i, j)		(((f)[i][j]).portal.x == MAP_WALL && 0 <= ((f)[i][j]).portal.y && ((f)[i][j]).portal.y <= 1)
#define isPortal(f, i, j)			(	(validCoord((f)[i][j].portal.y, (f)[i][j].portal.x)) && \
										isCrossable((f), (f)[i][j].portal.y, (f)[i][j].portal.x))

extern bool mapIsEmpty(const Map[MAP_HEIGHT][MAP_WIDTH]);
extern void printMapCell(WINDOW*, const Level*, const short, const short);

extern void resetMap(Map[MAP_HEIGHT][MAP_WIDTH]);
extern void mapColor(Map[MAP_HEIGHT][MAP_WIDTH], const uint8_t);

//END
