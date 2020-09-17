/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in main.c at root directory.
 */

//player's sprite functions

extern void beginPowerPoint(level*);

#define PL_MAX_LIVES		7
#define PL_DEFAULT_LIVES	3

#define resetPlayer(s)			do { (s).pos = (s).spawn, (s).nextway = (s).way = WEST; } while(0);
#define playerCanMove(f, y, x)	(validCoord(y, x) && (isCrossable(f, y, x) || isPortal(f, y, x)))
#define isPlayerSpawn(l, i, j)	(i == (l)->player.spawn.y && j == (l)->player.spawn.x)
#define teleport(p, i, j) 		do { (p).pos.y = (i), (p).pos.x = (j); } while(0);

void printPlayer(WINDOW*w, const level*l) {
	setcolor(w, bgcolor((l)->map[(l)->player.pos.y][(l)->player.pos.x].color) + fgcolor((l)->player.color));
	mvwaddnwstr(w, (l)->player.pos.y, (l)->player.pos.x, &((l)->player.s[INDEX_WAY((l)->player.way)]), 1);
}

way_t playerMoves(coord pos, map_t map[MAP_HEIGHT][MAP_WIDTH]) {
	way_t r = 0;
	//the player can choose these ways
	if(playerCanMove(map, pos.y, pos.x - 1)) r |= WEST;
	if(playerCanMove(map, pos.y, pos.x + 1)) r |= EAST;
	if(playerCanMove(map, pos.y - 1, pos.x)) r |= NORTH;
	if(playerCanMove(map, pos.y + 1, pos.x)) r |= SOUTH;
	return r;
}

#define shiftPlayer(l, p) do { \
	switch (p) { \
		case WEST:	(l)->player.pos.x--; break; \
		case EAST:	(l)->player.pos.x++; break; \
		case NORTH:	(l)->player.pos.y--; break; \
		case SOUTH:	(l)->player.pos.y++; break; \
	} \
} while(0)

#define PACMAN_EATEN -2
#define PACMAN_GAMEOVER -1
#define PACMAN_CONTINUE 0
#define PACMAN_WIN 1

#define PACMAN_POINT_SCORE 1
#define PACMAN_PPOINT_SCORE 5
#define PACMAN_GHOST_SCORE 20

int8_t playerGotCaught(level*l) {
	int8_t i = -1;
	return (i = ghostPos(l, l->player.pos.y, l->player.pos.x)) != -1 ? i : ghostHit(l, l->player.pos.y, l->player.pos.x, l->player.way);
}

short movePlayer(/*WINDOW*w, */level*l) {
	short r = PACMAN_CONTINUE;
	way_t moves = playerMoves(l->player.pos, l->map);
	int8_t i = -1;
	// unprintPlayer(w, l);

	//the player got caught or reached a ghost
	if((i = playerGotCaught(l)) != -1) {
		switch(l->ghost[i].flag) {
			case GH_NORMAL: case GH_HUNTING:
				if(l->player.lives-- == 0)
					r = PACMAN_GAMEOVER;
				else
					r = PACMAN_EATEN;
				l->i = i;
				break;
			case GH_SCARED:
				l->player.score += PACMAN_GHOST_SCORE;
				l->ghost[i].flag = GH_EATEN;
		}
	}

	//the player is still alive, then it gets moved
	if(r == PACMAN_CONTINUE) {
		if(moves & l->player.nextway) {
			l->player.way = l->player.nextway;
			shiftPlayer(l, l->player.way);
		} else if(moves & l->player.way) {
			shiftPlayer(l, l->player.way);
		}
		if(isPoint(l->map, l->player.pos.y, l->player.pos.x)) {
			l->player.score += PACMAN_POINT_SCORE;
			l->map[l->player.pos.y][l->player.pos.x].portal.y = MAP_EATENPOINT;
			if(mapIsEmpty(l->map))
				r = PACMAN_WIN;
		} else if(isPowerPoint(l->map, l->player.pos.y, l->player.pos.x)) {
			l->player.score += PACMAN_PPOINT_SCORE;
			l->map[l->player.pos.y][l->player.pos.x].portal.y = MAP_EATENPPOINT;
			beginPowerPoint(l);
			if(mapIsEmpty(l->map))
				r = PACMAN_WIN;
		} else if(isPortal(l->map, l->player.pos.y, l->player.pos.x)) {
			teleport(l->player, l->map[l->player.pos.y][l->player.pos.x].portal.y, l->map[l->player.pos.y][l->player.pos.x].portal.x);
		}
	}
	// printPlayer(w, l);
	return r;
}

void playerDeath(WINDOW*w, level*l) {
	short i;
	setcolor(w, l->player.color);
	for(i = 0; i < 4; i++) {
		mvwaddnwstr(w, l->player.pos.y, l->player.pos.x, &(l->player.deathanim[i]), 1);
		wrefresh(w);
		msleep(250);
	}
	msleep(750);
}

//END
