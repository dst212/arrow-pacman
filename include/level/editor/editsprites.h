//

void levelEditGhost(WINDOW*w, level*l, int gi) {
	const char*menu[] = {
		" Close menu   ",
		" Name         ",
		" Sprites      ",
		" Spawnpoint   ",
		" Wait ticks   ",
		" Ray of sight ",
		" Delete (!)   "
	};
	const short menuWordLength = strlen(menu[0]) + 1;
	int i, j, ch, defColor;
	short r, x, y, sx, sy, startx;
	WINDOW*popwin = NULL;
	l->ghost[gi].spawn.x = x = getcurx(w);
	l->ghost[gi].spawn.y = y = getcury(w);
	do {
		resetGhost(l->ghost[gi]);
		printGhost(w, l, gi);
		wrefresh(w);
		startx = (l->ghost[gi].spawn.x < menuWordLength + 2 && l->ghost[gi].spawn.y < 8) ? WIN_WIDTH - menuWordLength : 0;
		defColor = revcolor(fgcolor(l->ghost[gi].color[GH_NORMAL]));
		if(defColor == 0) defColor = revcolor(DEFAULT_COLOR);
		nctempmsg(popwin, 0, startx, "@P  Edit ghost  ", defColor); //menu title
		ch = ncmenu(1, startx, sizeof(menu) / sizeof(char*), menuWordLength, menu, defColor, revcolor(defColor));
		redrawwin(w);
		wrefresh(w);
		switch(ch) {
			case 0:
				break;
			case 1:
				popwin = newwin(1, MAP_WIDTH, (l->ghost[gi].spawn.y > MAP_HEIGHT - 5) ? 0 : MAP_HEIGHT - 1, 0);
				ncfillwin(popwin, ' ', defColor);
				waddstr(popwin, "Ghost name: ");
				wrefresh(popwin);
				ncscan((l->ghost[gi].spawn.y > MAP_HEIGHT - 5) ? 0 : MAP_HEIGHT - 1, getcurx(popwin), GH_NAME_SIZE, GH_NAME_SIZE, l->ghost[gi].name, "", defColor);
				delwin(popwin);
				break;
			case 2:
				popwin = newwin(10, 18, (l->ghost[gi].spawn.y < 12 && l->ghost[gi].spawn.x > MAP_WIDTH - 21) ? MAP_HEIGHT - 10 : 0, MAP_WIDTH - 18);
				ncflags(popwin);
				setbgcolor(popwin, defColor);
				setcolor(popwin, defColor);
				waddstr(popwin, "Sprites:  ← ↑ → ↓\n");
				for(i = 0; i < GH_STATUSES; i++) {
					waddstr(popwin, "\n ");
					switch(i) {
						case GH_NORMAL:
							waddstr(popwin, "Normal: ");
							getyx(popwin, sy, sx);
							break;
						case GH_SCARED:
							waddstr(popwin, "Scared: ");
							break;
						case GH_EATEN:
							waddstr(popwin, "Eaten:  ");
							break;
						case GH_HUNTING:
							waddstr(popwin, "Hunting:");
							break;
					}
					setcolor(popwin, bgcolor(defColor) + ((l->ghost[gi].color[i] == l->ghost[gi].color[GH_NORMAL]) ? 0 : fgcolor(l->ghost[gi].color[i])));
					for(j = 0; j < 4; j++) { //print the sprite for each way
						waddch(popwin, ' ');
						waddnwstr(popwin, &(l->ghost[gi].s[i][j]), 1);
					}
					setcolor(popwin, defColor);
					waddch(popwin, '\n');
				}
				levelChangeSprites(popwin, sy, ++sx, GH_STATUSES, 4, l->ghost[gi].s, l->ghost[gi].color, defColor);
				delwin(popwin);
				break;
			case 3:
				redrawwin(w);
				levelPickSpawnpoint(w, l, &y, &x, &(l->ghost[gi].spawn));
				l->ghost[gi].pos = l->ghost[gi].spawn;
				break;
			case 4:
				popwin = newwin(1, MAP_WIDTH, (l->ghost[gi].spawn.y > MAP_HEIGHT - 2) ? 0 : MAP_HEIGHT - 1, 0);
				ncflags(popwin);
				ncfillrow(popwin, 0, 0, -1, ' ', defColor);
				waddstr(popwin, "Ticks of inactivity: ");
				ncnumberchar(popwin, getcury(popwin), getcurx(popwin), &(l->ghost[gi].ticks), 0, 70);
				delwin(popwin);
				break;
			case 5:
				popwin = newwin(1, MAP_WIDTH, (l->ghost[gi].spawn.y > MAP_HEIGHT - 2) ? 0 : MAP_HEIGHT - 1, 0);
				ncflags(popwin);
				ncfillrow(popwin, 0, 0, -1, ' ', defColor);
				waddstr(popwin, "Ray of sight: ");
				r = l->ghost[gi].ai;
				if(r < 0) r = -r;
				ncnumber(popwin, getcury(popwin), getcurx(popwin), &r, 1, 40);
				l->ghost[gi].ai = (l->ghost[gi].ai < 0) ? -r : r ;
				delwin(popwin);
				break;
			case 6:
				popwin = newwin(1, MAP_WIDTH, (l->ghost[gi].spawn.y > MAP_HEIGHT - 2) ? 0 : MAP_HEIGHT - 1, 0);
				ncflags(popwin);
				ncfillrow(popwin, 0, 0, -1, ' ', defColor);
				if(tolower(ncask(popwin, "Are you sure to delete this ghost? (Y/N)", "\rYyNn") == 'y')) {
					ch = -2; //ghost deletion
					printMapCell(w, l, l->ghost[gi].pos.y, l->ghost[i].pos.x);
					l->ghost[gi] = l->ghost[l->ghosts - 1];
					l->ghosts--;
				}
				delwin(popwin);
				break;
		}
		redrawwin(w);
		wrefresh(w);
	} while(ch != 0 && ch != -2);
}

