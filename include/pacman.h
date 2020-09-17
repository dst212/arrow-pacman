/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in main.c at root directory.
 */

//main game functions, the game "engine"

int pacmanCountDown(WINDOW*w) {
	#define WIN_MIDDLE (WIN_HEIGHT - 1) / 2
	int8_t i;
	setcolor(w, COLOR_WHITE + 8);
	for(i = 3; i > 0; i--) {
		mvwprintw(w, WIN_MIDDLE, WIN_WIDTH - 1, "%1d", i);
		wrefresh(w);
		msleep(1000);
	}
	mvncprintv(w, WIN_MIDDLE, WIN_WIDTH - 1, "GO\0");
	wrefresh(w);
	msleep(1000);
	#undef WIN_MIDDLE
	return ncignoreinput(w);
}

void refreshBar(WINDOW*w, const level*l) {
	int8_t i;
	char score[10];
	ncfillcol(w, 0, WIN_WIDTH - 1, WIN_HEIGHT, ' ', COLOR_WHITE + 8);
	if(!is_nodelay(w)) mvwaddch(w, 0, WIN_WIDTH - 1, 'P'); //if the game was paused, show that
	//print the live
	setcolor(w, COLOR_RED + 8);
	for(i = 0; i < l->player.lives; i++)
		mvwaddwstr(w, 1 + i, WIN_WIDTH - 1, PACMAN_HEART);
	//print the level
	setcolor(w, COLOR_WHITE + 8);
	num2str(l->levels, score, 10);
	mvncprintv(w, WIN_HEIGHT - 1 - strlen(score) - 1, WIN_WIDTH - 1, "-");
	ncprintv(w, score);
	//print the score
	mvncprintv(w, WIN_HEIGHT - 1 - strlen(score) - strlen(num2str(l->player.score, score, 10)) - 1, WIN_WIDTH - 1, score);
}

void pacmanKeys(WINDOW*w, level*l, int ch) {
	switch(ch) {
		case ERR:
			break;
		//move the PacMan
		case KEY_UP: case 'w':
			l->player.nextway = NORTH;
			break;
		case KEY_LEFT: case 'a':
			l->player.nextway = WEST;
			break;
		case KEY_DOWN: case 's':
			l->player.nextway = SOUTH;
			break;
		case KEY_RIGHT: case 'd':
			l->player.nextway = EAST;
			break;
		case 'p': //pause the game
			nodelay(w, false);
			refreshBar(w, l);
			wrefresh(w);
			ch = wgetch(w);
			nodelay(w, true);
			break;
	}
}

