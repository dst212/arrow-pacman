/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

bool mapIsEmpty(const Map m[MAP_HEIGHT][MAP_WIDTH]) {
	short i, j;
	for(i = 0; i < MAP_HEIGHT; i++) for(j = 0; j < MAP_WIDTH; j++)
		if(isPoint(m, i, j) || isPowerPoint(m, i, j))
			return false;
	return true;
}

void printMapCell(WINDOW*w, const Level*l, const short y, const short x) {
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

void resetMap(Map m[MAP_HEIGHT][MAP_WIDTH]) {
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

void mapColor(Map m[MAP_HEIGHT][MAP_WIDTH], const uint8_t color) {
	short i, j;
	for(i = 0; i < MAP_HEIGHT; i++) for(j = 0; j < MAP_WIDTH; j++)
		m[i][j].color = color;
}

//END
