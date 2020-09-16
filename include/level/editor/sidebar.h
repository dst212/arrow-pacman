//

void levelPrintCoord(WINDOW*w, short i, const short y, const short x, const short color) {
	setcolor(w, COLOR_BLACK + 8);
	mvwaddch(w, WIN_HEIGHT - (i--), WIN_WIDTH - 1, 'X');
	setcolor(w, color);
	mvwprintw(w, WIN_HEIGHT - (i--), WIN_WIDTH - 1, "%d", x / 10);
	mvwprintw(w, WIN_HEIGHT - (i--), WIN_WIDTH - 1, "%d", x % 10);
	setcolor(w, COLOR_BLACK + 8);
	mvwaddch(w, WIN_HEIGHT - (i--), WIN_WIDTH - 1, 'Y');
	setcolor(w, color);
	mvwprintw(w, WIN_HEIGHT - (i--), WIN_WIDTH - 1, "%d", y / 10);
	mvwprintw(w, WIN_HEIGHT - (i--), WIN_WIDTH - 1, "%d", y % 10);
}

void levelUpdateSideBar(WINDOW*w, const level*l, const short y, const short x, const uint8_t flag) {
	short i = 8;
	curs_set(0);
	ncfillcol(w, 0, WIN_WIDTH - 1, -1, ' ', COLOR_WHITE);
	if(flag == 1) {
		wmove(w, 1, WIN_WIDTH - 1);
		setcolor(w, COLOR_WHITE + 8);
		waddch(w, '+');
	}else if(flag == 3) {
		wmove(w, 1, WIN_WIDTH - 1);
		setcolor(w, COLOR_RED + 8);
		waddnwstr(w, L"×", 1);
	}
	wmove(w, WIN_HEIGHT - i, WIN_WIDTH - 1);
	if(ghostSpawn(l, y, x) != -1) {
		setcolor(w, COLOR_BLACK + 8);
		waddch(w, 'G');
	} else if(isPlayerSpawn(l, y, x)) {
		setcolor(w, COLOR_YELLOW + 8);
		waddch(w, 'P');
	} else if(isPoint(l->map, y, x)) {
		setcolor(w, COLOR_WHITE + 8);
		waddnwstr(w, PACMAN_POINT, 1);
	} else if(isPowerPoint(l->map, y, x)) {
		setcolor(w, COLOR_GREEN + 8);
		waddnwstr(w, PACMAN_PPOINT, 1);
	} else if(isSpace(l->map, y, x)) {
		setcolor(w, COLOR_WHITE + 8);
		waddch(w, 'S');
	} else if(isWall(l->map, y, x)) {
		setcolor(w, COLOR_RED + 8);
		waddch(w, 'W');
	} else if(isBypassable(l->map, y, x)) {
		setcolor(w, COLOR_WHITE);
		waddch(w, 'B');
	} else if(isPortal(l->map, y, x)) {
		setcolor(w, COLOR_MAGENTA + 8);
		waddch(w, 'P');
		levelPrintCoord(w, i + 7, l->map[y][x].portal.y, l->map[y][x].portal.x, COLOR_MAGENTA + 8);
	} else {
		setcolor(w, COLOR_RED);
		waddch(w, '?');
	}
	i -= 2;
	levelPrintCoord(w, i, y, x, ((flag == 3) ? COLOR_RED : COLOR_WHITE) + 8);
	wattron(w, A_NORMAL);
	curs_set(1);
}
#define deniedPos(w, l, y, x) do { levelUpdateSideBar(w, l, y, x, 3); wrefresh(w); msleep(100); } while(0)

//END
