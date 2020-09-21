/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//

void lvlError(WINDOW*w, const Level_status s) {
	wprintw(w,
		(s.status == LVL_OLD)					? LVL_OLD_MSG :					//map is written with an older format (it will need a converter)
		(s.status == LVL_INVALID)				? LVL_INVALID_MSG :				//file found but invalid format
		(s.status == LVL_NOT_FOUND)				? LVL_NOT_FOUND_MSG :			//file not found
		(s.status == LVL_DENIED)				? LVL_DENIED_MSG :				//file inaccessible
		(s.status == LVL_BUSY_SPAWN)			? LVL_BUSY_SPAWN_MSG :			//pacman's spawn point is filled with a wall
		(s.status == LVL_WRONG_SPAWN)			? LVL_WRONG_SPAWN_MSG :			//pacman's spawn point is out of the map
		(s.status == LVL_WRONG_GHOSTS_COORDS)	? LVL_WRONG_GHOSTS_COORDS_MSG :	//some of ghosts' coords were wrong
		(s.status == LVL_GHOST_OUT_OF_MAP)		? LVL_GHOST_OUT_OF_MAP_MSG :	//some of ghosts' coords were wrong
		(s.status == LVL_NO_GHOSTS)				? LVL_NO_GHOSTS_MSG :			//there were no ghosts
		(s.status == LVL_WRONG_CELL)			? LVL_WRONG_CELL_MSG :			//wrong map-cell's data
		(s.status == LVL_WRONG_PLAYER_LIVES)	? LVL_WRONG_PLAYER_LIVES_MSG :	//player's liver are 0 or less
		LVL_UNKNOWN_ERROR														//something else
	);
	if(s.status == LVL_WRONG_CELL || s.status == LVL_BUSY_SPAWN || s.status == LVL_WRONG_GHOSTS_COORDS)
		wprintw(w, "%d, %d.\n", s.data.x, s.data.y);
	redrawwin(w);
	wrefresh(w);
}


Level_status checkGhosts(const Map m[MAP_HEIGHT][MAP_WIDTH], Ghost g[LVL_MAX_GHOSTS], int8_t*ng, const bool autoFix) {
	//the code below is a total mess. TO FIX
	short i;
	Level_status s;
	s.status = LVL_OK;
	s.data.x = -1;
	s.data.y = -1;
	if(*ng == 0) {
		s.status = LVL_NO_GHOSTS;
		if(autoFix) {
			lvlError(stdscr, s);
			lvlAutoFix(stdscr, "Add some, else it's too easy.\n");
		}
	} else {
		for(i = 0; i < *ng && (s.status == LVL_OK || autoFix); i++) {
			resetGhost(g[i]);
			if(!validCoord(g[i].spawn.y, g[i].spawn.x)) {
				s.status = LVL_GHOST_OUT_OF_MAP;
				s.data = g[i].spawn;
				g[i--] = g[--(*ng)];
				lvlError(stdscr, s);
				lvlAutoFix(stdscr, "This ghost has been deleted.\n");
			} else if(
				!isBypassable(m, g[i].spawn.y, g[i].spawn.x) &&
				!isCrossable(m, g[i].spawn.y, g[i].spawn.x)
			) {
				s.status = LVL_WRONG_GHOSTS_COORDS;
				s.data = g[i].spawn;
				lvlError(stdscr, s);
				lvlAutoFix(stdscr, "Change this ghost's spawn point.\n");
			}
		}
	}
	return s;
}


Level_status checkPlayer(Map m[MAP_HEIGHT][MAP_WIDTH], Player*p, const bool autoFix) {
	Level_status s;
	s.status = LVL_OK;
	s.data.x = -1;
	s.data.y = -1;
	if(!validCoord(p->spawn.y, p->spawn.x)) {
		s.status = LVL_WRONG_SPAWN, s.data = p->spawn;
		if(autoFix) {
			lvlError(stdscr, s);
			lvlAutoFix(stdscr, "Be sure to place the spawn point in a valid position.\n");
		}
	} else if(!isCrossable(m, p->spawn.y, p->spawn.x)) {
		s.status = LVL_BUSY_SPAWN;
		s.data = p->spawn;
		if(autoFix) {
			lvlError(stdscr, s);
			m[p->spawn.y][p->spawn.x].portal.x = MAP_NONWALL;
			m[p->spawn.y][p->spawn.x].portal.y = MAP_SPACE;
			lvlAutoFix(stdscr, "The spawn point has ben set to a blank space.\n");
		}
	}
	if(p->lives <= 0 || p->lives > PL_MAX_LIVES) {
		s.status = LVL_WRONG_PLAYER_LIVES;
		if(autoFix) {
			lvlError(stdscr, s);
			p->lives = PL_DEFAULT_LIVES;
			lvlAutoFix(stdscr, "Player's lives changed to %d.\n", p->lives);
		}
	}
	p->pos = p->spawn;
	p->way = WEST;
	p->nextway = p->way;
	return s;
}


Level_status checkMap(Map m[MAP_HEIGHT][MAP_WIDTH], const bool autoFix){
	Level_status s;
	int i, j;
	s.status = LVL_OK;
	s.data.x = -1;
	s.data.y = -1;
	for(i = 0; i < MAP_HEIGHT && (s.status == LVL_OK || autoFix); i++) {
		for(j = 0; j < MAP_WIDTH && (s.status == LVL_OK || autoFix); j++) {
			if(
				!isCrossable(m, i, j) &&
				!isGenericWall(m, i, j) &&
				!isPortal(m, i, j)
			) {
				s.status = LVL_WRONG_CELL;
				s.data.x = j, s.data.y = i;
				if(autoFix) {
					lvlError(stdscr, s);
					m[i][j].portal.x = MAP_NONWALL;
					m[i][j].portal.y = MAP_SPACE;
					lvlAutoFix(stdscr, "That portal has been disabled.\n");
				}
			}
		}
	}
	return s;
}

//END
