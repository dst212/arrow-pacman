//

void levelPickSpawnpoint(WINDOW*w, level*l, short*y, short*x, coord*spawn) {
	int ch;
	do {
		levelUpdateSideBar(w, l, *y, *x, true);
		wmove(w, *y, *x);
		wrefresh(w);
		ch = tolower(wgetch(w));
		switch(ch) {
			arrowKeysShift(*y, MAP_HEIGHT, 5, *x, MAP_WIDTH, 10);
			case KEY_ENTER: case NC_ENTER:
				if(!isWall(l->map, *y, *x)) {
					setcolor(w, l->map[spawn->y][spawn->x].color);
					mvwaddnwstr(w, spawn->y, spawn->x, &(l->map[spawn->y][spawn->x].wall), 1);
					spawn->x = *x;
					spawn->y = *y;
				} else {
					ch = ERR;
					deniedPos(w, l, *y, *x);
				}
				break;
			case QUIT_KEYS:
				*x = spawn->x;
				*y = spawn->y;
				break;
		}
	} while(ch != KEY_ENTER && ch != NC_ENTER && !isQuitKey(ch));
}

void levelPlacePortal(WINDOW*w, level*l) {
	int x, y, cx, cy, ch = ERR;
	getyx(w, y, x);
	wchgat(w, 1, A_REVERSE, l->map[y][x].color, NULL);
	//make cx and cy equal to current portal destination, if set:
	if(!outOfMap(l->map[y][x].portal))
		cx = l->map[y][x].portal.x, cy = l->map[y][x].portal.y;
	else
		cx = x, cy = y;
	do {
		if(ch == KEY_ENTER || ch == NC_ENTER) //it didn't quit the do-while cycle, the wall wasn't a valid wall
			deniedPos(w, l, cy, cx);
		levelUpdateSideBar(w, l, cy, cx, true);
		wmove(w, cy, cx);
		wrefresh(w);
		ch = tolower(wgetch(w));
		switch(ch) {
			arrowKeysShift(cy, MAP_HEIGHT, 10, cx, MAP_WIDTH, 10);
			case KEY_ENTER: case NC_ENTER:
				if(	(isCrossable(l->map, cy, cx) || isPortal(l->map, cy, cx)) &&
					!(cx == x && cy == y) &&
					!isPlayerSpawn(l, cy, cx) &&
					ghostSpawn(l, cy, cx) == -1
				)
					l->map[y][x].portal.x = cx, l->map[y][x].portal.y = cy;
				else {
					deniedPos(w, l, cy, cx);
					ch = ERR;
				}
				break;
			case QUIT_KEYS:
				break;
		}
	} while(ch != KEY_ENTER && ch != NC_ENTER && !isQuitKey(ch));
	mvwchgat(w, y, x, 1, A_NORMAL, l->map[y][x].color, NULL);
}

bool levelPickColor(WINDOW*w, short y, short x, uint8_t*color) {
	int ch, i;
	short cx;
	int8_t bg = 0; //background color is selected
	uint8_t initialColor = *color;
	WINDOW*pickColor;
	pickColor = newwin(1, MAP_WIDTH, (y < MAP_HEIGHT - 4) ? MAP_HEIGHT - 1 : 0, 0);
	ncflags(pickColor);
	wclear(pickColor);
	mvwprintw(pickColor, 0, 0, "Pick a color (arrows to move/change, enter to confirm): ");
	cx = getcurx(pickColor);
	do {
		setcolor(pickColor, *color);
		setbgcolor(pickColor, *color);
		mvwprintw(pickColor, 0, cx, "%2X", *color);
		wmove(pickColor, 0, cx + 1 - bg);
		redrawwin(pickColor);
		wrefresh(pickColor);
		ch = tolower(wgetch(pickColor));
		switch(ch) {
			case NC_EXIT: case KEY_EXIT: case 'q':
				*color = initialColor;
				break;
			case KEY_LEFT: case KEY_RIGHT:
				bg = bg ? 0 : 1;
				break;
			case KEY_UP:
				if(bg)
					*color += 16U;
				else
					*color += 1;
				break;
			case KEY_DOWN:
				if(bg)
					*color -= 16U;
				else
					*color -= 1U;
				break;
		}
		if(y == -1 && x == -1) {
			curs_set(0);
			for(i = 0; i < MAP_HEIGHT; i++)
				mvwchgat(w, i, 0, -1, A_NORMAL, *color, NULL);
			curs_set(1);
		} else {
			mvwchgat(w, y, x, 1, A_NORMAL, *color, NULL);
		}
		wrefresh(w);
	} while(ch != KEY_ENTER && ch != NC_ENTER && !isQuitKey(ch));
	delwin(pickColor);
	redrawwin(w);
	return(ch == NC_ENTER || ch == KEY_ENTER);
}

