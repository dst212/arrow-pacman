/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in main.c at root directory.
 */

/*
* Ghosts and the pacman have different ways to move around the map: ghosts can
* bypass some walls, while pacman can travel portals.
* Ghosts have different status too (normal, scared ...), while pacman is always
* the same.
* As a result they need different structures (see def.h) and functions for each
* operation.
*/

int8_t ghostSpawn(const level*l, const short y, const short x) {
	int i;
	for(i = 0; i < l->ghosts; i++)
		if(l->ghost[i].spawn.x == x && l->ghost[i].spawn.y == y)
			return i;
	return -1;
}
int8_t ghostPos(const level*l, const short y, const short x) {
	int i;
	for(i = 0; i < l->ghosts; i++)
		if(l->ghost[i].pos.x == x && l->ghost[i].pos.y == y)
			return i;
	return -1;
}
int8_t ghostHit(const level*l, const short y, const short x, const way_t way) {
	int i;
	for(i = 0; i < l->ghosts; i++)
		if(
			turnAround(way) == l->ghost[i].way && (
				(l->ghost[i].pos.x == x - 1 && l->ghost[i].pos.y == y) ||
				(l->ghost[i].pos.x == x + 1 && l->ghost[i].pos.y == y) ||
				(l->ghost[i].pos.x == x && l->ghost[i].pos.y == y - 1) ||
				(l->ghost[i].pos.x == x && l->ghost[i].pos.y == y + 1)
			)
		)	return i;
	return -1;
}

#include "player.h"			//player-related stuff
#include "ghost.h"			//ghosts-related stuff

void printSprites(WINDOW*w, level*l) {
	int8_t i;
	for(i = 0; i < l->ghosts; i++)
		printGhost(w, l, i);
	printPlayer(w, l);
}
void unprintSprites(WINDOW*w, level*l) {
	int8_t i;
	for(i = 0; i < l->ghosts; i++)
		printMapCell(w, l, l->ghost[i].pos.y, l->ghost[i].pos.x);
	printMapCell(w, l, l->player.pos.y, l->player.pos.x);
}

#define isSpawn(l, i, j) (ghostSpawn(l, i, j) != -1 || isPlayerSpawn(l, i, j))

//END
