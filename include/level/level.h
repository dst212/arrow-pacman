/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in main.c at root directory.
 */

//functions about the level struct

void printLevel(WINDOW*w, const level*l, const short starty, const short startx, const short endy, const short endx) {
	int i, j;
	for(i = starty; i < endy; i++) {
		for(j = startx; j < endx; j++) {
			if(isPowerPoint(l->map, i, j))
				wattron(w, A_BLINK);
			setcolor(w, l->map[i][j].color);
			mvwaddnwstr(w, i, j, &(l->map[i][j].wall), 1);
			if(isPowerPoint(l->map, i, j))
				wattroff(w, A_BLINK);
		}
	}
	if(	l->player.spawn.y >= starty &&
		l->player.spawn.x >= startx &&
		l->player.spawn.y < endy &&
		l->player.spawn.x < endx
	) {
		printPlayer(w, l);
	}
	for(i = 0; i < l->ghosts; i++) {
		if(	l->ghost[i].spawn.y >= starty &&
			l->ghost[i].spawn.x >= startx &&
			l->ghost[i].spawn.y < endy &&
			l->ghost[i].spawn.x < endx
		) {
			printGhost(w, l, i);
		}
	}
}

void levelUp(level*l, counter*ticks) {
	int8_t i;
	ticks->levels++;
	resetMap(l->map);
	resetPlayer(l->player);
	for(i = 0 ; i < l->ghosts; i++)
		resetGhost(l->ghost[i]);
	if(ticks->delay > 50)
		ticks->delay -= 10;
	if(l->powerticks > 20)
	 	l->powerticks -= 5;
}

void initLevel(level*l, counter*ticks) {
	//levels count
	ticks->levels = 0;
	//game's ticks count
	ticks->count = 0;
	//needed to keep track of when the last power point was caught
	ticks->checkpoint = -1;
	//framerate in ms / f (frame delay): 5 fps (initial)
	ticks->delay = 1000 / 5;
	//save variable fields
	ticks->initial_lives = l->player.lives;
	ticks->initial_powerticks = l->powerticks;
	//*philosophy mode on* the player starts it's first life *philosophy mode off*
	l->player.lives--;
	ticks->levels++;
}

void restartLevel(level*l, counter*ticks) {
	int8_t i;
	ticks->levels = 0;
	ticks->count = 0;
	ticks->checkpoint = -1;
	resetMap(l->map);
	resetPlayer(l->player);
	l->player.lives = ticks->initial_lives;
	for(i = 0 ; i < l->ghosts; i++)
		resetGhost(l->ghost[i]);
	l->powerticks = ticks->initial_powerticks;
	l->player.score = 0;
}

void beginPowerPoint(level*l, counter*ticks) {
	int8_t i;
	for(i = 0; i < l->ghosts; i++) {
		if(l->ghost[i].flag != GH_EATEN)
			l->ghost[i].flag = GH_SCARED;
	}
	ticks->checkpoint = ticks->count;
}

void endPowerPoint(level*l, counter*ticks) {
	int8_t i;
	for(i = 0; i < l->ghosts; i++) {
		if(l->ghost[i].flag == GH_SCARED)
			l->ghost[i].flag = GH_NORMAL;
	}
	ticks->checkpoint = -1;
}
//END
