/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in main.c at root directory.
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
//only used on map load
#define isValidWall(m)		((m).wall < ' ' || 255 <= (m).wall)

void printMapCell(WINDOW*w, const level*l, const short y, const short x) {
	bool blink = isPowerPoint(l->map, y, x);
	setcolor(w, l->map[y][x].color);
	if(blink) wattron(w, A_BLINK);
	mvwaddnwstr(
		w, y, x,
		(	isEatenPoint(l->map, y, x) ||
			isEatenPowerPoint(l->map, y, x)
		) ? L" " : &(l->map[y][x].wall),
		1
	);
	if(blink) wattroff(w, A_BLINK);
}

void resetMap(map_t m[MAP_HEIGHT][MAP_WIDTH]) {
	short i, j;
	for(i = 0; i < MAP_HEIGHT; i++) for(j = 0; j < MAP_WIDTH; j++) {
		if(m[i][j].portal.x == MAP_NONWALL) {
			if(m[i][j].portal.y == MAP_EATENPOINT)
			 	m[i][j].portal.y = MAP_POINT;
			else if(m[i][j].portal.y == MAP_EATENPPOINT)
				m[i][j].portal.y = MAP_PPOINT;
		}
	}
}

bool mapIsEmpty(map_t m[MAP_HEIGHT][MAP_WIDTH]) {
	short i, j;
	for(i = 0; i < MAP_HEIGHT; i++) for(j = 0; j < MAP_WIDTH; j++)
		if(isPoint(m, i, j) || isPowerPoint(m, i, j))
			return false;
	return true;
}

void mapColor(map_t m[MAP_HEIGHT][MAP_WIDTH], uint8_t color) {
	short i, j;
	for(i = 0; i < MAP_HEIGHT; i++) for(j = 0; j < MAP_WIDTH; j++)
		m[i][j].color = color;
}


//END
