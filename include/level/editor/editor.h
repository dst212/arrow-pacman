/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

/*
* Main file of the level editor, only accessible in the "standard" edition.
* Adding -DSERVER to meson's arguments won't include this header, so the level
* editor will be disabled.
*/

enum Editor_status_e {
	EDITOR_FAIL = -1,
	EDITOR_NOTHING = 0,
	EDITOR_OPEN = 1,
	EDITOR_SAVE = 2,
	EDITOR_QUIT = 4
};

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

extern int levelMenu(WINDOW*, Level*, char*, bool*);
extern void levelHelp(void);
extern void levelOptions(WINDOW*, Level*, bool*);
extern int levelEditorUI(WINDOW*, Level*, char*);
extern void levelEditor(void);

// #include "../../../src/level/editor/editor.c"

//END