void levelPickWall(WINDOW*w, level*l, short y, short x) {
	int i, j, ch;
	WINDOW*sidewin;
	bool refreshList = true; //refresh the wall list
	wchar_t outChar = L'\0', initialWall = l->map[y][x].wall;
	sidewin = newwin(WIN_HEIGHT, 1, 0, WIN_WIDTH - 1);
	ncflags(sidewin);
	setcolor(sidewin, l->map[y][x].color);
	setbgcolor(sidewin, l->map[y][x].color);
	setcolor(w, l->map[y][x].color);
	wclear(sidewin);
	redrawwin(sidewin);
	j = 0;
	do {
		if(refreshList) {
			wmove(sidewin, 0, 0);
			for(i = 0; i < CHSTUFF_GROUP; i++) {
				waddch(sidewin, ' ');
				outChar = charBorders((j - (j % CHSTUFF_GROUP)) + i);
				waddnwstr(sidewin, &outChar, 1);
			}
			refreshList = false;
		}
		wmove(sidewin, (j % CHSTUFF_GROUP + 1) * 2 - 1, 0); //move the cursor to the current selected wall
		wrefresh(sidewin);
		ch = wgetch(sidewin);
		switch(ch) {
			case KEY_DOWN: //select the following wall in the list
				j = (j + 1) % CHSTUFF_BORDERS;
				if(j % CHSTUFF_GROUP == 0 || j % CHSTUFF_GROUP == CHSTUFF_GROUP - 1)
					refreshList = true;
				break;
			case KEY_UP: //select the previous wall in the list
				j = (j - 1 + CHSTUFF_BORDERS) % CHSTUFF_BORDERS;
				if(	j % CHSTUFF_GROUP == 0 ||
					j % CHSTUFF_GROUP == CHSTUFF_GROUP - 1 ||
					j == CHSTUFF_BORDERS - 1
				)	refreshList = true;
				break;
			case KEY_LEFT: //go backward to the previous group of walls
				j = (j - CHSTUFF_GROUP + CHSTUFF_BORDERS) % CHSTUFF_BORDERS;
				refreshList = true;
				break;
			case KEY_RIGHT: //go forward to the next group of walls
				j = (j + CHSTUFF_GROUP) % CHSTUFF_BORDERS;
				refreshList = true;
				break;
			case KEY_EXIT: case NC_EXIT: case 'q': case 'Q':
				l->map[y][x].wall = initialWall;
				//intentionally omitted "break;"
			case KEY_ENTER: case NC_ENTER: //select the current wall
				wclear(sidewin);
				wrefresh(sidewin);
				break;
		}
		if(!isQuitKey(ch))
			l->map[y][x].wall = charBorders(j);
		mvwaddnwstr(w, y, x, &l->map[y][x].wall, 1);
		wrefresh(w);
	} while(ch != KEY_ENTER && ch != NC_ENTER && !isQuitKey(ch));
	delwin(sidewin);
	wmove(w, y, x);
}

void levelPickSymbol(WINDOW*w, wchar_t*symbol, uint8_t color) {
	int i, j, ch;
	short x, y;
	WINDOW*sidewin;
	bool refreshList = true; //refresh the wall list
	wchar_t outChar = L'\0', initialSymbol = *symbol;
	getyx(w, y, x);
	sidewin = newwin(WIN_HEIGHT, 1, 0, WIN_WIDTH - 1);
	ncflags(sidewin);
	setcolor(sidewin, color);
	setbgcolor(sidewin, color);
	setcolor(w, color);
	wclear(sidewin);
	redrawwin(sidewin);
	j = 0;
	do {
		if(refreshList) {
			wmove(sidewin, 0, 0);
			for(i = 0; i < CHSTUFF_GROUP; i++) {
				waddch(sidewin, ' ');
				outChar = charSymbol((j - (j % CHSTUFF_GROUP)) + i);
				waddnwstr(sidewin, &outChar, 1);
			}
			refreshList = false;
		}
		wmove(sidewin, (j % CHSTUFF_GROUP + 1) * 2 - 1, 0); //move the cursor to the current selected wall
		wrefresh(sidewin);
		ch = wgetch(sidewin);
		switch(ch) {
			case KEY_DOWN: //select the following symbol in the list
				j = (j + 1) % CHSTUFF_SYMBOLS;
				if(j % CHSTUFF_GROUP == 0 || j % CHSTUFF_GROUP == CHSTUFF_GROUP - 1)
					refreshList = true;
				break;
			case KEY_UP: //select the previous symbol in the list
				j = (j - 1 + CHSTUFF_SYMBOLS) % CHSTUFF_SYMBOLS;
				if(	j % CHSTUFF_GROUP == 0 ||
					j % CHSTUFF_GROUP == CHSTUFF_GROUP - 1 ||
					j == CHSTUFF_SYMBOLS - 1
				)	refreshList = true;
				break;
			case KEY_LEFT: //go backward to the previous group of symbols
				j = (j - CHSTUFF_GROUP + CHSTUFF_SYMBOLS) % CHSTUFF_SYMBOLS;
				refreshList = true;
				break;
			case KEY_RIGHT: //go forward to the next group of symbols
				j = (j + CHSTUFF_GROUP) % CHSTUFF_SYMBOLS;
				refreshList = true;
				break;
			case KEY_EXIT: case NC_EXIT: //quit without changing
				*symbol = initialSymbol;
				//intentionally omitted "break;"
			case KEY_ENTER: case NC_ENTER://select the current wall
				wclear(sidewin);
				wrefresh(sidewin);
				break;
		}
		if(!isQuitKey(ch))
			*symbol = charSymbol(j);
		mvwaddnwstr(w, y, x, &(*symbol), 1);
		wrefresh(w);
	} while(ch != KEY_ENTER && ch != NC_ENTER && !isQuitKey(ch));
	delwin(sidewin);
	wmove(w, y, x);
}

void levelChangeSprites(WINDOW*w, short y, short x, int rows, int cols, wchar_t table[][4], uint8_t color[], int defColor) {
	int i = 0, j = 0, ch;
	do {
		wmove(w, y + (i * 2), x + (j * 2)); //sprites are supposed to be spaced
		wrefresh(w);
		ch = tolower(wgetch(w));
		switch(ch) {
			arrowKeys(i, rows, j, cols);
			case 'c':
				levelPickColor(w, y + (i * 2), x + (j * 2), &color[i]);
				break;
			case KEY_ENTER: case NC_ENTER:
				levelPickSymbol(w, &(table[i][j]), bgcolor(defColor) + (fgcolor(color[i]) == (defColor >> 4)) ? 0 : fgcolor(color[i]));
				break;
		}
	} while(!isQuitKey(ch));
}

//END