void startPacMan(void) {
	char levelName[MAX_STR_SIZE + 1] = "";
	char msg[MAX_STR_SIZE + GH_NAME_SIZE + 1] = "";
	levelStatus reason;
	level pacman;
	uint16_t*bitmap[LVL_MAX_GHOSTS]; //ghosts' routes
	short i, gameStatus = PACMAN_CONTINUE;
	int ch;
	bool playAgain = false;
	WINDOW*w;
	w = newwin(WIN_HEIGHT, WIN_WIDTH, 0, 0);
	ncflags(w);
	wclear(w);
	mvwaddstr(w, 0, 0, "Type the level file's name: ");
	wrefresh(w);
	ncscan(getcury(w), getcurx(w), MAX_STR_SIZE + 1, MAX_STR_SIZE, levelName, PACMAN_FILE_CHARS, A_NORMAL);
	reason = loadLevel(w, levelName, &pacman, false);
	if(reason.status == LVL_OK) {
		//the map gets loaded and is printed and the game starts
		for(i = 0; i < pacman.ghosts; i++) {
			bitmap[i] = (uint16_t*)calloc(1, sizeof(uint16_t) * MAP_HEIGHT * MAP_WIDTH);
			ghostRoutes(&pacman, i, bitmap[i], pacman.ghost[i].spawn, 0, 0);
		}
		curs_set(0);
		do { //init game
			nodelay(w, true);
			initLevel(&pacman);
			refreshBar(w, &pacman);
			ch = pacmanCountDown(w);
			do { //game cycle
				//perform action based on key press
				ch = tolower(wgetch(w));
				pacmanKeys(w, &pacman, ch);
				//print all to the output, refresh and wait
				unprintSprites(w, &pacman);
				for(i = 0; i < pacman.ghosts; i++)
					if(pacman.ghost[i].flag != GH_SCARED || pacman.ticks % 2 == 0) {
						moveGhost(&pacman, i, bitmap[i]);
						printGhost(w, &pacman, i);
					} else
						reprintGhost(w, &pacman, i, (pacman.map[pacman.ghost[i].pos.y][pacman.ghost[i].pos.x].color < 128 /*dark background*/) ? COLOR_WHITE + 8 : COLOR_BLACK);
				gameStatus = movePlayer(&pacman);
				printPlayer(w, &pacman);
				refreshBar(w, &pacman);
				wrefresh(w);
				msleep(pacman.delay / 2);
				//check game's status
				switch(gameStatus) {
					case PACMAN_CONTINUE:
						//eaten ghosts are twice faster
						for(i = 0; i < pacman.ghosts; i++) {
								if(pacman.ghost[i].flag == GH_EATEN) {
								printMapCell(w, &pacman, pacman.ghost[i].pos.y, pacman.ghost[i].pos.x);
								moveGhost(&pacman, i, bitmap[i]);
								printGhost(w, &pacman, i);
							}
						}
						wrefresh(w);
						msleep(pacman.delay / 2);
						pacman.ticks++;
						if(pacman.checkpoint != -1 && pacman.ticks - pacman.checkpoint >= pacman.powerticks)
							endPowerPoint(&pacman);
						break;
					case PACMAN_GAMEOVER:
					case PACMAN_EATEN:
						for(i = 0; i < pacman.ghosts; i++) {
							printMapCell(w, &pacman, pacman.ghost[i].pos.y, pacman.ghost[i].pos.x);
							resetGhost(pacman.ghost[i]);
						}
						playerDeath(w, &pacman);
						strcpy(msg, "");
						strcat(strcat(strcat(msg, "You got caught by "), pacman.ghost[pacman.i].name), "!");
						ncpopup_info(NC_POPUP_CENTER(w, 1, 30), revcolor(pacman.ghost[pacman.i].color[GH_NORMAL]), msg, "OK");
						redrawwin(w);
						if(gameStatus != PACMAN_GAMEOVER) {
							printMapCell(w, &pacman, pacman.player.pos.y, pacman.player.pos.x);
							resetPlayer(pacman.player);
							printSprites(w, &pacman);
							wrefresh(w);
							msleep(200);
							// pacmanCountDown(w);
						}
						break;
					case PACMAN_WIN:
						levelUp(&pacman);
						printLevel(w, &pacman, 0, 0, MAP_HEIGHT, MAP_WIDTH);
						pacmanCountDown(w);
						break;
				}
			} while(gameStatus != PACMAN_GAMEOVER);
			nodelay(w, false);
			wrefresh(w);
			wmove(w, WIN_HEIGHT - 1, 0);
			if((playAgain = ncpopup_bool(NC_POPUP_CENTER(w, 1, 16), revcolor(DEFAULT_COLOR), "  Play again?  ", NC_POPUP_YES, NC_POPUP_NO))) {
				restartLevel(&pacman);
				printLevel(w, &pacman, 0, 0, MAP_HEIGHT, MAP_WIDTH);
				redrawwin(w);
				wrefresh(w);
			}
		} while(playAgain);
		curs_set(1);
		for(i = 0; i < pacman.ghosts; i++)
			free(bitmap[i]);
	} else {
		lvlError(stdscr, reason);
	}
	delwin(w);
}

void mainMenu(WINDOW*mainwin) {
	#define PACMAN_PLAY 0
	#ifndef _PACMAN_COMPILE_FOR_SERVER
		#define PACMAN_LEVEL_EDITOR PACMAN_PLAY + 1
	#else
		#define PACMAN_LEVEL_EDITOR 0
	#endif
	#define PACMAN_SCORES PACMAN_LEVEL_EDITOR + 1
	#define PACMAN_ABOUT PACMAN_SCORES + 1
	#define PACMAN_QUIT PACMAN_ABOUT + 1

	int defColor = DEFAULT_COLOR, choice;
	const char*menu[] = {
		"* Play",
		#ifndef _PACMAN_COMPILE_FOR_SERVER
		"* Editor",
		#endif
		"* Scores",
		"* About",
		"* Quit"
	};
	do{
		pacmanWelcome(mainwin, 0, 0, defColor);
		waddwstr(mainwin, L"Use ↓↑ to move and ENTER to select.\n\n");
		wrefresh(mainwin);
		choice = ncmenu(getcury(mainwin), getcurx(mainwin) + 1, sizeof(menu) / sizeof(menu[0]), 15, menu, defColor, revcolor(defColor));
		switch(choice) {
			case PACMAN_PLAY:
				startPacMan();
				break;
			#ifndef _PACMAN_COMPILE_FOR_SERVER
			case PACMAN_LEVEL_EDITOR:
				levelEditor();
				break;
			#endif
			case PACMAN_SCORES:
				listScores();
				break;
			case PACMAN_ABOUT:
				printLicense();
		}
		wclear(mainwin);
	} while(choice != PACMAN_QUIT);
}

//END
