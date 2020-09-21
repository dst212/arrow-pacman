/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */


void printLevel(WINDOW*w, const Level*l, const short starty, const short startx, const short endy, const short endx) {
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

void levelUp(Level*l) {
	int8_t i;
	l->levels++;
	resetMap(l->map);
	resetPlayer(l->player);
	for(i = 0 ; i < l->ghosts; i++)
		resetGhost(l->ghost[i]);
	if(l->delay > 50)
		l->delay -= 10;
	if(l->powerticks > 20)
	 	l->powerticks -= 5;
}

void initLevel(Level*l) {
	//levels count
	l->levels = 1;
	//game's ticks count
	l->ticks = 0;
	//needed to keep track of when the last power point was caught
	l->checkpoint = -1;
	//framerate in ms / f (frame delay): 5 fps (initial)
	l->delay = 1000 / 5;
	//save variable fields
	l->initial.lives = l->player.lives;
	l->initial.powerticks = l->powerticks;
	//*philosophy mode on* the player starts it's first life *philosophy mode off*
	l->player.lives--;
}

void restartLevel(Level*l) {
	int8_t i;
	l->levels = 0;
	l->ticks = 0;
	l->checkpoint = -1;
	resetMap(l->map);
	resetPlayer(l->player);
	l->player.lives = l->initial.lives;
	for(i = 0 ; i < l->ghosts; i++)
		resetGhost(l->ghost[i]);
	l->powerticks = l->initial.powerticks;
	l->player.score = 0;
}

void beginPowerPoint(Level*l) {
	int8_t i;
	for(i = 0; i < l->ghosts; i++) {
		if(l->ghost[i].flag != GH_EATEN)
			l->ghost[i].flag = GH_SCARED;
	}
	l->checkpoint = l->ticks;
}

void endPowerPoint(Level*l) {
	int8_t i;
	for(i = 0; i < l->ghosts; i++) {
		if(l->ghost[i].flag == GH_SCARED)
			l->ghost[i].flag = GH_NORMAL;
	}
	l->checkpoint = -1;
}

//END
