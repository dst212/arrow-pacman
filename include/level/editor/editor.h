/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in main.c at root directory.
 */

/*
* Main file of the level editor, only accessible in the "standard" edition.
* Adding -DSERVER to meson's arguments won't include this header, so the level
* editor will be disabled.
*/

#define EDITOR_FAIL -1
#define EDITOR_NOTHING 0
#define EDITOR_OPEN 1
#define EDITOR_SAVE 2
#define EDITOR_QUIT 4

#define arrowKeys(y, maxy, x, maxx) \
case KEY_LEFT: \
	x = (x - 1 + maxx) % maxx; \
	break; \
case KEY_RIGHT: \
	x = (x + 1) % maxx; \
	break; \
case KEY_UP: \
	y = (y - 1 + maxy) % maxy; \
	break; \
case KEY_DOWN: \
	y = (y + 1) % maxy; \
	break

#define arrowKeysShift(y, maxy, shifty, x, maxx, shiftx) \
arrowKeys(y, maxy, x, maxx); \
case KEY_SLEFT: \
	x = (x - shiftx + maxx) % maxx; \
	break; \
case KEY_SRIGHT: \
	x = (x + shiftx) % maxx; \
	break; \
case KEY_SR: \
	y = (y - shifty + maxy) % maxy; \
	break; \
case KEY_SF: \
	y = (y + shifty) % maxy; \
	break

#include "init.h"
#include "sidebar.h"
#include "pick.h"
#include "editsprites.h"

int levelMenu(WINDOW*w, level*l, char*levelName, bool*saved) {
	//macros' definitions
	#define levelMenuScan(msg) do { \
		wclear(tmpwin); \
		mvncprint(tmpwin, 0, 0, "@P" msg, defColor); \
		wrefresh(tmpwin); \
		do { \
			ncscan(WIN_HEIGHT - 1, getcurx(tmpwin), MAX_STR_SIZE + 1, MAX_STR_SIZE, levelName, PACMAN_FILE_CHARS, defColor); \
		} while(levelName[0] == 0); \
	} while(0)
	#define levelMenuSave() do { \
		wclear(tmpwin); \
		wmove(tmpwin, 0, 0); \
		if(writeLevel(levelName, l)) { \
			ncpopup_info(NC_POPUP_CENTER(w, 2, 25), defColor, "Level saved successfully.", "OK"); \
			/*ncpause(tmpwin, "Level saved successfully. Press any key to continue.");*/ \
			*saved = true; \
			ch = EDITOR_NOTHING; \
		} else \
			ncpopup_info(NC_POPUP_CENTER(w, 2, 25), bgcolor(defColor) + (DEFAULT_COLOR_ERROR >> 4), "Couldn't save the level.", "OK"); \
			/*ncpause(tmpwin, "@PCouldn't save the level. @PPress any key to continue.", bgcolor(defColor) + (DEFAULT_COLOR_ERROR >> 4), defColor);*/ \
	} while(0);
	//end of macros' definitions
	const char*menu[] = {
		" Close menu ",	//0
		" New/Open   ",	//1
		" Save       ",	//2
		" Save as... ",	//3
		" Quit       "	//4
	};
	int ch, defColor = revcolor(DEFAULT_COLOR);
	WINDOW*tmpwin = newwin(1, WIN_WIDTH, WIN_HEIGHT - 1, 0);
	ncflags(tmpwin);
	setbgcolor(tmpwin, defColor);
	setcolor(tmpwin, defColor);
	ch = ncmenu(0, 0, sizeof(menu) / sizeof(char*), strlen(menu[0]) + 1, menu, defColor, revcolor(defColor));
	redrawwin(w);
	wrefresh(w);
	switch(ch) {
		case EDITOR_OPEN:
			//ask whether to discard the current file (if not saved) or not
			if(*saved || !ncpopup_bool(NC_POPUP_CENTER(w, 2, 28), defColor, "Open a new level discarding this one?", NC_POPUP_YES, NC_POPUP_NO) /*tolower(ncask(tmpwin, "Open a new level discarding this one? (Y/N)", "\rYyNn")) == 'y'*/)
				ch = EDITOR_OPEN;
			else
				ch = EDITOR_NOTHING;
			break;
		case EDITOR_QUIT:
			//ask whether to save or not the file (if not saved) before quitting
			//if it's saved or user doesn't want to save it:
			if(*saved || ncpopup_bool(NC_POPUP_CENTER(w, 1, 20), defColor, "Quit without saving?", NC_POPUP_YES, NC_POPUP_NO) /*tolower(ncask(tmpwin, "Quit without saving? (Y/N)", "\rYyNn")) == 'y'*/)
				break;
			//intentionally omitted "break;"
		case EDITOR_SAVE:
			//save the file if it's got a name, else ask a name before
			if(!(*saved)) {
				if(levelName[0] == 0) {
					// wclear(tmpwin);
					levelMenuScan("Save new level as: ");
					if(levelExist(levelName) && !ncpopup_bool(NC_POPUP_CENTER(w, 2, 27), defColor, "This level already exists. Overwrite it?", NC_POPUP_YES, NC_POPUP_NO)/*tolower(ncask(tmpwin, "This level already exists. Overwrite it? (Y/N)", "\rYyNn")) != 'y'*/) {
						wclear(tmpwin);
						// mvncpause(tmpwin, 0, 0, "Nothing was done. Press any key to continue.");
						ncpopup_info(NC_POPUP_CENTER(w, 1, 20), defColor, "Nothing was done.", "OK");
						ch = EDITOR_NOTHING;
						break;
					}
				}
				levelMenuSave();
			}
			ch = EDITOR_NOTHING;
			break;
		case 3:
			levelMenuScan("Save level as: ");
			wclear(tmpwin);
			if(levelExist(levelName) && ncask(tmpwin, "This level already exists. Overwrite it? (Y/N)", "\rYyNn") != 'y') {
				ch = EDITOR_NOTHING;
				break;
			}
			levelMenuSave();
			break;
	}
	delwin(tmpwin);
	redrawwin(w);
	wrefresh(w);
	return ch;
	#undef levelMenuScan
	#undef levelMenuSave
}

