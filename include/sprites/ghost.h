/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in main.c at root directory.
 */

//ghosts' functions

#define resetGhost(s)			(s).pos = (s).spawn, (s).way = WEST, (s).flag = GH_NORMAL
#define ghostCanMove(f, y, x)	(validCoord(y, x) && !isWall(f, y, x))
void printGhost(WINDOW*w, const level*l, const int8_t i) {
	setcolor(w, bgcolor((l)->map[l->ghost[i].pos.y][l->ghost[i].pos.x].color) + fgcolor(l->ghost[i].color[l->ghost[i].flag]));
	mvwaddnwstr(w, l->ghost[i].pos.y, l->ghost[i].pos.x, &(l->ghost[i].s[l->ghost[i].flag][INDEX_WAY(l->ghost[i].way)]), 1);
}
void reprintGhost(WINDOW*w, const level*l, const int8_t i, const short c) {
	setcolor(w, bgcolor((l)->map[l->ghost[i].pos.y][l->ghost[i].pos.x].color) + fgcolor(c));
	mvwaddnwstr(w, l->ghost[i].pos.y, l->ghost[i].pos.x, &(l->ghost[i].s[l->ghost[i].flag][INDEX_WAY(l->ghost[i].way)]), 1);
}

way_t ghostMoves(coord pos, map_t m[MAP_HEIGHT][MAP_WIDTH]) {
	way_t r = 0;
	//the ghost can choose these ways
	if(ghostCanMove(m, pos.y, pos.x - 1)) r |= WEST;
	if(ghostCanMove(m, pos.y, pos.x + 1)) r |= EAST;
	if(ghostCanMove(m, pos.y - 1, pos.x)) r |= NORTH;
	if(ghostCanMove(m, pos.y + 1, pos.x)) r |= SOUTH;
	return r;
}

#include "routes.h"

void moveGhost(/*WINDOW*w, */level*l, int8_t i, uint16_t*bitmap) {
	way_t r = 0, moves = (l->ghost[i].ticks < l->ticks) ? ghostMoves(l->ghost[i].pos, l->map) : playerMoves(l->ghost[i].pos, l->map);

	// printMapCell(w, l, l->ghost[i].pos.y, l->ghost[i].pos.x);
	if(l->ghost[i].flag == GH_EATEN && isSamePos(l->ghost[i].spawn, l->ghost[i].pos))
		l->ghost[i].flag = GH_NORMAL;

	//if the ghost has to go to home (it's eatend and has to be reset)
	if(l->ghost[i].flag == GH_EATEN)
		r = ghostNextMove(l->ghost[i].pos, bitmap);

	//the ghost is near the player
	if(r == 0 && l->ghost[i].ai < 0 && l->ghost[i].flag != GH_EATEN && l->ghost[i].flag != GH_SCARED) {
		if(l->ghost[i].pos.x == l->player.pos.x) {
			if(
				(l->ghost[i].pos.y - l->player.pos.y) >= 0 &&
			 	(l->ghost[i].pos.y - l->player.pos.y) <= -l->ghost[i].ai &&
				(moves & NORTH)
			)	l->ghost[i].way = r = NORTH; //the player is above the ghost (stupid way to reach it but nvm, i'll do something better)
			else if(
				(l->player.pos.y - l->ghost[i].pos.y) >= 0 &&
			 	(l->player.pos.y - l->ghost[i].pos.y) <= -l->ghost[i].ai &&
				(moves & SOUTH)
			)	l->ghost[i].way = r = SOUTH; //the player is below the ghost
		} else if(l->ghost[i].pos.y == l->player.pos.y) {
			if(
				(l->ghost[i].pos.x - l->player.pos.x) >= 0 &&
			 	(l->ghost[i].pos.x - l->player.pos.x) <= -l->ghost[i].ai &&
				(moves & WEST)
			)	l->ghost[i].way = r = WEST; //the player is left to the ghost
			else if(
				(l->player.pos.y - l->ghost[i].pos.y) >= 0 &&
			 	(l->player.pos.y - l->ghost[i].pos.y) <= -l->ghost[i].ai &&
				(moves & EAST)
			)	l->ghost[i].way = r = EAST; //the player is right to the user
		}
	}

	//the ghost continues its way
	if(r == 0)
		r = randomMove(moves, l->ghost[i].way);

	//the ghost gets moved
	shiftSprite(l->ghost[i], r ? (l->ghost[i].way = r) : l->ghost[i].way);
	// printGhost(w, l, i);
}

//END
