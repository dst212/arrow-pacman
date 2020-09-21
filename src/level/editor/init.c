/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in main.c at root directory.
 */

void initPlayer(Player*p) {
	p->s[INDEX_WAY(WEST)] = L'<';
	p->s[INDEX_WAY(NORTH)] = L'ʌ';
	p->s[INDEX_WAY(EAST)] = L'>';
	p->s[INDEX_WAY(SOUTH)] = L'v';
	p->deathanim[0] = L' ';
	p->deathanim[1] = L'×';
	p->deathanim[2] = L' ';
	p->deathanim[3] = L'×';
	p->color = 0x0B;
	p->spawn.x = -1;
	p->spawn.y = -1;
	p->lives = PL_DEFAULT_LIVES;
	p->score = 0;
	resetPlayer(*p);
}

void initGhost(Ghost*g) {
	int i, j;
	memset(g->name, 0, GH_NAME_SIZE);
	g->spawn.x = -1;
	g->spawn.y = -1;
	g->ai = -7;
	for(i = 0; i < GH_STATUSES; i++) {
		switch(i) {
			case GH_SCARED:
				g->color[i] = COLOR_BLUE;
				for(j = 0; j < 4; j++)
					g->s[i][j] = L'ᗣ';
				break;
			case GH_EATEN:
				g->color[i] = COLOR_WHITE + 8;
				for(j = 0; j < 4; j++)
					g->s[i][j] = L'¨';
				break;
			default:
				g->color[i] = COLOR_RED + 8;
				for(j = 0; j < 4; j++)
					g->s[i][j] = L'ᗣ';
				break;
		}
	}
	g->ticks = 12;
	resetGhost(*g);
}

void initMap(Map m[MAP_HEIGHT][MAP_WIDTH]) {
	short i, j;
	for(i = 0; i < MAP_HEIGHT; i++) for(j = 0; j < MAP_WIDTH; j++) {
		m[i][j].color = 0x0F;
		m[i][j].wall = L' ';
		m[i][j].portal.x = MAP_NONWALL;
		m[i][j].portal.y = MAP_SPACE;
	}
}

void newLevel(Level*l){
	int i;
	initMap(l->map);
	initPlayer(&(l->player));
	for(i = 0; i < LVL_MAX_GHOSTS; i++) {
		initGhost(&l->ghost[i]);
		resetGhost(l->ghost[i]);
	}
	l->ghosts = 0;
	l->powerticks = 60;
}

//END