void levelHelp() {
	; //lol
}

void levelOptions(WINDOW*w, level*l, bool*saved) {
	const char*menu[] = {
		" Close menu  ",
		" Power ticks "
	};
	WINDOW*tempwin;
	int ch, defColor = revcolor(DEFAULT_COLOR);
	ch = ncmenu(0, 0, sizeof(menu) / sizeof(char*), strlen(menu[0]) + 1, menu, defColor, revcolor(defColor));
	switch(ch) {
		case 0:
			break;
		case 1:
			tempwin = newwin(1, MAP_WIDTH, MAP_HEIGHT - 1, 0);
			ncflags(tempwin);
			ncfillrow(tempwin, 0, 0, -1, ' ', defColor);
			waddstr(tempwin, "Duration of a power-point: ");
			ncnumber(tempwin, getcury(tempwin), getcurx(tempwin), &(l->powerticks), 0, 150);
			delwin(tempwin);
			*saved = false;
			break;
	}
	redrawwin(w);
	wrefresh(w);
}

int levelEditorUI(WINDOW*w, level*l, char*levelName) {
	int i, ch, r = EDITOR_NOTHING;
	short x = 0, y = 0;
	bool saved = true;
	do {
		levelUpdateSideBar(w, l, y, x, false);
		wmove(w, y, x);
		setcolor(w, l->map[y][x].color);
		wrefresh(w);
		ch = tolower(wgetch(w));
		switch(ch) {
			arrowKeysShift(y, MAP_HEIGHT, 5, x, MAP_WIDTH, 10);
			case 'i': //move up by 1 and copy
				y = (y - 1 + MAP_HEIGHT) % MAP_HEIGHT;
				if(!isSpawn(l, y, x)) {
					l->map[y][x] = l->map[(y + 1) % MAP_HEIGHT][x];
				}
				break;
			case 'k': //move down by 1 and copy
				y = (y + 1) % MAP_HEIGHT;
				if(!isSpawn(l, y, x)) {
					l->map[y][x] = l->map[(y - 1 + MAP_HEIGHT) % MAP_HEIGHT][x];
				}
				break;
			case 'j': //move to the left by 1 and copy
				x = (x - 1 + MAP_WIDTH) % MAP_WIDTH;
				if(!isSpawn(l, y, x)) {
					l->map[y][x] = l->map[y][(x + 1) % MAP_WIDTH];
				}
				break;
			case 'l': //move to the right by 1 and copy
				x = (x + 1) % MAP_WIDTH;
				if(!isSpawn(l, y, x)) {
					l->map[y][x] = l->map[y][(x - 1 + MAP_WIDTH) % MAP_WIDTH];
				}
				break;
			case 'u': //update the whole map, if the output is bugged
				printLevel(w, l, 0, 0, MAP_HEIGHT, MAP_WIDTH);
				break;
			case 'v': //fill the whole map
				if(levelPickColor(w, -1, -1, &(l->map[y][x].color))) {
					mapColor(l->map, l->map[y][x].color);
					saved = false;
				}
				break;
			case 'c': //color
				if(levelPickColor(w, y, x, &(l->map[y][x].color)))
					saved = false;
				break;
			case 'a': //cell's appearance
				levelPickWall(w, l, y, x);
				saved = false;
				break;
			case 'w': //make the cell a wall
				if(!isSpawn(l, y, x)) {
					l->map[y][x].portal.x = MAP_WALL;
					l->map[y][x].portal.y = MAP_SOLID;
					levelPickWall(w, l, y, x);
					saved = false;
				} else {
					deniedPos(w, l, y, x);
				}
				break;
			case '.':
				l->map[y][x].portal.x = MAP_NONWALL;
				l->map[y][x].portal.y = MAP_POINT;
				l->map[y][x].wall = PACMAN_POINT[0];
				setcolor(w, l->map[y][x].color);
				waddnwstr(w, &(l->map[y][x].wall), 1);
				saved = false;
				break;
			case '*':
				l->map[y][x].portal.x = MAP_NONWALL;
				l->map[y][x].portal.y = MAP_PPOINT;
				l->map[y][x].wall = PACMAN_PPOINT[0];
				setcolor(w, l->map[y][x].color);
				waddnwstr(w, &(l->map[y][x].wall), 1);
				saved = false;
				break;
			case 'b': //make the cell bypassable
				l->map[y][x].portal.x = MAP_WALL;
				l->map[y][x].portal.y = MAP_BYPASSABLE;
				saved = false;
				break;
			case 'r': //reset to space
				if(l->map[y][x].portal.x == MAP_NONWALL && l->map[y][x].portal.y == MAP_SPACE) {
					l->map[y][x].wall = L' ';
					setcolor(w, l->map[y][x].color);
					waddnwstr(w, &(l->map[y][x].wall), 1);
				} else {
					l->map[y][x].portal.x = MAP_NONWALL;
					l->map[y][x].portal.y = MAP_SPACE;
				}
				saved = false;
				break;
			case 'p': //portal
				levelPlacePortal(w, l);
				saved = false;
				break;
			case 'g': //place/edit a ghost
				//check if no other sprites are in that position
				i = -1;
				if(	((i = ghostSpawn(l, y, x)) != -1) ||
					(	l->ghosts < LVL_MAX_GHOSTS &&
						!isWall(l->map, y, x) &&
						!isPlayerSpawn(l, y, x)
					)
				) {
					levelEditGhost(w, l, (i != -1) ? i : (l->ghosts)++);
					saved = false;
				} else
					deniedPos(w, l, y, x);
				break;
			case 's': //place spawnpoint
				if(isCrossable(l->map, y, x) && ghostSpawn(l, y, x) == -1) {
					if(validCoord(l->player.pos.y, l->player.pos.x))
						printMapCell(w, l, l->player.pos.y, l->player.pos.x);
					levelEditPlayer(w, l);
					saved = false;
				}
				break;
			case 'n': //level options
				levelOptions(w, l, &saved);
				break;
			case 'm': case NC_EXIT: case KEY_EXIT: //open menu
				r = levelMenu(w, l, levelName, &saved);
				break;
			case 'h': case '?': //help
				levelHelp();
				redrawwin(w);
				break;
			case 'f':
				// payRespect();
				break;
		}
		if((ch == 'i' || ch == 'j' || ch == 'k' || ch == 'l') && !isSpawn(l, y, x)) {
			setcolor(w, l->map[y][x].color);
			mvwaddnwstr(w, y, x, &(l->map[y][x].wall), 1);
			saved = false;
		}
	} while(r == EDITOR_NOTHING);
	wclear(w);
	wrefresh(w);
	return r;
}
void levelEditor(void) {
	char levelName[MAX_STR_SIZE + 1] = {0};
	levelStatus s;
	level pacman;
	int8_t reopenFile;
	WINDOW*w;
	w = newwin(WIN_HEIGHT, WIN_WIDTH, 0, 0);
	wclear(stdscr); //used as "logs' window"
	ncflags(w);
	do{
		do {
			wclear(w);
			wrefresh(w);
			reopenFile = 0;
			wprintw(w, "Enter level's name (leave blank to create a new one):\n");
			wrefresh(w);
			ncscan(getcury(w), getcurx(w), MAX_STR_SIZE, MAX_STR_SIZE, levelName, PACMAN_FILE_CHARS, A_NORMAL);
			wclear(w);
			if(levelName[0] == 0) { //no input
				newLevel(&pacman);
			} else {
				s = loadLevel(w, levelName, &pacman, true);
				if(s.status != LVL_OK && s.status <= LVL_FILE_ERRORS) {
					reopenFile = 1;
					lvlError(stdscr, s);
				} else if(s.status == LVL_OK) {
					lvlAutoFix(stdscr, "No errors.\n");
				}
				lvlAutoFixWait(stdscr);
				if(reopenFile == 0) {
					printLevel(w, &pacman, 0, 0, MAP_HEIGHT, MAP_WIDTH);
					redrawwin(w);
					wrefresh(w);
				}
			}
		} while(reopenFile == 1);
	} while(levelEditorUI(w, &pacman, levelName) != EDITOR_QUIT);
}

//END