void levelEditPlayer(WINDOW*w, level*l) {
	const char*menu[] = {
		" Close menu    ",
		" Spawnpoint    ",
		" Sprites       "
	};
	const short menuWordLength = strlen(menu[0]) + 1;
	int i, ch, defColor;
	short x, y, sx, sy, startx;
	WINDOW*popwin = NULL;
	x = getcurx(w);
	y = getcury(w);
	if(!validCoord(l->player.spawn.y, l->player.spawn.x)) {
		l->player.spawn.x = x;
		l->player.spawn.y = y;
	}
	do {
		resetPlayer(l->player);
		printPlayer(w, l);
		wrefresh(w);
		startx = (l->player.spawn.x < menuWordLength + 2 && l->player.spawn.y < 8) ? WIN_WIDTH - menuWordLength : 0;
		defColor = revcolor(fgcolor(l->player.color));
		if(defColor == 0) defColor = revcolor(DEFAULT_COLOR);
		nctempmsg(popwin, 0, startx, "@P  Edit player  ", defColor); //menu title
		ch = ncmenu(1, startx, sizeof(menu) / sizeof(char*), menuWordLength, menu, defColor, revcolor(defColor));
		redrawwin(w);
		wrefresh(w);
		switch(ch) {
			case 0:
				break;
			case 1:
				levelPickSpawnpoint(w, l, &y, &x, &(l->player.spawn));
				l->player.pos = l->player.spawn;
				break;
			case 2:
				popwin = newwin(4, 18, (l->player.spawn.y < 12 && l->player.spawn.x > MAP_WIDTH - 21) ? MAP_HEIGHT - 10 : 0, MAP_WIDTH - 18);
				ncflags(popwin);
				setbgcolor(popwin, defColor);
				setcolor(popwin, defColor);
				waddstr(popwin, "Sprites: ← ↑ → ↓\n\n Normal: ");
				getyx(popwin, sy, sx);
				setcolor(popwin, bgcolor(defColor));
				for(i = 0; i < 4; i++) { //print the sprite for each way
					waddch(popwin, ' ');
					waddnwstr(popwin, &(l->player.s[i]), 1);
				}
				setcolor(popwin, defColor);
				waddch(popwin, '\n');
				//here below, l->player.s and l->player.color ar treated as a matrix and a array respectively
				//because the function was built for a ghost's sprite, but works also on the player's one
				levelChangeSprites(popwin, sy, ++sx, 1, 4, &(l->player.s), &(l->player.color), defColor);
				delwin(popwin);
				break;
		}
		redrawwin(w);
		wrefresh(w);
	} while(ch != 0);
}

//END
