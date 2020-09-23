/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

void printPlayer(WINDOW*w, const Level*l) {
	setcolor(w, bgcolor((l)->map[(l)->player.pos.y][(l)->player.pos.x].color) + fgcolor((l)->player.color));
	mvwaddnwstr(w, (l)->player.pos.y, (l)->player.pos.x, &((l)->player.s[INDEX_WAY((l)->player.way)]), 1);
}

Way playerMoves(Coord pos, Map map[MAP_HEIGHT][MAP_WIDTH]) {
	Way r = 0;
	//the player can choose these ways
	if(playerCanMove(map, pos.y, pos.x - 1)) r |= WEST;
	if(playerCanMove(map, pos.y, pos.x + 1)) r |= EAST;
	if(playerCanMove(map, pos.y - 1, pos.x)) r |= NORTH;
	if(playerCanMove(map, pos.y + 1, pos.x)) r |= SOUTH;
	return r;
}

int8_t playerGotCaught(const Level*l) {
	int8_t i = -1;
	return (i = ghostPos(l, l->player.pos.y, l->player.pos.x)) != -1 ? i : ghostHit(l, l->player.pos.y, l->player.pos.x, l->player.way);
}

//player death animation
void playerDeath(WINDOW*w, const Level*l) {
	short i;
	setcolor(w, l->player.color);
	for(i = 0; i < 4; i++) {
		mvwaddnwstr(w, l->player.pos.y, l->player.pos.x, &l->player.deathanim[i], 1);
		wrefresh(w);
		msleep(250);
	}
	msleep(750);
}

short movePlayer(Level*l) {
	short r = PACMAN_CONTINUE;
	Way moves = playerMoves(l->player.pos, l->map);
	int8_t i = -1;

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
			shiftSprite(&l->player.pos, l->player.way);
		} else if(moves & l->player.way) {
			shiftSprite(&l->player.pos, l->player.way);
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
			l->player.pos = l->map[l->player.pos.y][l->player.pos.x].portal;
		}
	}
	return r;
}

//END
